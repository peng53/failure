#!/usr/bin/env tclsh8.6
package require Tk
source hierarch_clo.tcl
menu .men
menu .men.mfile
# Create the file menu options
foreach {l c s} [list New init_db 1 Open g_open_db 1 {Save as} g_saveas 0 Save g_save_db 0 Close g_close_db 0 Quit exit_prog 1] {
	.men.mfile add command -label $l -command $c -state [expr {($s==1) ? {normal} : {disabled}}]
}
.men.mfile insert 6 separator
.men.mfile insert 3 separator
.men add cascade -label File -menu .men.mfile
# Create operation bar options.
foreach {l c} [list {Add Group} {modify_grp -1} {Add Link} {modify_row -1} Modify try_modify {Batch Group} switch_grp Delete copy_url] {
	.men add command -label $l -command $c -state disabled
}
. configure -menu .men
set rootg_cb 0
#pack [label .statusbar -text Idle -anchor w] -side bottom -fill x
pack [ttk::treeview .tv_links -columns {title url mtime} -yscrollcommand {.tv_links_sb set}] -side left -fill both -expand 1
pack [scrollbar .tv_links_sb -command {.tv_links yview}] -side left -fill y

foreach {c l w} [list #0 Groups/RowID 128 title Name 128 url URL 256 mtime {Time Modified} 128] {
	# this loop adjusts the treeview's columns
	.tv_links heading $c -text $l -anchor w
	.tv_links column $c -minwidth 16 -width $w
}
.tv_links heading #0 -command {reorder_#0 {}}
.tv_links heading title -command {reorder_title {}}

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
		close_db
	}
	set s [tk_getOpenFile -defaultextension .db -filetypes {{{Bookmarks DB} .db}} -title {Load Bookmarks}]
	if {[string length $s] > 0} {
		open_db_i $s
		menu_is_open
		load_rows
		puts "Opened DB $s"
		wm title . "bkmks - $s"
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
}
proc g_close_db {} {
	# Doesn't actually close anything.
	# Clears rows visually, wipes :memory:, and disables options.
	close_db
	.tv_links delete [.tv_links children {}]
	menu_is_open
	puts "DB status: $DBConn::is_open"
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
		.tv_links insert {} end -id "g$gid" -text [dict get $DBConn::groups $gid]
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
		foreach {l cc} [list a sel_all c copy d del_sel r del_paste] {
			pack [button $w.b$l -text $l -command "entry_$cc $w.e" -takefocus 0] -side left
		}
	}
	grp_frame
	set srn [expr {($rownumber>0) ? $rownumber : {NEW}}]
	pack [label .sub.l -text "LINK #: $srn"]
	pack [frame .sub.buttons] -side bottom
	pack [button .sub.buttons.save -text Save -command "save_row $rownumber"] -side left
	pack [button .sub.buttons.cancel -text Cancel -command {destroy .sub}] -side left
	if {$rownumber != -1} {
		# Prefill fields if possible.
		lassign [.tv_links item $rownumber -values] t u
		.sub.t.e insert 0 $t
		.sub.u.e insert 0 $u
		if {[set p [.tv_links parent $rownumber]]!={}} {
			# Set selected group to that of row's
			#.win_row_mod.grp.root deselect
			set rootg_cb 0
			.sub.grp.cb set [dict get $DBConn::groups [string range $p 1 end]]
		} else {
			# If the selected is a root key, check the button instead
			.sub.grp.root select
		}
		g_grp_root_onoff
	} else {
		#otherwise, if something was selected in treeview, select the group here.
		set q_grp [.tv_links selection]
		if {[llength $q_grp]==1} {
			if {[string index $q_grp 0]!={g}} {
				set q_grp [.tv_links parent $q_grp]
			}
			.sub.grp.root deselect
			.sub.grp.cb set [dict get $DBConn::groups [string range $q_grp 1 end]]
		}
	}
	bind .sub <Escape> {
		destroy %W
	}
	bind .sub.t.e <Shift-Button-1> {
		%W selection range 0 end
	}
	bind .sub.u.e <Shift-Button-1> {
		%W selection range 0 end
	}
}
proc save_row {rownumber} {
	# Takes properties from 'win_row_mod' window and
	# saves them to both database & treeview with
	# current time. A rownumber of -1 implies a new
	# row. This proc also closes 'win_row_mod'.
	set t [.sub.t.e get]
	set u [.sub.u.e get]
	set m [clock format [clock seconds] -format {%D - %R}]
	global rootg_cb
	if {$rootg_cb==1} {
		set g 0
		set k {}
	} else {
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
	pack [checkbutton .sub.grp.root -text Root -variable rootg_cb -command g_grp_root_onoff] -side left
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
	pack [button .sub.b.apply -text Apply -command apply_g_chg] -side left
	pack [button .sub.b.cancel -text Cancel -command {destroy .sub}] -side left
}
proc apply_g_chg {} {
	# See switch_grp. The actually changes the 'gid' by
	# calling procs in hierarch_clo.
	global rootg_cb
	if {$rootg_cb==1} {
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
	pack [button .sub.b.save -text Save -command "save_grp $gid"] -side left
	pack [button .sub.b.cancel -text Cancel -command {destroy .sub}] -side left

	if {$gid!=-1} {
		# existing group
		set p [.tv_links parent g$gid]
		if {[string length $p]>0} {
			.sub.grp.root deselect
			.sub.grp.cb set [dict get $DBConn::groups [string range $p 1 end]]
		} else {
			.sub.grp.root select
		}
		.sub.n.e insert 0 [dict get $DBConn::groups $gid]
	} else {
		# otherwise, if something was selected in treeview, select the group here.
		set q_grp [.tv_links selection]
		if {[llength $q_grp]==1} {
			if {[string index $q_grp 0]!={g}} {
				set q_grp [.tv_links parent $q_grp]
			}
			.sub.grp.root deselect
			.sub.grp.cb set [dict get $DBConn::groups [string range $q_grp 1 end]]
		}
	}
	g_grp_root_onoff
	bind .sub <Escape> {
		destroy %W
	}
}
proc save_grp {gid} {
	# Save changes to group (gid is just an int)
	set n [.sub.n.e get]
	global rootg_cb
	if {$rootg_cb==1} {
		set p 0
		set k {}
	} else {
		set k g[set p [lindex [dict keys $DBConn::groups] [.sub.grp.cb current]]]
	}
	if {$gid == -1} {
		# a new group.
		set gid [add_group $n $p]
		.tv_links insert $k end -id g$gid -text $n
	} else {
		# need to check whether the parent is the child.
		if {$gid==$p} {
			# TODO: an error message
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
proc g_grp_root_onoff {} {
	global rootg_cb
	if {$rootg_cb==1} {
		set m select
		set s disabled
	} else {
		set m deselect
		set s normal
		if {[.sub.grp.cb get]=={}} {
			if {[dict size $DBConn::groups]>0} {
				.sub.grp.cb current 0
			}
		}
	}
	.sub.grp.root $m
	.sub.grp.cb configure -state $s
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
	for {set i 2} {$i<8} {incr i} {
		.men entryconfigure $i -state $nstate
	}
	.men.mfile entryconfigure 4 -state $nstate
	# Save as
	.men.mfile entryconfigure 5 -state $nstate
	# Save
	.men.mfile entryconfigure 6 -state $nstate
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
proc reorder_title {c} {
	set g [list]
	set d [list]
	foreach r [.tv_links children $c] {
		if {[string index $r 0]=={g}} {
			lappend g $r
		} else {
			lassign [.tv_links item $r -values] title url mtime
			lappend d [list $title $r]
		}
		.tv_links detach $r
	}
	foreach {v} [lsort -index 0 $d] {
		lassign $v k r
		.tv_links move $r $c end
	}
	foreach r [lsort $g] {
		.tv_links move $r $c end
		.tv_links move $r $c end
		reorder_title $r
	}
}
proc sub_win {t} {
	toplevel .sub
	grab set .sub
	wm attributes .sub -topmost 1
	wm resizable .sub 0 0
	wm title .sub $t
}
bind .tv_links <c> {
	copy_url
}
