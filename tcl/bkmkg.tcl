#!/usr/bin/env tclsh8.6
package require Tk
source hierarch_clo.tcl
menu .men
menu .men.mfile
.men.mfile add command -label New -command init_db -underline 0
.men.mfile add command -label Open -command g_open_db -underline 0
.men.mfile add separator
.men.mfile add command -label {Save as} -command g_saveas -underline 0 -state disabled
.men.mfile add command -label Save -command g_save_db -underline 0 -state disabled
.men.mfile add command -label Close -command g_close_db -underline 0 -state disabled
.men.mfile add separator
.men.mfile add command -label Quit -command exit_prog -underline 0
.men add cascade -label File -menu .men.mfile
.men add command -label {Add Group} -command {modify_grp -1} -state disabled
.men add command -label {Add Link} -command {modify_row -1} -state disabled
.men add command -label Modify -command try_modify -state disabled
.men add command -label Delete -command try_delete -state disabled
.men add command -label {Copy URL} -command copy_url -underline 0 -state disabled
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

proc init_db {} {
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
	close_db
	.tv_links delete [.tv_links children {}]
	menu_is_open
	puts "DB status: $DBConn::is_open"
}
proc load_rows {} {
	# Loads rows of DB to treeview.
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
		puts $g
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
	# a row. Only allows edit of title & url. This proc takes
	# values from the treeview only, which should be okay since
	# modification of the treeview is limited.
	toplevel .win_row_mod
	grab set .win_row_mod
	wm attributes .win_row_mod -topmost 1
	wm title .win_row_mod {Modify link..}
	wm resizable .win_row_mod 0 0
	pack [labelframe .win_row_mod.t -text Title]
	pack [entry .win_row_mod.t.e -width 50] -side left
	pack [button .win_row_mod.t.ba -text A -command {entry_sel_all .win_row_mod.t.e}] -side left
	pack [button .win_row_mod.t.bc -text C -command {entry_copy .win_row_mod.t.e}] -side left
	pack [button .win_row_mod.t.bd -text D -command {entry_del_sel .win_row_mod.t.e}] -side left
	pack [button .win_row_mod.t.br -text R -command {entry_del_paste .win_row_mod.t.e}] -side left
	pack [labelframe .win_row_mod.u -text URL]
	pack [entry .win_row_mod.u.e -width 50] -side left
	pack [button .win_row_mod.u.ba -text A -command {entry_sel_all .win_row_mod.u.e}] -side left
	pack [button .win_row_mod.u.bc -text C -command {entry_copy .win_row_mod.u.e}] -side left
	pack [button .win_row_mod.u.bd -text D -command {entry_del_sel .win_row_mod.u.e}] -side left
	pack [button .win_row_mod.u.br -text R -command {entry_del_paste .win_row_mod.u.e}] -side left
	pack [labelframe .win_row_mod.grp -text Group] -fill x
	pack [ttk::combobox .win_row_mod.grp.cb -values [dict values $DBConn::groups] -state readonly] -fill x -expand 1 -side left
	global rootg_cb
	pack [checkbutton .win_row_mod.grp.root -text Root -command {g_grp_root_onoff .win_row_mod} -variable rootg_cb] -side left
	set srn [expr {($rownumber>0) ? $rownumber : "NEW"}]
	pack [label .win_row_mod.l -text "LINK #: $srn"]
	pack [frame .win_row_mod.buttons] -side bottom
	pack [button .win_row_mod.buttons.save -text Save -command "save_row $rownumber"] -side left
	pack [button .win_row_mod.buttons.cancel -text Cancel -command {destroy .win_row_mod}] -side left
	if {$rownumber != -1} {
		lassign [.tv_links item $rownumber -values] t u
		.win_row_mod.t.e insert 0 $t
		.win_row_mod.u.e insert 0 $u
		if {[set p [.tv_links parent $rownumber]]!={}} {
			# Set selected group to that of row's
			#.win_row_mod.grp.root deselect
			set rootg_cb 0
			.win_row_mod.grp.cb set [dict get $DBConn::groups [string range $p 1 end]]
		} else {
			# If the selected is a root key, check the button instead
			.win_row_mod.grp.root select
		}
		g_grp_root_onoff .win_row_mod
	} else {
		#otherwise, if something was selected in treeview, select the group here.
		set q_grp [.tv_links selection]
		if {[llength $q_grp]==1} {
			if {[string index $q_grp 0]!={g}} {
				set q_grp [.tv_links parent $q_grp]
			}
			.win_row_mod.grp.root deselect
			.win_row_mod.grp.cb set [dict get $DBConn::groups [string range $q_grp 1 end]]
		}
	}
	bind .win_row_mod <Escape> {
		destroy %W
	}
	bind .win_row_mod.t.e <Shift-Button-1> {
		%W selection range 0 end
	}
	bind .win_row_mod.u.e <Shift-Button-1> {
		%W selection range 0 end
	}
}
proc save_row {rownumber} {
	# Takes properties from 'win_row_mod' window and
	# saves them to both database & treeview with
	# current time. A rownumber of -1 implies a new
	# row. This proc also closes 'win_row_mod'.
	set t [.win_row_mod.t.e get]
	set u [.win_row_mod.u.e get]
	set m [clock format [clock seconds] -format {%D - %R}]
	#puts $DBConn::groups
	#set g [lindex $DBConn::groups [expr {2*[.win_row_mod.grp.cb current]+1}]]
	#set g [lindex [dict keys $DB::groups] .win_row_mod.grp.cb]
	global rootg_cb
	if {$rootg_cb==1} {
		set g 0
	} else {
		set g [lindex [dict keys $DBConn::groups] [.win_row_mod.grp.cb current]]
	}
	if {$rownumber == -1} {
		set rownumber [add_data $t $u $m $g]
		# note : need sql for get id (which is also text)
		if {$rootg_cb==1} {
			.tv_links insert {} end -id $rownumber -text $rownumber -value [list $t $u $m]
		} else {
		.tv_links insert g$g end -id $rownumber -text $rownumber -value [list $t $u $m]
		}
	} else {
		update_data $rownumber $t $u $m $g
		.tv_links item $rownumber -value [list $t $u $m]
		if {$rootg_cb==1} {
			if {[string length [.tv_links parent $rownumber]]>0} {
				.tv_links move $rownumber {} end
			}
		} else {
			if {[.tv_links parent $rownumber]!="g$g"} {
				.tv_links move $rownumber g$g end
			}
		}
	}
	destroy .win_row_mod
}

proc modify_grp {gid} {
	# Change a group's parent and/or name
	toplevel .win_grp_mod
	grab set .win_grp_mod
	wm attributes .win_grp_mod -topmost 1
	wm title .win_grp_mod {Modify Group..}
	pack [labelframe .win_grp_mod.grp -text Parent] -fill x
	pack [ttk::combobox .win_grp_mod.grp.cb -values [dict values $DBConn::groups] -state readonly] -fill x -expand 1 -side left
	global rootg_cb
	pack [checkbutton .win_grp_mod.grp.root -text Root -variable rootg_cb -command {g_grp_root_onoff .win_grp_mod}] -side left
	pack [labelframe .win_grp_mod.n -text Name]
	pack [entry .win_grp_mod.n.e -width 50]
	pack [frame .win_grp_mod.b]
	pack [button .win_grp_mod.b.save -text Save -command "save_grp $gid"] -side left
	pack [button .win_grp_mod.b.cancel -text Cancel -command {destroy .win_grp_mod}] -side left

	if {$gid!=-1} {
		# existing group
		set p [.tv_links parent g$gid]
		if {[string length $p]>0} {
			.win_grp_mod.grp.root deselect
			.win_grp_mod.grp.cb set [dict get $DBConn::groups [string range $p 1 end]]
		} else {
			.win_grp_mod.grp.root select
		}
		.win_grp_mod.n.e insert 0 [dict get $DBConn::groups $gid]
	} else {
		# otherwise, if something was selected in treeview, select the group here.
		set q_grp [.tv_links selection]
		if {[llength $q_grp]==1} {
			if {[string index $q_grp 0]!={g}} {
				set q_grp [.tv_links parent $q_grp]
			}
			.win_grp_mod.grp.root deselect
			.win_grp_mod.grp.cb set [dict get $DBConn::groups [string range $q_grp 1 end]]
		}
	}
	g_grp_root_onoff .win_grp_mod
	bind .win_grp_mod <Escape> {
		destroy %W
	}
}
proc save_grp {gid} {
	# Save changes to group (gid is just an int)
	set n [.win_grp_mod.n.e get]
	global rootg_cb
	if {$rootg_cb==1} {
		set p 0
	} else {
		set p [lindex [dict keys $DBConn::groups] [.win_grp_mod.grp.cb current]]
	}
	if {$gid == -1} {
		# a new group.
		set gid [add_group $n $p]
		if {$p==0} {
			.tv_links insert {} end -id g$gid -text $n
		} else {
			.tv_links insert g$p end -id g$gid -text $n
		}
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
			if {$p==0} {
				.tv_links move g$gid {} end
			} else {
				.tv_links move g$gid g$p end
			}
		}
	}
	destroy .win_grp_mod
}
proc try_modify {} {
	# Modify a row if there is a selection
	set rownumbers [.tv_links selection]
	if {[llength $rownumbers] > 0} {
		foreach r $rownumbers {
			if {[string index $r 0]!={g}} {
				modify_row $r
				tkwait window .win_row_mod
			} else {
				modify_grp [string range $r 1 end]
				tkwait window .win_grp_mod
			}
		}
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
			foreach r $rownumbers {
				if {[string index $r 0]!={g}} {
					rm_data $r
					.tv_links delete $r
				}
			}
		}
	}
}
proc g_grp_root_onoff {w} {
	global rootg_cb
	if {$rootg_cb==1} {
		set m select
		set s disabled
	} else {
		set m deselect
		set s normal
		if {[$w.grp.cb get]=={}} {
			if {[dict size $DBConn::groups]>0} {
				$w.grp.cb current 0
			}
		}
	}
	$w.grp.root $m
	$w.grp.cb configure -state $s
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
	for {set i 2} {$i<7} {incr i} {
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
bind .tv_links <c> {
	copy_url
}
