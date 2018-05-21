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
.men add command -label {Add Link} -command {modify_row -1} -state disabled
.men add command -label Modify -command try_modify -state disabled
.men add command -label Delete -command try_delete -state disabled
.men add command -label {Copy URL} -command copy_url -state disabled -underline 0
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
		.tv_links insert {} end -id $gid -text [dict get $DBConn::groups $gid]
		lappend groups $gid
	}
	# Second, other groups
	foreach g $groups {
		puts $g
		conn eval {
			SELECT gid,pid FROM rel WHERE
			gid IN (SELECT gid FROM rel WHERE pid=:g AND gid!=pid) and depth=1 and pid IS NOT NULL;
		} {
			puts "$gid $pid"
			.tv_links insert $pid end -id $gid -text [dict get $DBConn::groups $gid]
			
		}
	}
	# Lastly, the data
	conn eval {
		SELECT rowid,ifnull(gid,0) as gid,key,value,mtime FROM data;
	} {
		if {$gid==0} {
			set gid {}
		}
		.tv_links insert $gid end -text $rowid -values [list $key $value $mtime]
	}
}
testing_db
load_rows
