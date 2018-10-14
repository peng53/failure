#!/usr/bin/env tclsh8.6
package require Tk
source hierarch_clo.tcl
menu .men
menu .men.mfile
# Create the file menu options
foreach {l c s} [list New init_db 1 Open g_open_db 1 {Import sqlite} firefox_bm 1 {Save as} g_saveas 0 Save g_save_db 0 Close g_close_db 0 Quit exit_prog 1] {
	.men.mfile add command -label $l -command $c -state [expr {$s ? {normal} : {disabled}}]
}
.men.mfile insert 7 separator
.men.mfile insert 4 separator

.men add cascade -label File -menu .men.mfile
# Create operation bar options.
foreach {l c} [list {Add Group} {modify_grp -1} {Add Link} {modify_row -1} Modify try_modify {Batch Group} switch_grp Delete copy_url {Search} g_search {Auto Group} g_auto_grp] {
	.men add command -label $l -command $c -state disabled
}
#.men add command -label Import -command firefox_bm
#.men add command -label {Auto Group} -command g_auto_grp
. configure -menu .men
set rootg_cb false
set s_area key 
#pack [label .statusbar -text Idle -anchor w] -side bottom -fill x
pack [ttk::treeview .tv_links -columns {title url mtime} -yscrollcommand {.tv_links_sb set}] -side left -fill both -expand 1
pack [scrollbar .tv_links_sb -command {.tv_links yview}] -side left -fill y

set i -1
foreach {c l w} [list #0 Groups/RowID 128 title Name 128 url URL 256 mtime {Time Modified} 170] {
	# this loop adjusts the treeview's columns
	.tv_links heading $c -text $l -anchor w -command "reorder_rows_q $i {}"
	.tv_links column $c -minwidth 16 -width $w
	incr i
}

proc init_db {} {
	# Creates a working database in memory. Doesn't work if DB is already open.
	if {$DBConn::is_open} {
		set saved [tk_messageBox -icon question -message {DB Already open. Proceed?} -type yesno -title Prompt]
		if {[string equal $saved no]} {
			return
		}
		close_db
	}
	prepare_memory
	menu_is_open
}
proc g_open_db {} {
	# Opens a database and reads it.
	if {$DBConn::is_open} {
		set saved [tk_messageBox -icon question -message {DB Already open. Proceed?} -type yesno -title Prompt]
		if {[string equal $saved no]} {
			return
		}
		g_close_db
	}
	set s [tk_getOpenFile -defaultextension .db -filetypes {{{Bookmarks DB} .db}} -title {Load Bookmarks}]
	if {[string length $s] > 0} {
		open_db_i $s
		menu_is_open
		load_rows
		puts "Opened DB $s"
		wm title . "bkmkg - $s"
	}
	puts "DB status: $DBConn::is_open"
}
proc g_save_db {} {
	# Saves rows to file. If no current file name is stored,
	# prompt g_save_as
	if {!$DBConn::is_open} {
		return
	}
	if {[string length $DBConn::current_file]==0} {
		g_saveas
	} else {
		save_db_i
		puts "Saved rows to $DBConn::current_file"
	}
}

proc g_saveas {} {
	# Saves rows to a chosen file.
	# Beware: that file is deleted.
	if {!$DBConn::is_open} {
		return
	}
	set file_name [tk_getSaveFile -defaultextension .db -filetypes {{{Bookmarks DB} .db}} -title {Save database as..}]
	if {[string length $file_name] == 0} { return }
	if {[file exists $file_name]} { file delete $file_name }
	set DBConn::current_file $file_name
	save_db_i
	puts "Saved rows to $DBConn::current_file"
}
proc g_close_db {} {
	# Doesn't actually close anything.
	# Clears rows visually, wipes :memory:, and disables options.
	puts "Closed file $DBConn::current_file"
	close_db
	.tv_links delete [.tv_links children {}]
	menu_is_open

}
proc load_rows {} {
	# Loads rows of DB to treeview.
	# Group items' ids are prepend with g
	# while data items are just their rowid.
	# First, the root groups
	set groups [list]
	conn eval {
		SELECT gid FROM rel WHERE pid IS NULL;
	} {
		.tv_links insert {} end -id "g$gid" -text [dict get $DBConn::groups $gid] -open 1
		lappend groups $gid
	}
	# Second, other groups
	foreach g $groups {
		conn eval {
			SELECT gid,pid FROM rel WHERE gid IN
				(SELECT gid FROM rel WHERE pid=:g AND gid!=pid)
			and depth=1 and pid IS NOT NULL;
		} {
			.tv_links insert "g$pid" end -id "g$gid" -text [dict get $DBConn::groups $gid]
		}
	}
	# Lastly, the data
	conn eval {
		SELECT rowid,ifnull(gid,0) as gid,key,value,mtime FROM data;
	} {
		.tv_links insert [expr {($gid==0)?{}:"g$gid"}] end -id $rowid -text $rowid -values [list $key $value $mtime]
	}
}

proc modify_row {rownumber} {
	# Opens a window for entering/modifying properties of
	# a row. Only allows edit of title, url, and group. This proc takes
	# values from the treeview only, which should be okay since
	# modification of the treeview is limited.
	sub_win {Modify link..}
	foreach {p n} [list t Title u URL] {
		set w .sub.$p
		pack [labelframe .sub.$p -text $n]
		pack [entry $w.e -width 50] -side left
		bind $w.e <Shift-Button-1> {
			%W selection range 0 end
		}
		foreach {l cc} [list a sel_all c copy d del_sel r del_paste] {
			pack [button $w.b$l -text $l -command "entry_$cc $w.e" -takefocus 0] -side left
		}
	}
	grp_frame
	set srn [expr {($rownumber>0) ? $rownumber : {NEW}}]
	pack [label .sub.l -text "LINK #: $srn"]
	pack [frame .sub.buttons] -side bottom
	pack [button .sub.buttons.save -text Save -command "modify_row_proc $rownumber"] -side left
	pack [button .sub.buttons.cancel -text Cancel -command {destroy .sub}] -side left
	if {$rownumber != -1} {
		# Prefill fields if possible.
		lassign [.tv_links item $rownumber -values] t u
		.sub.t.e insert 0 $t
		.sub.u.e insert 0 $u
	}
}
proc modify_row_proc {rownumber} {
	# Takes properties from 'win_row_mod' window and
	# saves them to both database & treeview with
	# current time. A rownumber of -1 implies a new
	# row. This proc also closes 'win_row_mod'.
	set t [.sub.t.e get]
	set u [.sub.u.e get]
	set m [clock format [clock seconds] -format {%Y-%m-%d %T.000}]
	global rootg_cb
	if {$rootg_cb==1 || [string length [.sub.grp.cb get]]==0} {
		set g 0
		set k {}
	} else {
		puts "this"
		set k g[set g [lindex [dict keys $DBConn::groups] [.sub.grp.cb current]]]
	}
	if {$rownumber == -1} {
		set rownumber [add_data $t $u $m $g]
		# note : need sql for get id (which is also text)
		.tv_links insert $k end -id $rownumber -text $rownumber -value [list $t $u $m]
	} else {
		update_data $rownumber $t $u $m $g
		.tv_links item $rownumber -value [list $t $u $m]
		if {![string equal $k [.tv_links parent $rownumber]]} {
			.tv_links move $rownumber $k end
		}
	}
	destroy .sub
}
proc grp_frame {} {
	# Creates the widget for group selection.
	# A combobox and a checkbutton in a labelframe.
	global rootg_cb
	pack [labelframe .sub.grp -text Group] -fill x
	pack [ttk::combobox .sub.grp.cb -values [dict values $DBConn::groups] -state readonly] -fill x -expand 1 -side left
	pack [checkbutton .sub.grp.root -text Root -variable rootg_cb -command grp_frame_root] -side left
	if {[llength [set grp [.tv_links selection]]]==0} {
		set rootg_cb true
	} else {
		set grp [closest_parent $grp]
		if {[string length $grp]==0} {
			set rootg_cb true
		} else {
			.sub.grp.cb set [dict get $DBConn::groups [string range $grp 1 end]]
			set rootg_cb false
		}
	}
	if {$rootg_cb && [dict size $DBConn::groups]>0} {
		.sub.grp.cb current 0
	}
	grp_frame_root
}
proc closest_parent {items} {
	# returns item id.
	if {[llength $items]==0} {
		return {}
	} elseif {[string equal [string index [set grp [lindex $items 0]] 0] g]} {
		return $grp
	} else {
		return [.tv_links parent $grp]
	}
}

proc grp_frame_root {} {
	global rootg_cb
	if {$rootg_cb} {
		set m select
		set s disabled
	} else {
		set m deselect
		set s normal
		if {[string length [.sub.grp.cb get]]==0 && [dict size $DBConn::groups]>0} {
			.sub.grp.cb current 0
		}
	}
	.sub.grp.root $m
	.sub.grp.cb configure -state $s
}
proc switch_grp {} {
	# Allows user to change the group of multiple
	# items in batch.
	if {[llength [.tv_links selection]]==0} {
		return
	}
	sub_win {Switch to group..}
	grp_frame
	pack [frame .sub.b]
	pack [button .sub.b.apply -text Apply -command switch_grp_proc] -side left
	pack [button .sub.b.cancel -text Cancel -command {destroy .sub}] -side left
}
proc switch_grp_proc {} {
	# See switch_grp. The actually changes the 'gid' by
	# calling procs in hierarch_clo.
	global rootg_cb
	if {$rootg_cb} {
		set p 0
		set k {}
	} else {
		set k g[set p [lindex [dict keys $DBConn::groups] [.sub.grp.cb current]]]
	}
	set rowids [.tv_links selection]
	conn eval { BEGIN TRANSACTION; }
	foreach r $rowids {
		if {[string index $r 0]!={g}} {
			data_group $r $p
			.tv_links move $r $k end
		}
	}
	conn eval { END TRANSACTION; }
	destroy .sub
}

proc modify_grp {gid} {
	# Change a group's parent and/or name
	sub_win {Modify Group..}
	grp_frame
	pack [labelframe .sub.n -text Name]
	pack [entry .sub.n.e -width 50]
	pack [frame .sub.b]
	pack [button .sub.b.save -text Save -command "modify_grp_proc $gid"] -side left
	pack [button .sub.b.cancel -text Cancel -command {destroy .sub}] -side left
	if {$gid!=-1} {	# existing group
		.sub.n.e insert 0 [dict get $DBConn::groups $gid]
	} 
	bind .sub <Escape> {
		destroy %W
	}
}
proc modify_grp_proc {gid} {
	# Save changes to group (gid is just an int)
	set n [.sub.n.e get]
	global rootg_cb
	if {$rootg_cb} {
		set p 0
		set k {}
	} else {
		set k g[set p [lindex [dict keys $DBConn::groups] [.sub.grp.cb current]]]
	}
	if {$gid == -1} {
		.tv_links insert $k end -id g[add_group $n $p] -text $n
	} else {
		# need to check whether the parent is the child.
		if {$gid==$p} {
			tk_messageBox -type ok -icon error -message {Group's parent cannot be itself!}
			return
		}
		update_grp $gid $n
		.tv_links item g$gid -text $n
		if {$p!=[string range [.tv_links parent g$gid] 1 end]} {
			# there was a change in parent group
			change_pgroup $gid $p
			.tv_links move g$gid $k end
		}
	}
	destroy .sub
}
proc try_modify {} {
	# Modify a row if there is a selection
	set rownumbers [.tv_links selection]
	if {[llength $rownumbers] > 0} {
		conn eval {BEGIN TRANSACTION;}
		foreach r $rownumbers {
			if {[string index $r 0]!={g}} {
				modify_row $r
			} else {
				modify_grp [string range $r 1 end]
			}
			tkwait window .sub
		}
		conn eval {END TRANSACTION;}
	} else {
		tk_messageBox -type ok -icon error -message {Please select link to modify.}
	}
}
proc try_delete {} {
	# Deletes a row or group
	# Will only delete a group if multiple is selected
	set rownumbers [.tv_links selection]
	set n [llength $rownumbers]
	switch $n {
		0 {
			return
		}
		1 {
			if {[string index $rownumbers 0]=={g}} {
				# its a group
				del_group $rownumbers
				.tv_links delete $rownumbers
			} else {
				# its a row
				rm_data $rownumbers
				.tv_links delete $rownumbers
			}
		}
		default {
			conn eval {BEGIN TRANSACTION;}
			foreach r $rownumbers {
				if {[string index $r 0]!={g}} {
					rm_data $r
					.tv_links delete $r
				}
			}
			conn eval {END TRANSACTION;}
		}
	}
}
proc exit_prog {} {
	# Closes the database and then the program.
	if {[string equal [tk_messageBox -icon question -message {Quit program?} -type yesno -title Prompt] no]} { return }
	close_db
	destroy .
}
proc menu_is_open {} {
	# Toggles menu options based on file state.
	set nstate [expr {($DBConn::is_open) ? "normal" : "disabled"}]
	for {set i 2} {$i<9} {incr i} {
		.men entryconfigure $i -state $nstate
	}
	.men.mfile entryconfigure 5 -state $nstate
	# Save as
	.men.mfile entryconfigure 6 -state $nstate
	# Save
	.men.mfile entryconfigure 7 -state $nstate
	# Close
}
proc entry_sel_all {e} {
	# Selects all the text in an entry
	focus $e
	$e selection range 0 end
}
proc entry_del_sel {e} {
	# Deletes selected text in an entry
	if {[$e selection present]} {
		$e delete sel.first sel.last
	}
}
proc entry_copy {e} {
	# Copies either selection of entry or entire entry.
	set s [$e get]
	if {[$e selection present]} {
		set s [string range $s [$e index sel.first] [$e index sel.last]]
	}
	if {[string length $s]>0} {
		clipboard clear
		clipboard append $s
	}
}
proc entry_del_paste {e} {
	# Replaces contents of entry with whats in clipboard
	set s [clipboard get]
	if {[string length $s]>0} {
		$e delete 0 end
		$e insert 0 $s
	}
}
proc copy_url {} {
	# Sets clipboard to url of selected row
	# If no row is selected, nothing happens
	# and clipboard is left same
	set r [.tv_links selection]
	if {[llength $r] > 0} {
		lassign [.tv_links item [lindex $r 0] -values] title url mtime
		clipboard clear
		clipboard append $url
	}
}
proc reorder_#0 {c} {
	set rows [lsort -increasing [.tv_links children $c]]
	.tv_links detach $rows
	foreach r $rows {
		.tv_links move $r $c end
		if {[string index $r 0]=={g}} {
			reorder_#0 $r
		}
	}
}
proc reorder_rows_q {bycol fp} {
	# Reorder the items starting at $c by a column.
	puts "Reordered by column $bycol"
	if {$bycol==-1} {
		reorder_#0 {}
		return
	}
	set i 0
	set ql 1
	set Q [list $fp]
	while {$i<$ql} {
		set c [lindex $Q $i]
		set D [list]
		foreach {r} [lsort [.tv_links children $c]] {
			if {[string index $r 0]=={g}} {
				.tv_links move $r $c end
				lappend Q $r
				incr ql
			} else {
				lappend D [list $r [lindex [.tv_links item $r -values] $bycol]]
				.tv_links detach $r
			}
		}
		foreach {i_i} [lsort -indices -index 1 $D] {
			.tv_links move [lindex [lindex $D $i_i] 0] $c end
		}
		incr i
	}
}
proc sub_win {t {lf false}} {
	# Creates the sub-window with default options and title.
	if {$lf} {
		pack [labelframe .sub -text $t]
	} else {
		toplevel .sub
		grab set .sub
		wm attributes .sub -topmost 1
		wm resizable .sub 0 0
		wm title .sub $t
	}
	bind .sub <Escape> {
		destroy %W
	}
}
proc firefox_bm {} {
	set myfile [tk_getOpenFile -defaultextension .sqlite -filetypes {{{Firefox places.sqlite} .sqlite}} -title {Select places.sqlite..}]
	if {[string length $myfile]==0} return
	set mtime [clock format [clock seconds] -format {%Y-%m-%d %T.000}]
	set ngid [import_places $myfile $mtime]
	switch $ngid {
		-1 {
			puts {NO FILE OPEN!!}
		}
		0 {
			puts {IMPORT GROUP EXISTS ALREADY!!}
		}
		default {
			.tv_links insert {} end -id "g$ngid" -text [format {places.sqlite %s} $mtime]
			conn eval {
				SELECT rowid,key,value FROM data WHERE gid=:ngid;
			} {
				.tv_links insert "g$ngid" end -id $rowid -text $rowid -values [list $key $value $mtime]
			}
		}
	}
}
proc g_auto_grp {} {
	global rootg_cb search_all
	set search_all false
	sub_win {Auto group..}
	grp_frame
	pack [labelframe .sub.pattern -text Pattern:] -fill x	
	pack [entry .sub.pattern.en] -fill x -expand 1 -side left
	pack [labelframe .sub.to -text To:] -fill x
	pack [ttk::combobox .sub.to.cb -values [dict values $DBConn::groups] -state readonly] -fill x -expand 1 -side left
	pack [frame .sub.b]
	pack [button .sub.b.pre -text Preview -command {g_search_proc}] -side left
	pack [button .sub.b.ok -text Ok -command {g_auto_grp_proc}] -side left
	pack [button .sub.b.exit -text Exit -command {destroy .sub}] -side left
	if {[dict size $DBConn::groups]>0} {
		.sub.to.cb current 0
	}
	bind .sub <Alt-a> {
		.sub.pattern.en insert end .*
	}
	bind .sub.pattern.en <Return> {
		g_auto_grp_proc true
	}
}
proc g_auto_grp_proc {} {
	if {[string length [set pattern [.sub.pattern.en get]]]==0} {
		tk_messageBox -type ok -icon error -message {Please enter a pattern.}
		return
	}
	global rootg_cb search_all
	set f [expr {($rootg_cb) ? {NULL} : [lindex [dict keys $DBConn::groups] [.sub.grp.cb current]]}]
	set t [lindex [dict keys $DBConn::groups] [.sub.to.cb current]]
	# output a search.
	# if f == t, then only a dry_run is done
	
	set to_move [search_data $pattern key $f]
	puts $to_move
	auto_group $f $t $pattern
	foreach r $to_move {
		.tv_links move $r g$t end
	}
}
proc g_search {} {
	sub_win {Search Data}
	pack [labelframe .sub.pattern -text Pattern] -fill x
	pack [entry .sub.pattern.en] -fill x -expand 1
	global rootg_cb search_all s_area
	grp_frame
	pack [checkbutton .sub.grp.sall -text Any -variable search_all] -side left
	pack [labelframe .sub.area -text Area] -fill x
	pack [radiobutton .sub.area.key -text Key -variable s_area -value key] -side left
	pack [radiobutton .sub.area.value -text Value -variable s_area -value value] -side left
	pack [radiobutton .sub.area.both -text Both -variable s_area -value both] -side left
	pack [frame .sub.b]
	pack [button .sub.b.go -text Search -command g_search_proc] -side left
	pack [button .sub.b.exit -text Exit -command {destroy .sub}] -side left
	bind .sub <Alt-a> {
		.sub.pattern.en insert end .*
	}
	bind .sub.pattern.en <Return> {
		g_search_proc
	}
}
proc g_search_proc {} {
	if {[string length [set pattern [.sub.pattern.en get]]]==0} {
		tk_messageBox -type ok -icon error -message {Please enter a pattern.}
		return
	}
	global rootg_cb search_all s_area
	if {$search_all} {
		set parent {}
	} elseif {$rootg_cb} {
		set parent NULL
	} else {
		set parent [lindex [dict keys $DBConn::groups] [.sub.grp.cb current]]
		if {[string length $parent]==0} return		
	}
	puts "Searched \'$pattern\' in group \'$parent\' with mode \'$s_area\'."
	.tv_links selection set [search_data $pattern $s_area $parent]
}

bind . <Control-n> {
	init_db
}
bind . <Control-c> {
	g_close_db
}
bind . <Control-s> {
	g_save_db
}
bind . <Control-o> {
	g_open_db
}
bind . <Control-q> {
	exit_prog
}
bind .tv_links <c> {
	copy_url
}
bind .tv_links <Return> {
	try_modify
}
bind .tv_links <Delete> {
	try_delete
}
bind . <Control-i> {
	firefox_bm
}
