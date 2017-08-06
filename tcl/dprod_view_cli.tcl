#!/usr/bin/tclsh8.6
package require sqlite3
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
	foreach r [lrange $rows $off [expr min($c+$off,[llength $rows])]] {
		puts [format "%2d | %6s | %10s %5s | %10s %5s | %5s | %-22s" $n {*}$r]
		incr n
	}
	while {$n<$c} {
		puts [format "   | %6s | %16s | %16s | %5s |" "" "" "" ""]
		incr n
	}
	puts "[string repeat = 69] [format "%10s %2d" "Page" [expr $off/$c]]"
}
proc draw_screen {results r_i r_c notes n_i n_c} {
	puts {Productivity Viewer}
	puts {Results table from lookup}
	print_rows $results $r_i $r_c
	puts {Noted records}
	print_rows $notes $n_i $n_c
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

proc main {} {
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


	set R [list]
	set r_s 0
	set r_c 10
	set N [list]
	set n_s 0
	set n_c 10
	set delta 1
	while {$delta!=-1} {
		if {$delta==1} {
			draw_screen $R $r_s $r_c $N $n_s $n_c
			set delta 0
		}
		gets stdin s
		if {[string length $s]==0} {
			set delta 1
		} elseif {$s eq "L"} {
			set db_open [open_db]
		} elseif {$s eq "l"} {
			if {[info exists db_open] && $db_open} {
				set R [list]
				set r_s 0
				set delta 1
				db eval $SELS(0) {
					lappend R "$uid $start_time $end_time $code $desc"
				}
			}
		} elseif {$s eq "Q"} {
			return
		}
	}
}

main
