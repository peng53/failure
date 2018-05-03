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
.men.mfile add command -label Quit -command exit_prog -underline 0
.men add cascade -label File -menu .men.mfile
.men add command -label {Add Link} -command {modify_row -1} -state disabled
.men add command -label Modify -command try_modify -state disabled
.men add command -label Delete -command try_delete -state disabled
pack [ttk::treeview .tv_links -columns {title url mtime} -yscrollcommand {.tv_links_sb set}] -side left -fill both -expand 1
pack [scrollbar .tv_links_sb -command {.tv_links yview}] -side left -fill y
foreach {c l w} [list #0 # 32 title Name 128 url URL 256 mtime {Time Modified} 128] {
	# this loop adjusts the treeview's columns
	.tv_links heading $c -text $l -anchor w
	.tv_links column $c -minwidth 16 -width $w
}
bind .tv_links <Return> {
	try_modify
}
bind .tv_links <Delete> {
	try_delete
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
			conn eval {DELETE from bookmarks WHERE rowid=:r LIMIT 1}
			.tv_links delete $r
		}
	}
}
proc save_row {rownumber} {
	# Takes properties from 'win_row_mod' window and
	# saves them to both database & treeview with
	# current time. A rownumber of -1 implies a new
	# row. This proc also closes 'win_row_mod'.
	puts $rownumber
	set t [.win_row_mod.t.e get]
	set u [.win_row_mod.u.e get]
	set m [clock format [clock seconds] -format {%D - %R}]
	puts $t
	puts $u
	if {$rownumber == -1} {
		conn eval {INSERT into bookmarks VALUES(NULL,:t,:u,:m)}
		set rownumber [conn eval {select last_insert_rowid();}]
		# note : need sql for get id (which is also text)
		.tv_links insert {} end -id $rownumber -text $rownumber -value [list $t $u $m]
	} else {
		conn eval {UPDATE bookmarks SET title=:t, url=:u, mtime=:m WHERE rowid=:rownumber}
		.tv_links item $rownumber -value [list $t $u $m]
	}
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
	bind .win_row_mod <Escape> {
		destroy .win_row_mod
	}
	wm title .win_row_mod {Modify link..}
	wm resizable .win_row_mod 0 0
	pack [labelframe .win_row_mod.t -text Title]
	pack [entry .win_row_mod.t.e -width 50]
	pack [labelframe .win_row_mod.u -text URL]
	pack [entry .win_row_mod.u.e -width 50]
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
	}
}
namespace eval DB {
	# Simply stores whether a DB is open or not. ATM.
	variable is_open 0
	variable order rowid
}
proc init_db {} {
	# Creates a database file, if one is not already open.
	# The dialog asks whether to replace if the file already
	# exists and it WILL delete that file. Warnings were given.
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
	conn eval {CREATE TABLE bookmarks(rowid INTEGER PRIMARY KEY autoincrement,title TEXT,url TEXT,mtime TEXT); BEGIN TRANSACTION;}
	set DB::is_open 1
	menu_is_open
}
proc load_rows {} {
	# Adds rows to treeview.
	conn eval {SELECT rowid,title,url,mtime from bookmarks ORDER BY rowid} {
		.tv_links insert {} end -id $rowid -text $rowid -values [list $title $url $mtime]
	}
}
proc open_db {} {
	# Prompt user to choose which database file to open. Given one,
	# will read the rows from the 'bookmarks' table into the treeview.
	# ATM no checks are made whether this table exists nor if the schema
	# matches.
	if {$DB::is_open} {
		tk_messageBox -icon error -type ok -message {Close current file first.}
	} else {
		set s [tk_getOpenFile -defaultextension .db -filetypes {{{Bookmarks DB} .db}} -title {Load Bookmarks}]
		if {[string length $s] > 0} {
			puts "Opened DB $s"
			sqlite3 conn $s
			conn eval {BEGIN TRANSACTION;}
			set DB::is_open 1
			load_rows
			menu_is_open
		}
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
		puts {Closing DB..}
		conn close
		set DB::is_open 0
		.tv_links delete [.tv_links children {}]
		menu_is_open
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
	puts {Saving DB..}
	conn eval {END TRANSACTION;BEGIN TRANSACTION;}
}
proc menu_is_open {} {
	set nstate [expr {($DB::is_open) ? "normal" : "disabled"}]
	for {set i 2} {$i<5} {incr i} {
		.men entryconfigure $i -state $nstate
	}
	.men.mfile entryconfigure 4 -state $nstate
	.men.mfile entryconfigure 5 -state $nstate
}
#.tv_links insert {} end -id 0 -text 0 -values [list {Some Site} http://123fakesite.456 18-04-28]
#.tv_links insert {} end -id 1 -text 1 -values [list {Some Site} http://123fakesite.456 18-04-28]
proc reorder_rows {bycol} {
	set rs [list]
	foreach r [.tv_links children {}] {
		lappend rs [concat $r [.tv_links item $r -values]]
	}
	puts [lsort -index $bycol $rs]
}
bind . <Control-n> {
	# TODO: add yes/no prompt
	init_db
}
bind . <Control-c> {
	# TODO: add yes/no prompt
	close_db
}
bind . <Control-s> {
	# TODO: add yes/no prompt
	save_db
}
bind . <Control-o> {
	# TODO: add warning if DB::is_open
	open_db
}
bind . <Control-q> {
	exit_prog
}
bind . <Control-z> {
	reorder_rows t

}
