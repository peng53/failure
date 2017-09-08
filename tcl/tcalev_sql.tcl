# tcalev_sql.tcl
package require sqlite3

namespace eval EventStor {
	namespace eval v {
		variable is_open 0
		variable col_names [list end_date event_name desc_more rowid start_date {strftime('%Y',start_date)} {strftime('%M',start_date)}]
		variable where_operators [list < <= = >= >]
	}
	proc open_db {file_name} {
		# Open file_name as sqlite3 database. Returns 1/0 for success/fail.
		if {$v::is_open} { return 0 }
		set v::is_open 1
		sqlite3 conn $file_name
		return 1
	}
	proc add_row {start_date end_date event_name desc_more} {
		# Add row to event. Returns 0 if database isn't open.
		if {$v::is_open==0} { return 0 }
		conn eval {INSERT into events VALUES(null,$start_date,$end_date,$event_name,$desc_more)}
		return 1
	}
	proc update_row {rowid start_date end_date event_name desc_more} {
		# Update row in event. Returns 0 if database isn't open.
		if {$v::is_open==0} { return 0 }
		conn eval {UPDATE events SET start_date=$start_date, end_date=$end_date, event_name=$event_name, desc_more=$desc_more WHERE rowid=$rowid}
		return 1
	}
	proc delete_row {rowid} {
		# Deletes row in event. Returns 0 if database isn't open
		# and -1 if rowid is zero-length string.
		if {$v::is_open==0} { return 0 }
		if {[string length $rowid]==0} { return -1 }
		conn eval {DELETE from events WHERE rowid=$rowid}
		return 1
	}
	proc get_all_rows {} {
		# Yields single row at a time from open database.
		# Returns 0 if not open. For use as coroutine.
		if {$v::is_open==0} { return 0 }
		yield 1
		conn eval {SELECT * from events} {
			yield [list $rowid $start_date $end_date $event_name $desc_more]
		}
		yield 0
	}
	proc wheres {vs} {
		# Checks where cols & operators for validity.
		set ws [list]
		foreach {COL OP X} $vs {
			if {$COL in $v::col_names && $OP in $v::where_operators} {
				lappend ws "$COL$OP$X"
			}
		}
		if {[llength $ws]>0} {
			return "WHERE [join $ws { AND }]"
		}
	}
	proc second_date {year {mth 01} {day 01}} {
		# Returns seconds since unix epoch. (shortcut)
		return [clock scan [format %02d/%02d/%04d $mth $day $year] -format %D]
	}
	proc exp_date {year {mth 0}} {
		# Returns where-value-list for a year (and month)
		set vs [list {strftime('%Y',start_date)} == $year]
		if {$mth!=0} {
			lappend vs {strftime('%m',start_date)} == $mth
		}
		return vs
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
	proc get_base_rows {{ws {}} {os {}}} {
		# Yields single row at a time from open database.
		# Returns 0 if not open.
		# Only basic stuff
		if {$v::is_open==0} { return 0 }
		set rs [list]
		conn eval "SELECT rowid,datetime(start_date,'unixepoch','localtime') as date,event_name from events [wheres $ws] [orders $os]" {
			lappend rs [list $rowid $date $event_name]
		}
		return $rs
	}
	proc get_month_rows {month year} {
		# Yields rows for one calendar page.
		if {$v::is_open==0} { return 0 }
		yield 1
		conn eval {SELECT rowid,day($start_date),$event_name from events WHERE year(start_date)=$year AND month(start_date)=$month} {
			yield [list $rowid $start_date $event_name]
		}
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
			conn eval {CREATE TABLE events(rowid integer primary key autoincrement, start_date integer, end_date integer, event_name string, desc_more string)}
		}
		return 1
	}
	proc close_db {} {
		# Closes the open database.
		if {$v::is_open==0 } { return 0 }
		set v::is_open 0
		conn close
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
			add_row $date $date $n test
			incr n
		}
		puts {Getting rows}
		if {[set rs [get_base_rows]]==0} {
			puts {get_base_rows failed}
			return 1
		}
		foreach r $rs { puts [join $r] }
		puts {Getting rows where event_name is > 1 ordered by start_date}
		if {[set rs [get_base_rows {event_name >= 1} {start_date}]]==0} {
			puts {get_base_rows failed}
			return 1
		}
		foreach r $rs { puts [join $r] }
		puts {Getting rows where 0<rowid<4 ordered by rowid}
		if {[set rs [get_base_rows {rowid > 0 rowid < 4} {rowid}]]==0} {
			puts {get_base_rows failed}
			return 1
		}
		foreach r $rs { puts [join $r] }
		puts {Closing DB}
		if {![close_db]} {
			puts {close_db failed}
			return 1
		}
		puts {Successful.}
	}
}
EventStor::test