# tcalev_sql.tcl
package require sqlite3

namespace eval EventStor {
	namespace eval v {
		variable is_open 0
	}
	proc open_db {file_name} {
		# Open file_name as sqlite3 database. Returns 1/0 for success/fail.
		if {$v::is_open} { return 0 }
		sqlite3 conn $file_name
		return 1
	}
	proc add_row {start_date, end_date, event_name, desc_more} {
		# Add row to event. Returns 0 if database isn't open.
		if {$v::is_open} { return 0 }
		conn eval {INSERT into events VALUES(null,$start_date,$end_date,$event_name,$desc_more)}
		return 1
	}
	proc update_row {rowid, start_date, end_date, event_name, desc_more} {
		# Update row in event. Returns 0 if database isn't open.
		if {$v::is_open} { return 0 }
		conn eval {UPDATE events SET start_date=$start_date, end_date=$end_date, event_name=$event_name, desc_more=$desc_more WHERE rowid=$rowid}
		return 1
	}
	proc delete_row {rowid} {
		# Deletes row in event. Returns 0 if database isn't open
		# and -1 if rowid is zero-length string.
		if {$v::is_open} { return 0 }
		if {[string length $rowid]==0} { return -1 }
		conn eval {DELETE from events WHERE rowid=$rowid}
		return 1
	}
	proc get_all_rows {} {
		# Yields single row at a time from open database.
		# Returns 0 if not open. For use as coroutine.
		if {$v::is_open} { return 0 }
		yield 1
		conn eval {SELECT * from events} {
			yield [list $rowid $start_date $end_date $event_name $desc_more]
		}
	}
}