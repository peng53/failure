#!/usr/bin/env tclsh8.6
package require Tk
source hierarch_clo.tcl
menu .men
. configure -menu .men
menu .men.mfile
.men.mfile add command -label New -command init_db -underline 0
.men.mfile add command -label Open -command open_db -underline 0
.men.mfile add separator
.men.mfile add command -label Save -command save_db -state disabled -underline 0
.men.mfile add command -label Close -command close_db -state disabled -underline 0
.men.mfile add command -label Quit -command exit_prog -underline 0
.men add cascade -label File -menu .men.mfile
.men add command -label {Add Group} -command {modify_grp -1}
.men add command -label {Add Link} -command {modify_row -1}

.men add command -label Modify -command try_modify
.men add command -label Delete -command try_delete -state disabled
.men add command -label {Copy URL} -command copy_url -state disabled -underline 0
set rootg_cb 0
pack [label .statusbar -text Idle -anchor w] -side bottom -fill x
pack [ttk::treeview .tv_links -columns {title url mtime} -yscrollcommand {.tv_links_sb set}] -side left -fill both -expand 1
pack [scrollbar .tv_links_sb -command {.tv_links yview}] -side left -fill y

foreach {c l w} [list #0 Groups 128 title Name 128 url URL 256 mtime {Time Modified} 128] {
	# this loop adjusts the treeview's columns
	.tv_links heading $c -text $l -anchor w
	.tv_links column $c -minwidth 16 -width $w
}


proc g_open_db {} {
	# Opens a database and reads it.
	if {$DBConn::is_open} {
		return
	}
 set s [tk_getOpenFile -defaultextension .db -filetypes {{{Bookmarks DB} .db}} -title {Load Bookmarks}]
	if {[string length $s] > 0} {
		open_db_i $s
		load_rows
		puts "Opened DB $s"
		wm title . "bkmks - $s"
		set_status "Opened DB $s"
		menu_is_open
	}
	puts "DB status: $DB::is_open"
}
proc load_rows {} {
	# Loads rows of DB to treeview.
	# First, the root groups
	set groups [list]
	conn eval {
		SELECT gid FROM rel WHERE pid IS NULL;
	} {
		.tv_links insert {} end -id "g$gid" -text "$gid [dict get $DBConn::groups $gid]"
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
			.tv_links insert "g$pid" end -id "g$gid" -text "$gid [dict get $DBConn::groups $gid]"
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
	if {!$DBConn::is_open} {
		tk_messageBox -icon error -type ok -message {Please open a database first.}
		return
	}
	toplevel .win_row_mod
	grab set .win_row_mod
	wm attributes .win_row_mod -topmost 1
	bind .win_row_mod <Escape> {
		destroy .win_row_mod
	}
	wm title .win_row_mod {Modify link..}
	wm resizable .win_row_mod 0 0
	pack [labelframe .win_row_mod.t -text Title]
	pack [entry .win_row_mod.t.e -width 50] -side left
	pack [button .win_row_mod.t.ba -text A -command {entry_sel_all .win_row_mod.t.e}
	] -side left
	pack [button .win_row_mod.t.bc -text C -command {entry_copy .win_row_mod.t.e}
	] -side left
	pack [button .win_row_mod.t.bd -text D -command {entry_del_sel .win_row_mod.t.e}
	] -side left
	pack [button .win_row_mod.t.br -text R -command {entry_del_paste .win_row_mod.t.e}
	] -side left
	pack [labelframe .win_row_mod.u -text URL]
	pack [entry .win_row_mod.u.e -width 50] -side left
	pack [button .win_row_mod.u.ba -text A -command {entry_sel_all .win_row_mod.u.e}
	] -side left
	pack [button .win_row_mod.u.bc -text C -command {entry_copy .win_row_mod.u.e}
] -side left
	pack [button .win_row_mod.u.bd -text D -command {entry_del_sel .win_row_mod.u.e}
	] -side left
	pack [button .win_row_mod.u.br -text R -command {entry_del_paste .win_row_mod.u.e}
	] -side left
	pack [labelframe .win_row_mod.grp -text Group] -fill x
	pack [ttk::combobox .win_row_mod.grp.cb -values [dict values $DBConn::groups] -state readonly] -fill x -expand 1 -side left
	global rootg_cb
	pack [checkbutton .win_row_mod.grp.root -text Root -command {g_grp_root_onoff .win_row_mod} -variable rootg_cb] -side left
	bind .win_row_mod.t.e <Shift-Button-1> {
		.win_row_mod.t.e selection range 0 end
	}
	bind .win_row_mod.u.e <Shift-Button-1> {
		.win_row_mod.u.e selection range 0 end
	}
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
		.tv_links insert $g end -id $rownumber -text $rownumber -value [list $t $u $m]
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
	if {!$DBConn::is_open} {
		return
	}
	toplevel .win_grp_mod
	grab set .win_grp_mod
	wm attributes .win_grp_mod -topmost 1
	bind .win_grp_mod <Escape> {
		destroy %W
	}
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
		# need to set parent and current name
		# parent
		set p [.tv_links parent g$gid]
		if {[string length $p]>0} {
			.win_grp_mod.grp.root deselect
			.win_grp_mod.grp.cb set [dict get $DBConn::groups [string range $p 1 end]]
		} else {
			.win_grp_mod.grp.root select
		}
		# name
		.win_grp_mod.n.e insert 0 [dict get $DBConn::groups $gid]
	}
	g_grp_root_onoff .win_grp_mod
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
			.tv_links insert {} end -id g$gid -text "$gid $n"
		} else {
			.tv_links insert g$p end -id g$gid -text "$gid $n"
		}
	} else {
		# need to check whether the parent is the child.
		if {$gid==$p} {
			# TODO: an error message
			return
		}
		update_grp $gid $n
		.tv_links item g$gid -text "$gid $n"
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
proc g_grp_root_onoff {w} {
	global rootg_cb
	if {$rootg_cb} {
		set m select
		set s disabled
	} else {
		set m deselect
		set s normal
		if {[$w.grp.cb get]=={}} {
			$w.grp.cb current 0
		}
	}
	$w.grp.root $m
	$w.grp.cb configure -state $s
}

testing_db
load_rows
