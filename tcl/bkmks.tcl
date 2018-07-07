#!/usr/bin/env tclsh8.6
package require Tk
package require sqlite3
menu .men
. configure -menu .men
menu .men.mfile
.men.mfile add command -label New -command init_db -underline 0
.men.mfile add command -label Open -command open_db -underline 0
.men.mfile add separator
.men.mfile add command -label Save -command save_db -state disabled -underline 0
.men.mfile add command -label Close -command close_db -state disabled -underline 0
.men.mfile add command -label Import -command import_txt -state disabled -underline 0

.men.mfile add command -label Quit -command exit_prog -underline 0
.men add cascade -label File -menu .men.mfile
.men add command -label {Add Link} -command {modify_row -1} -state disabled
.men add command -label Modify -command try_modify -state disabled
.men add command -label Delete -command try_delete -state disabled
.men add command -label {Copy URL} -command copy_url -state disabled -underline 0
pack [label .statusbar -text Idle -anchor w] -side bottom -fill x
pack [ttk::treeview .tv_links -columns {title url mtime} -yscrollcommand {.tv_links_sb set}] -side left -fill both -expand 1
pack [scrollbar .tv_links_sb -command {.tv_links yview}] -side left -fill y
set i 0
foreach {c l w } [list #0 # 32 title Name 128 url URL 256 mtime {Time Modified} 128] {
	# this loop adjusts the treeview's columns
	.tv_links heading $c -text $l -anchor w -command "reorder_rows $i"
	.tv_links column $c -minwidth 16 -width $w
	incr i
}
namespace eval DB {
	# Simply stores whether a DB is open or not. ATM.
	variable is_open 0 order 0 col_n [dict create 0 rowid 1 title 2 url 3 mtime]
}
proc try_modify {} {
	# Modify a row if there is a selection
	set rownumbers [.tv_links selection]
	if {[llength $rownumbers] > 0} {
		foreach r $rownumbers {
			modify_row $r
			tkwait window .win_row_mod
		}
	} else {
		tk_messageBox -type ok -icon error -message {Please select link to modify.}
	}
}
proc try_delete {} {
	# Delete a row if one is selected
	# Deletes from db first, then from treeview.
	# Note, DB::is_open need not be checked
	# TODO: add warning prompt on multiple?
	set rownumbers [.tv_links selection]
	if {[llength $rownumbers] > 0} {
		foreach r $rownumbers {
			conn eval {DELETE from data WHERE rowid=:r LIMIT 1}
			.tv_links delete $r
		}
	}
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
	# (if clipboard length >0)
	if {[string length [set s [clipboard get]]]>0} {
		$e delete 0 end
		$e insert 0 $s
	}
}
proc add_row {t u rownumber} {
	set m [clock format [clock seconds] -format {%D - %R}]
	if {$rownumber == -1} {
		set rownumber [conn eval {INSERT into data (gid,key,value,mtime) VALUES(NULL,:t,:u,:m);
			SELECT last_insert_rowid();}]
		.tv_links insert {} end -id $rownumber -text $rownumber -value [list $t $u $m]
	} else {
		conn eval {UPDATE data SET key=:t, value=:u, mtime=:m WHERE rowid=:rownumber}
		.tv_links item $rownumber -value [list $t $u $m]
	}
}
proc save_row {rownumber} {
	# Takes properties from 'win_row_mod' window and
	# saves them to both database & treeview with
	# current time. A rownumber of -1 implies a new
	# row. This proc also closes 'win_row_mod'.
	set t [.win_row_mod.t.e get]
	set u [.win_row_mod.u.e get]
	#set m [clock format [clock seconds] -format {%D - %R}]
	add_row $t $u $rownumber
	#if {$rownumber == -1} {
	#	set rownumber [conn eval {INSERT into data (gid,key,value,mtime) VALUES(NULL,:t,:u,:m);
	#		SELECT last_insert_rowid();}]
	#	.tv_links insert {} end -id $rownumber -text $rownumber -value [list $t $u $m]
	#} else {
	#	conn eval {UPDATE data SET key=:t, value=:u, mtime=:m WHERE rowid=:rownumber}
	#	.tv_links item $rownumber -value [list $t $u $m]
	#}
	destroy .win_row_mod
}
proc modify_row {rownumber} {
	# Opens a window for entering/modifying properties of
	# a row. Only allows edit of title & url. This proc takes
	# values from the treeview only, which should be okay since
	# modification of the treeview is limited.
	if {!$DB::is_open} {
		tk_messageBox -icon error -type ok -message {Please open a database first.}
		return
	}
	toplevel .win_row_mod
	grab set .win_row_mod
	wm attributes .win_row_mod -topmost 1
	wm title .win_row_mod {Modify link..}
	wm resizable .win_row_mod 0 0
	foreach {p n} [list t Title u URL] {
		set w .win_row_mod.$p
		pack [labelframe $w -text $n]
		pack [entry $w.e -width 50] -side left
		foreach {l cc} [list a sel_all c copy d del_sel r del_paste] {
			pack [button $w.b$l -text $l -command "entry_$cc $w.e" -takefocus 0] -side left
		}
	}
	set srn [expr {($rownumber>0) ? $rownumber : {NEW}}]
	pack [label .win_row_mod.l -text "LINK #: $srn"]
	pack [frame .win_row_mod.buttons] -side bottom
	pack [button .win_row_mod.buttons.save -text Save -command "save_row $rownumber"] -side left
	pack [button .win_row_mod.buttons.cancel -text Cancel -command {destroy .win_row_mod}] -side left
	if {$rownumber != -1} {
		lassign [.tv_links item $rownumber -values] t u
		.win_row_mod.t.e insert 0 $t
		.win_row_mod.u.e insert 0 $u
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
proc init_db {} {
	# Creates a database file, if one is not already open.
	# The dialog asks whether to replace if the file already
	# exists and it WILL deletedelete that file. Warnings were given.
	# A default table is created in the new file and a transaction
	# is started.
	if {$DB::is_open} {
		set saved [tk_messageBox -icon question -message {DB Already open. Proceed?} -type yesno -title Prompt]
		if {[string equal $saved no]} {
			return
		}
		close_db
	}
	set file_name [tk_getSaveFile -defaultextension .db -filetypes {{{Bookmarks DB} .db}} -title {Save database as..}]
	if {[string length $file_name] == 0} { return }
	if {[file exists $file_name]} { file delete $file_name }
	sqlite3 conn $file_name
	conn eval {
		CREATE TABLE groups(rowid INTEGER primary key,name TEXT);
		CREATE TABLE rel(rowid INTEGER primary key, gid INTEGER,pid INTEGER,depth INTEGER);
		CREATE TABLE data(gid INTEGER,key TEXT,value TEXT,mtime TEXT);
		BEGIN TRANSACTION;
	}
	set DB::is_open 1
	menu_is_open
}
proc load_rows {} {
	# Adds rows to treeview.
	set order [dict get $DB::col_n $DB::order]
	conn eval "SELECT rowid,key,value,mtime from data ORDER BY $order" {
		.tv_links insert {} end -id $rowid -text $rowid -values [list $key $value $mtime]
	}
}
proc open_db {} {
	# Prompt user to choose which database file to open. Given one,
	# will read the rows from the 'bookmarks' table into the treeview.
	# ATM no checks are made whether this table exists nor if the schema
	# matches.
	if {$DB::is_open} {
		set closeit [tk_messageBox -icon question -message {Close current DB?} -type yesno -title Prompt]
		if {[string equal $closeit no]} {
			tk_messageBox -icon error -type ok -message {Close current file first.}
			return
		}
	}
	close_db
	set s [tk_getOpenFile -defaultextension .db -filetypes {{{Bookmarks DB} .db}} -title {Load Bookmarks}]
	if {[string length $s] > 0} {
		wm title . "bkmks - $s"
		set_status "Opened DB $s"
		sqlite3 conn $s
		conn eval {BEGIN TRANSACTION;}
		set DB::is_open 1
		load_rows
		menu_is_open
	}
	puts "DB status: $DB::is_open"
}
proc close_db {} {
	# Closes a database if one is open. Does not commit changes.
	# Also clears treeview of rows from last file.
	if {$DB::is_open} {
		set saveit [tk_messageBox -icon question -message {Save before closing?} -type yesno -title Prompt]
		if {[string equal $saveit yes]} {
			save_db
		}
		conn eval {
			PRAGMA optimize;
		}
		conn close
		set DB::is_open 0
		.tv_links delete [.tv_links children {}]
		menu_is_open
		set_status {Closed DB}
	}
}
proc exit_prog {} {
	# Closes the database and then the program.
	if {[string equal [tk_messageBox -icon question -message {Quit program?} -type yesno -title Prompt] no]} { return }
	close_db
	destroy .
}
proc save_db {} {
	# Commits changes to database with end and begin.
	conn eval {END TRANSACTION;BEGIN TRANSACTION;}
	set_status [format {Saved at %s} [clock format [clock seconds] -format {%D - %R}]]
}
proc menu_is_open {} {
	# Toggles menu options based on file state.
	set nstate [expr {($DB::is_open) ? {normal} : {disabled}}]
	for {set i 2} {$i<6} {incr i} {
		.men entryconfigure $i -state $nstate
	}
	.men.mfile entryconfigure 4 -state $nstate
	.men.mfile entryconfigure 5 -state $nstate
	.men.mfile entryconfigure 6 -state $nstate
}

proc reorder_rows {bycol} {
	# Reorders rows by column. The reordering is only done visually
	# meaning that no reads from DB will occur. A column is reordered
	# in reverse if it was the previous order. See variable DB::order.
	set rs [list]
	foreach r [.tv_links children {}] {
		lappend rs [concat $r [.tv_links item $r -values]]
		.tv_links detach $r
	}
	set reverse [expr {($bycol==$DB::order) ? {decreasing} : {increasing}}]
	set DB::order $bycol
	foreach i [lsort -indices -index $bycol -$reverse $rs] {
		set r [lindex [lindex $rs $i] 0]
		.tv_links move $r {} end
	}
	set_status "Reordered by column $bycol in $reverse order"
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
		set_status "Copied $url"
	}
}
proc set_status {s} {
	.statusbar configure -text $s
}
proc import_txt {} {
	set myfile [tk_getOpenFile -defaultextension .txt -filetypes {{{Bookmarks TXT} .txt}} -title {Load TXT}]
	if {[string length $myfile]==0} return
	set fp [open $myfile r]
	while {[gets $fp L]>=0} {
		#set L2 [split $L "|"]
		#if {[llength $L2]==2} {
		#	add_row {*}$L2 -1
		#} else {
		#	add_row [concat [lrange $L2 0 end-1]] [lindex $L2 end] -1
		#}
		set d [string last "|" $L]
		add_row [string range $L 0 $d-1] [string range $L $d+1 end] -1
	}
}
bind . <Control-n> {
	init_db
}
bind . <Control-c> {
	close_db
}
bind . <Control-s> {
	save_db
}
bind . <Control-o> {
	open_db
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
