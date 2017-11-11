# tcalev_sql.tcl
package require sqlite3

namespace eval EventStor {
	namespace eval v {
		variable is_open 0
		variable col_names [list end_date event_name desc_more rowid start_date {strftime('%Y',start_date)} {strftime('%m',start_date)}]
		variable where_operators [list < <= = >= >]
	}
	proc open_db {file_name} {
		# Open file_name as sqlite3 database. Returns 1/0 for success/fail.
		if {$v::is_open} { return 0 }
		set v::is_open 1
		sqlite3 conn $file_name
		return 1
	}
	proc open_instead {file_name} {
		# Replace current db with file_name.
		set v::is_open 1
		conn restore $file_name
	}
	proc copy_to {file_name} {
		# Copies current db to a new file
		if {!$v::is_open} { return 0 }
		conn backup $file_name
		return 1
	}
	proc save_changes {} {
		# Saves to changes to db
		if {!$v::is_open} { return 0 }
		conn eval {commit}
		return 1
	}
	proc add_row {start_date end_date event_name {desc_more {}}} {
		# Add row to event. Returns 0 if database isn't open.
		if {!$v::is_open} { return 0 }
		conn eval {INSERT into events VALUES(null,:start_date,:end_date,:event_name,:desc_more)}
		return 1
	}
	proc lastrow_added {} {
		# Returns the rowid of the last row inserted.
		# Returns -1 if db not open.
		if {!$v::is_open} { return -1 }
		return [conn eval {SELECT last_insert_rowid()}]
	}
	proc update_row {rowid start_date end_date event_name desc_more} {
		# Update row in event. Returns 0 if database isn't open.
		if {!$v::is_open} { return 0 }
		conn eval {UPDATE events SET start_date=:start_date, end_date=:end_date, event_name=:event_name, desc_more=:desc_more WHERE rowid=:rowid}
		return 1
	}
	proc delete_row {rowid} {
		# Deletes row in event. Returns 0 if database isn't open
		# and -1 if rowid is zero-length string.
		if {!$v::is_open} { return 0 }
		if {[string length $rowid]==0} { return -1 }
		conn eval {DELETE from events WHERE rowid=:rowid}
		return 1
	}
	proc wheres {vs} {
		# Checks where cols & operators for validity.
		set ws [list]
		foreach {COL OP X} $vs {
			#puts "$COL $OP $X"
			if {$COL in $v::col_names && $OP in $v::where_operators} {
				lappend ws "$COL$OP$X"
			}
		}
		if {[llength $ws]>0} {
			return "WHERE [join $ws { AND }]"
		}
	}
	proc orders {vs} {
		# Checks order cols for validity.
		set os [list]
		foreach {COL} $vs {
			if {$COL in $v::col_names} {
				lappend os $COL
			}
		}
		if {[llength $os]>0} {
			return "ORDER BY [join $os ,]"
		}
	}
	proc second_date {year {mth 1} {day 1} {hr 0} {mins 0}} {
		# Returns seconds since unix epoch. (shortcut)
		return [clock scan [format %02d/%02d/%04d-%02d:%02d $mth $day $year $hr $mins] -format %D-%H:%M]
	}
	proc exp_date {op year {mth 0}} {
		# Returns where-value-list for a year (and month)
		return [list start_date $op [second_date $year [expr {($mth==0 ? 1:$mth)}]]]
	}
	proc get_all_rows {{ws {}} {os {}}} {
		# Yields single row at a time from open database.
		# Returns 0 if not open.
		if {!$v::is_open} { return 0 }
		set rs [list]
		conn eval "SELECT * from events [wheres $ws] [orders $os]" {
			lappend rs [list $rowid $start_date $end_date $event_name $desc_more]
		}
		return $rs
	}
	proc get_base_rows {{ws {}} {os {}}} {
		# Returns brief rows.
		# Returns 0 if not open.
		# Only basic stuff
		if {!$v::is_open} { return 0 }
		puts $ws
		set rs [list]
		conn eval "SELECT rowid,date(start_date,'unixepoch','localtime') as date,event_name from events [wheres $ws] [orders $os]" {
			lappend rs [list $rowid $date $event_name]
		}
		return $rs
	}
	proc ps_get_basic {} {
		# Returns brief rows without user input
		if {!$v::is_open} { return 0 }
		set rs [list]
		conn eval {SELECT rowid,date(start_date,'unixepoch','localtime') as date, event_name from events ORDER by start_date} {
			lappend rs [list $date $event_name $rowid]
		}
		return $rs
	}
	proc ps_get_event {name} {
		# Returns row(s) with exact event_name
		if {!$v::is_open} { return 0 }
		set rs [list]
		conn eval {SELECT rowid,date(start_date,'unixepoch','localtime') as date,event_name from events WHERE lower(event_name) LIKE :name ORDER by start_date} {
			lappend rs [list $date $event_name $rowid]
		}
		return $rs
	}
	proc ps_get_more {rowid} {
		# Returns all cols for row with rowid.
		if {!$v::is_open} { return 0 }
		conn eval {SELECT datetime(start_date,'unixepoch','localtime') as date1, datetime(end_date,'unixepoch','localtime') as date2, event_name, desc_more from events WHERE rowid=:rowid} {
			return [list $date1 $date2 $event_name $desc_more]
		}
		return -1
	}
	proc ps_get_date_range {d1 d2} {
		# Returns all rows with start_date between d1 & d2 including endpts
		# Where d1 & d2 are [list year {month {day}}]
		if {!$v::is_open} { return 0 }
		set rs [list]
		set d1s [second_date {*}$d1]
		set d2s [second_date {*}$d2]
		conn eval {SELECT rowid,date(start_date,'unixepoch','localtime') as date, event_name from events WHERE start_date>=:d1s AND start_date<=:d2s ORDER by start_date} {
			lappend rs [list $date $event_name $rowid]
		}
		return $rs
	}
	proc get_a_cal {year {mth 0}} {
		# Yields rows for one calendar {page}.
		if {!$v::is_open} { return 0 }
		if {$mth==0} {
			return [get_base_rows [list [exp_date >= $year] [exp_date < [expr $year+1]]]]
		}
		#return [get_base_rows [list [exp_date >= $year $mth] [exp_date < [expr $year+1]]]]
	}
	proc build_db {file_name} {
		# Builds a database if one isn't already open.
		# Returns -1 if file already exists.
		# Does not check schema ATM
		if {$v::is_open} { return 0 }
		set v::is_open 1
		if {[file exists $file_name]} { return -1 }
		sqlite3 conn $file_name
		conn transaction {
			conn eval {CREATE TABLE events(rowid integer primary key autoincrement, start_date integer, end_date integer, event_name text, desc_more text)}
		}
		return 1
	}
	proc close_db {} {
		# Closes the open database.
		if {!$v::is_open} { return 0 }
		set v::is_open 0
		conn close
		return 1
	}
	proc xth_dow {mth year d xth} {
		# Where 0<=d<=6 for sunday-saturday.
		# Where xth is 0-indexed
		# Ex: Want to find 3rd Monday of Sept 2017
		# Use: xth_dow 9 2017 1 3-1
		set f [clock format [clock scan $mth/01/$year -format %D] -format %w]
		return [expr {$d-$f+7*$xth+($f>$d ? 8:1)}]
	}
	proc event_days {s1 s2} {
		# Returns all days between unixepoch s1 s2 with events
		if {!$v::is_open} { return 0 }
		set rs [list]
		conn eval {SELECT distinct strftime('%d',start_date,'unixepoch') as D from events WHERE start_date>=:s1 AND start_date<=:s2} {
			lappend rs $D
		}
		return $rs
	}
	proc event_day {mth d d2 year} {
		# Returns all days in mth/year with events
		return [event_days [second_date $year $mth $d] [second_date $year $mth $d2]]
	}
	proc holidays_us {year} {
		# Adds US holidays for year.
		if {!$v::is_open} { return 0 }
		conn transaction {
			foreach {mth day name} {12 25 Christmas 7 4 {Independence Day} 10 31 Halloween 2 14 {Valentines Day} 3 17 {St Patrick's Day} 1 1 {New Years Eve} 1 15 {Martin Luther King Jr Day} 11 11 {Veterans Day}} {
				set date [second_date $year $mth $day]
				add_row $date [expr {$date+86399}] $name
			}
			foreach {mth xth day name} {11 3 4 Thanksgiving 5 1 0 {Mothers Day} 6 2 0 {Fathers Day} 9 0 1 {Labor Day} 10 1 1 {Columbus Day}} {
				set date [second_date $year $mth [xth_dow $mth $year $day $xth]]
				add_row $date [expr {$date+86399}] $name
			}
			set date [expr {[second_date $year 6 [xth_dow 6 $year 1 0]]-604800}]
			# I find first Monday of the month after and go back 7 days.
			add_row $date [expr {$date+86399}] {Memorial Day}
		}
		return 1
	}
	proc test {} {
		puts {Building DB in memory}
		if {![build_db :memory:]} {
			puts {build_db failed}
			return 1
		}
		puts {Inserting test rows}
		set n 0
		foreach {year mth day} {2017 1 23 2018 3 22 2017 1 25 2017 2 11 } {
			set date [second_date $year $mth $day]
			add_row $date $date $n {;DROP TABLE events;}
			incr n
		}
		puts {Getting rows}
		if {[set rs [get_base_rows]]==0} {
			puts {get_base_rows failed}
			return 1
		}
		foreach r $rs { puts [join $r ,] }

		puts {Getting rows where event_name is > 1 ordered by start_date}
		foreach r [get_base_rows {event_name >= 1} {start_date}] { puts [join $r ,] }

		puts {Getting rows where 0<rowid<4 ordered by rowid}
		foreach r [get_base_rows {rowid > 0 rowid < 4} {rowid}] { puts [join $r ,] }

		puts {Getting rows where desc_more = ;DROP TABLE events;}
		foreach r [get_base_rows {desc_more = "';DROP TABLE events;'"}] { puts [join $r ,] }

		puts {Confirming table & row still exists with get_all_rows}
		foreach r [get_all_rows] { puts [join $r ,] }

		puts {Adding Holidays for year 2017}
		holidays_us 2017
		puts {Gettings rows ordered by event_name then start_date}
		foreach r [get_base_rows {} {event_name start_date}] { puts [join $r ,] }

		puts {Getting rows with start_date in Sept-Dec 2017}
		foreach r [get_base_rows [list start_date >= [second_date 2017 9] start_date < [second_date 2018]] {start_date}] { puts [join $r ,] }
		puts {AGAIN!!}
		foreach r [ps_get_date_range [list 2017 9] [list 2017 12 31]] { puts [join $r ,] }

		puts {Getting 2017}
		foreach r [get_a_cal 2017] { puts [join $r ,] }
		holidays_us 2018
		puts {All events with name 'Memorial Day'}
		foreach r [ps_get_event {Memorial Day}] { puts [join $r ,] }
		set i [lastrow_added]
		puts $i
		puts [ps_get_more $i]
		puts [get_base_rows]
		puts {Closing DB}
		if {![close_db]} {
			puts {close_db failed}
			return 1
		}
		puts {Successful.}
	}
}
if {[string match *tcalev_sql.tcl $argv0]} {
	EventStor::test
}
