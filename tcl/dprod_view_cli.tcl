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
	puts "Productivity Viewer\nResults table from lookup"
	print_rows $results $r_i $r_c
	puts {Noted records}
	print_rows $notes $n_i $n_c
	puts "Command Bar\n(L)oad (Q)uit (l)ookup (a)dd-to-notes (d)elete-from-notes\n(S)ort (C)lear (1)st-page (n)ext-page (p)rev-page (s)ummarize\n(e)mployee (f)ilter"
}
proc open_db {{s ""}} {
	if {[string length $s]==0} {
		puts "Opening DB for viewing.\nPlease enter file name.\nFile name:"
		gets stdin s
	}
	if {[string length $s]>0 && [file exists $s]} {
		sqlite3 db $s
		if {[sqlmat db]} {
			puts "'$s' loaded for viewing."
			return true
		} else {
			puts "Database schema does not match intended.\nClosing file."
		}
	}
	return false
}
proc row_choose {max} {
	puts "Row selection from 0 to [expr $max-1]\nTo indicate a range, use a hyphen to seperate two numbers.\nFor specific rows, use commas to seperate numbers."
	if {[gets stdin s]>0} {
		if {[string first - $s]!=-1} {
			set r [split $s -]
			foreach n $r {
				if {[expr !{[string is integer $n]}]} {
					puts {Invalid numbers.}
					return
				}
			}
			if {[lindex $r 1]<=[lindex $r 0] || [lindex $r 0]<0 || [lindex $r 1]>=$max} {
				puts {Invalid range given.}
				return
			}
			set l [list]
			for {set i [lindex $r 0]} {$i < [lindex $r 1]+1} {incr i} {
				lappend l $i
			}
			return $l
		} elseif {[string first , $s]!=-1} {
			set r [split $s ,]
			foreach n $r {
				if {[expr !{[string is integer $n]}] || $n >= $max || $n < 0} {
					puts {Invalid number.}
					return
				}
			}
			return $r
		} else {
			if {[string is integer $s]} {
				return $s
			} else {
				puts {Bad input??}
				return
			}
		}
	}
}
proc all_int {l} {
	foreach n $l {
		if {[string is integer $n]==0} { return 0 }
	}
	return 1
}
proc date_in {} {
	if {[gets stdin s]==0} return
	if {[string first / $s]!=-1 && [llength [set s [split $s /]]]==3} {
		if {[all_int $s]==1} { return [format %04d-%02d-%02d [lindex $s 2] {*}[lrange $s 0 1]] }
	} elseif {[string first - $s]!=-1 && [llength [set s [split $s -]]]==3} {
		if {[all_int $s]==1} { return [format %04d-%02d-%02d {*}$s] }
	} elseif {[string length $s]>=8} {
		for {set i 0} {$i<8} {incr i} {
			if {[string is integer [string index $s $i]]==0} return
		}
		return "[string range $s 4 7]-[string range $s 0 1]-[string range $s 2 3]"
	}
}
proc d_lookup {} {
	array set DTE {
		b {WHERE date(end_time)<}
		a {WHERE date(start_time)>}
		d {WHERE date(start_time)=}
	}
	puts "Record lookup choices\n()all\nExact / Limited\n(u)id (c)ode (b)efore (d)ate (a)fter\nBy Order (prepend with space)\n( u)id ( c)ode ( s)tart ( e)nd ( d)esc"
	gets stdin s
	switch [string length $s] {
		0 { return {SELECT * from prod_records} }
		1 {
			switch $s {
				b - a - d {
					puts {Enter requirement}
					if {[string length [set t [date_in]]]==0} {
						puts {Requirement is required.}
					} else {
						return  "SELECT * from prod_records $DTE($s)\"$t\""
					}
				}
			}
		}
	}
}
proc main {{f ""}} {
	set R [list]
	set r_s 0
	set r_c 10
	set N [list]
	set n_s 0
	set n_c 10
	set delta 1
	if {[string length $f]>0} {
		set db_open [open_db $f]
	}
	while {$delta!=-1} {
		if {$delta==1} {
			draw_screen $R $r_s $r_c $N $n_s $n_c
			set delta 0
		}
		gets stdin s
		if {[string length $s]==0} {
			set delta 1
		}
		switch $s {
			L { set db_open [open_db] }
			l {
				if {[info exists db_open] && $db_open} {
					if {[string length [set q [d_lookup]]]!=0} {
					set R [list]
					set r_s 0
					puts $q
					db eval $q {
						lappend R "$uid $start_time $end_time $code $desc"
					}
					set delta 1
					}
				}
			}
			a {
				set q [row_choose [llength $R]]
				foreach r $q {
					puts [lindex $R $r]
					lappend N [lindex $R $r]
				}
				set delta 1
			}
			d {
				set q [lsort -unique [row_choose [llength $N]]]
				puts $q
				if {[string length q]>0} {
					set q_i 0
					set n_l [llength $N]
					set NN [list]
					for {set i 0} {$i<$n_l} {incr i} {
						if {$i==[lindex $q $q_i]} {
							incr q_i
							if {$q_i == [llength $q]} {
								puts $NN
								puts [lrange $N $i+1 [llength $N]]
								lappend NN {*}[lrange $N $i+1 [llength $N]+1]
								break
							}
						} else {
							lappend NN [lindex $N $i]
						}
					}
					set N $NN
					unset NN
					set delta 1
				}
			}
			Q { set delta -1 }
		}
	}
}

if {[llength $argv]==0} {
	main
} else {
	main [lindex $argv 0]
}
