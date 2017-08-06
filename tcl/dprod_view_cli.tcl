#!/usr/bin/tclsh8.6
package require sqlite3
array set SELS {
	0 {SELECT * FROM prod_records}
	" u" {SELECT * from prod_records ORDER by uid}
	" c" {SELECT * from prod_records ORDER by code}
	" s" {SELECT * from prod_records ORDER by start_time}
	" e" {SELECT * from prod_records ORDER by end_time}
	" d" {SELECT * from prod_records ORDER by desc}
	u {SELECT * from prod_records WHERE uid=?}
	c {SELECT * from prod_records WHERE code=?}
	b {SELECT * from prod_records WHERE date(end_time)<?}
	a {SELECT * from prod_records WHERE date(start_time)>?}
	d {SELECT * from prod_records WHERE date(start_time)=?}
}
set ORD [list "" " u" " c" " s" " e" " d"]
set EXT [list u c]
set DTE [list b a d]

proc sqlmat {db} {
	array set ccols {
		uid {text} start_time {datetime} end_time {datetime} code {text} desc {text}
	}
	db eval {pragma table_info(prod_records)} {
		try {
			if {$ccols($name) ne $type} {
				return false
			}
		} on error {} {
			return false
		}
	}
	return true
}
proc print_rows {rows off c} {
	puts [string repeat _ 82]
	puts {RN  /  UID   /   Start Time     /    End Time      / Code  /  Desc}
	puts [string repeat - 82]
	set n 0
	set i $off
	set l [expr min($c+$off,[llength $rows])]
	while {$i<$l} {
		puts [format "%2d | %6s | %16s | %16s | %5s | %-22s" $n {*}[lindex $rows $i]]
		#puts [format "%2d | %74s | %16s" $n {*}[lindex $rows $i]]
		incr i
		incr n
	}

	#for n,i in enumerate(xrange(s,min(self.ic+s,len(rows)))):
		#print "%2d | %6s | %16s | %16s | %5s | %-22s" %(n,rows[i][0],rows[i][1],rows[i][2],rows[i][3],rows[i][4])
	#for _ in xrange(len(rows),s+self.ic):
		#print "   | %6s | %16s | %16s | %5s |" %('','','','')
	#print '='*59, "%21s %d" %("Page",s//self.ic)
}
proc draw_screen {results r_i r_c notes n_i n_c} {
	puts {Productivity Viewer}
	puts {Results table from lookup}
	print_rows results r_i r_c
	puts {Noted records}
	print_rows notes n_i n_c
	puts {Command Bar}
	puts {(L)oad (Q)uit (l)ookup (a)dd-to-notes (d)elete-from-notes}
	puts {(S)ort (C)lear (1)st-page (n)ext-page (p)rev-page (s)ummarize}
	puts {(e)mployee (f)ilter}
}
proc open_db {} {
	puts {Opening DB for viewing.}
	puts {Please enter file name.}
	puts {File name:}
	if {[gets stdin s]>0} {
		if {[file exists $s]} {
			sqlite3 db $s
			if {[sqlmat db]} {
				puts [format "'%s' loaded for viewing." $s]
				return true
			} else {
				puts {Database schema does not match intended.}
				puts {Closing file.}
			}
		}
	}
	return false
}

set db_open [open_db]
if {$db_open} {
	set l [list]
	db eval $SELS(0) {
		lappend l "$uid $start_time $end_time $code $desc"
	}
	#foreach r $l {
	#	puts $r
	#}
	print_rows $l 0 10
}
