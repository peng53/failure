#!/usr/bin/tclsh8.6
package require sqlite3

namespace eval viewer {
	variable R [list]
	variable N [list]
	variable R_o 0
	variable N_o 0
	variable R_c 10
	variable N_c 5
	variable delta 1
	variable db_open 0
	proc sqlmat {} {
		array set ccols { uid {text} start_time {datetime} end_time {datetime} code {text} desc {text} }
		db eval {pragma table_info(prod_records)} {
			try {
				if {$ccols($name) ne $type} { return 0 }
			} on error {} { return 0 }
		}
		return 1
	}
	proc open_db_silent {s} {
		if {[file exists $s]} {
			sqlite3 db $s
			if {[sqlmat]} {
				variable db_open
				set db_open 1
			}
		}
	}
	proc open_db {} {
		puts "Opening DB for viewing.\nPlease enter file name.\nFile name:"
		if {[gets stdin s]>0 && [file exists $s]} {
			sqlite3 db $s
			if {[sqlmat]} {
				puts "'$s' loaded for viewing."
				variable db_open
				set db_open 1
			} else { puts "Database schema does not match intended.\nClosing file." }
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
		if {[string first / $s]!=-1} {
			if {[llength [set s [split $s /]]]==3 && [all_int $s]==1} { return [format %04d-%02d-%02d [lindex $s 2] {*}[lrange $s 0 1]] }
		} elseif {[string first - $s]!=-1} {
			if {[llength [set s [split $s -]]]==3 && [all_int $s]==1} { return [format %04d-%02d-%02d {*}$s] }
		} elseif {[string length $s]>=8 && [string is integer $s]} {
			return "[string range $s 4 7]-[string range $s 0 1]-[string range $s 2 3]"
		}
	}
	proc d_lookup {} {
		if {$viewer::db_open==0} { return }
		set base {SELECT * from prod_records}
		array set WHE {
			b {WHERE date(end_time)<} a {WHERE date(start_time)>} d {WHERE date(start_time)=}
			u {WHERE uid=} c {WHERE code=} ou {ORDER by uid} oc {ORDER by code}
			os {ORDER by start_time} oe {ORDER by end_time} od {ORDER by desc}
		}
		puts "Lookup Options\nBy Order: (ou)id (oc)ode (os)tart (oe)nd (od)esc ()all\nExact / Limited: (u)id (c)ode (b)efore (d)ate (a)fter"
		switch [gets stdin s] {
			0 { db_q $base
			} 1 {
				switch $s {
					b - a - d {
						puts {Enter requirement}
						if {[string length [set t [date_in]]]==0} { puts {Requirement is required.}
						} else { db_q "$base $WHE($s)\"$t\"" }
					} u - c {
						puts {Enter requirement}
						if {[gets stdin t]==0} { puts {Requirement is required.}
						} else { db_q "$base $WHE($s)\"$t\"" }
					}
				}
			} 2 {
				switch $s {
					ou - oc - os - oe - od { db_q "$base $WHE($s)" }
				}
			}
		}
	}
	proc db_q {q} {
		variable R
		variable R_o
		variable delta
		set R [list]
		set R_o 0
		set delta 1
		db eval $q { lappend R "$uid $start_time $end_time $code $desc" }
	}
	proc print_rows {rows off c} {
		puts [string repeat _ 62]
		puts { # {  UID /    Start Time   \ /    End Time     }  Code | Desc}
		set n -1
		foreach r [lrange $rows $off [expr min($c+$off,[llength $rows])]] {
			puts [format "%2d ! %4s | %10s %5s | %10s %5s | %5s | %-22s" [expr [incr n]+$off] {*}$r]
		}
		if {$n==0} { set n [expr max($c-3,0)]}
		while {[incr n]<$c} { puts [format "   '%6s'%18s'%18s'%7s'" "" "" "" ""] }
		puts "[string repeat # 62] Pg [expr $off/$c]"
	}
	proc draw_screen {} {
		variable delta
		set delta 0
		puts "Productivity Viewer\nLookup Results"
		print_rows $viewer::R $viewer::R_o $viewer::R_c
		puts {Noted records}
		print_rows $viewer::N $viewer::N_o $viewer::N_c
		puts "(L)oad (Q)uit (l)ookup (a)dd-to-notes (d)elete-from-notes\n(S)ort (C)lear (1)st-page (n)ext-page (p)rev-page (s)ummarize\n(e)mployee (f)ilter"
	}
	proc row_choose {max} {
		puts "Row selection from 0-[expr $max-1]\nRange (-) List (,)"
		if {[gets stdin s]>0} {
			if {[string first - $s]!=-1} {
				lassign [split $s -] a b
				if {[string is integer $a] && [string is integer $b] && 0<=$a && $a<$b && $b<$max} {
					set l $a
					while {$a<$b} { lappend l [incr a] }
					return $l
				} else { puts {Invalid range / number(s) given.} }
			} elseif {[string first , $s]!=-1} {
				set l [list]
				foreach n [split $s ,] {
					if {[string is integer $n] && 0<=$n && $n<$max} { lappend l $n }
				}
				return $l
			} elseif {[string is integer $s] && 0<=$s && $s<$max} { return $s }
		}
	}
	proc d_addnote {} {
		variable N
		variable delta
		set delta 1
		foreach i [row_choose [llength $viewer::R]] { lappend N [lindex $viewer::R $i] }
	}
	proc d_delnote {} {
		variable N
		variable delta
		set delta 1
		set n_l [llength $N]
		set q [lsort -unique [row_choose $n_l]]
		set q_i 0
		set n_i 0
		set NN [list]
		foreach n $N {
			if {$n_i==[lindex $q $q_i]} {
				incr q_i
				if {$q_i == [llength $q]} {
					lappend NN {*}[lrange $N $n_i+1 $n_l+1]
					break
				}
			} else { lappend NN $n }
			incr n_i
		}
		set N $NN

	}
	proc d_next {c} {
		switch $c {
			R {
				variable R_o
				if {$R_o<[llength $viewer::R]} {
					variable delta
					set delta 1
					incr R_o $viewer::R_c
				}
			} N {
				variable N_o
				if {$N_o<[llength $viewer::N]} {
					variable delta
					set delta 1
					incr N_o $viewer::N_c
				}
			}
		}
	}
	proc d_prev {c} {
		switch $c {
			R {
				variable R_o
				if {$R_o!=0} {
					variable delta
					set delta 1
					incr R_o [expr -$viewer::R_c]
				}
			} N {
				variable N_o
				if {$N_o!=0} {
					variable delta
					set delta 1
					incr N_o [expr -$viewer::N_c]
				}
			}
		}
	}
	proc d_first {c} {
		variable delta
		set delta 1
		switch $c {
			R {
				variable R_o
				set R_o 0
			} N {
				variable N_o
				set N_o 0
			}
		}
	}
	proc d_clear {c} {
		variable delta
		set delta 1
		switch $c {
			R {
				variable R
				variable R_o
				set R [list]
				set R_o 0
			} N {
				variable N
				variable N_o
				set N [list]
				set N_o 0
			}
		}
	}
	proc active {} {
		if {$viewer::delta==1} { draw_screen }
		if {[gets stdin s]==0} { return 1 }
		switch $s {
			" " { draw_screen }
			L { open_db } Q { return 0 } l { d_lookup } a { d_addnote } d { d_delnote }
			nr { d_next R } nn { d_next N } pr { d_prev R } pn { d_prev N }
			1r { d_first R } 1n { d_first N } Cr { d_clear R } Cn { d_clear N }
		}
		return 1
	}
}
if {[llength $argv]>0} { viewer::open_db_silent [lindex $argv 0] }
while {[viewer::active]==1} { continue }
