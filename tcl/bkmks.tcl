package require Tk
menu .men
. configure -menu .men
.men add command -label Open
.men add command -label Save
.men add command -label Close -command {destroy .}
.men add command -label {Add Link} -command {modify_row -1}
.men add command -label Modify -command {mod_if_sel [.tv_links selection]}
.men add command -label Delete
pack [ttk::treeview .tv_links -columns {title url mtime}] -side left -fill both -expand 1
pack [scrollbar .tv_links_sb] -side left -fill y
foreach {c l w} [list #0 # 32 title Name 128 url URL 256 mtime {Time Modified} 128] {
	.tv_links heading $c -text $l -anchor w
	.tv_links column $c -minwidth 16 -width $w
}
proc mod_if_sel {rownumber} {
	if {[string length $rownumber] != 0} {
		modify_row $rownumber
	} else {
		tk_messageBox -type ok -icon error -message {Please select link to modify.}
	}
}
proc save_row {rownumber} {
	puts $rownumber
	set t [.win_row_mod.t.e get]
	set u [.win_row_mod.u.e get]
	puts $t
	puts $u
	if {$rownumber == -1} {
		# note : need sql for get id (which is also text)
		.tv_links insert {} end -value [list $t $u ?NEWDATE?]
	} else {
		.tv_links item $rownumber -value [list $t $u ?NEWDATE?]
	}
	destroy .win_row_mod
}
proc modify_row {rownumber} {
	toplevel .win_row_mod
	wm title .win_row_mod {Modify Row}
	wm resizable .win_row_mod 0 0
	pack [labelframe .win_row_mod.t -text Title]
	pack [entry .win_row_mod.t.e]
	pack [labelframe .win_row_mod.u -text URL]
	pack [entry .win_row_mod.u.e]
	pack [frame .win_row_mod.buttons] -side bottom
	pack [button .win_row_mod.buttons.save -text Save -command "save_row $rownumber"] -side left
	pack [button .win_row_mod.buttons.cancel -text Cancel -command {destroy .win_row_mod}] -side left
	if {$rownumber != -1} {
		lassign [.tv_links item $rownumber -values] t u
		.win_row_mod.t.e insert 0 $t
		.win_row_mod.u.e insert 0 $u
	}
}
.tv_links insert {} end -id 0 -text 0 -values [list {Some Site} http://123fakesite.456 18-04-28]
.tv_links insert {} end -id 1 -text 1 -values [list {Some Site} http://123fakesite.456 18-04-28]

