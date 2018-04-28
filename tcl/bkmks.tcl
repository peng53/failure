#!/usr/bin/env tclsh8.6
package require Tk
package require sqlite3
menu .men
. configure -menu .men
.men add command -label {New DB} -command init_db
.men add command -label Open -command open_db
.men add command -label Save -command save_db
.men add command -label Close -command close_db
.men add command -label Exit -command exit_prog
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
		conn eval {INSERT into bookmarks VALUES(NULL,:t,:u,:u)}
		set rownumber [conn eval {select last_insert_rowid();}]
		# note : need sql for get id (which is also text)
		.tv_links insert {} end -id $rownumber -text $rownumber -value [list $t $u ?NEWDATE?]
	} else {
		conn eval {UPDATE bookmarks SET title=:t, url=:u, mtime=:u WHERE rowid=:rownumber}
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
namespace eval DB {
	variable is_open 0
}
proc init_db {} {
	if {$DB::is_open} { return }
	set file_name [tk_getSaveFile -defaultextension .db -filetypes {{{Bookmarks DB} .db}} -title {Save database as..}]
	if {[string length $file_name] == 0} { return }
	if {[file exists $file_name]} { file delete $file_name }
	sqlite3 conn $file_name
	conn eval {CREATE TABLE bookmarks(rowid INTEGER PRIMARY KEY autoincrement,title TEXT,url TEXT,mtime TEXT); BEGIN TRANSACTION;}
	set DB::is_open 1
}
proc open_db {} {
	if {$DB::is_open} {
		tk_messageBox -icon error -type ok -message {Close current file first.}
	} else {
		set s [tk_getOpenFile -defaultextension .db -filetypes {{{Bookmarks DB} .db}} -title {Load Bookmarks}]
		if {[string length $s] > 0} {
			puts "Opened DB $s"
			sqlite3 conn $s
			conn eval {BEGIN TRANSACTION;}
			set DB::is_open 1
			#set rs [list]
			conn eval {SELECT rowid,title,url,mtime from bookmarks ORDER BY rowid} {
				#lappend rs [list $rowid $title $url $mtime]
				.tv_links insert {} end -id $rowid -text $rowid -values [list $title $url $mtime]
			}
			#foreach r $rs {
			#	puts $r
			#}
		}
	}
	puts "DB status: $DB::is_open"
}
proc close_db {} {
	if {$DB::is_open} {
		puts {Closing DB..}
		conn close
		set DB::is_open 0
		.tv_links delete [.tv_links children {}]
	}
}
proc exit_prog {} {
	close_db
	destroy .
}
proc save_db {} {
	conn eval {END TRANSACTION;BEGIN TRANSACTION;}
}
#.tv_links insert {} end -id 0 -text 0 -values [list {Some Site} http://123fakesite.456 18-04-28]
#.tv_links insert {} end -id 1 -text 1 -values [list {Some Site} http://123fakesite.456 18-04-28]

