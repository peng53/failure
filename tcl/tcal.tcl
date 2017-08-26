#!/usr/bin/env tclsh8.6
package require Tk
namespace eval Cal {
	array set days { Sun {0} Mon {1} Tue {2} Wed {3} Thu {4} Fri {5} Sat {6} }
	variable SQS 64 mth {31 31 28 31 30 31 30 31 31 30 31 30 31} imth {Null January Febuary March April May June July August September October November December}
	variable c_mth 1 c_yr 2000 selA 0 selB 0 dfont {Arial 21}
	proc dowMY {m y} {
		# Returns first-day-of-the-week given month & year. Where Sun=0 .. Sat=6
		return $Cal::days([clock format [clock scan $m-01-$y -format %m-%d-%Y] -format %a])
	}
	proc is_lpyr {y} {
		# Returns whether a year is a leap-year.
		return [expr {(($y%4==0) && ($y%100!=0 || $y%400==0))}]
	}
	proc Init { m y } {
		# Calls/sets init procs and vars for Cal
		variable c_mth $m c_yr $y
		cal_base
		cal_day
	}
	proc prev_mth {} {
		# Set current shown month back by 1 and redraws.
		variable c_mth
		incr c_mth -1
		if {$c_mth==0} {
			set c_mth 12
			variable c_yr
			incr c_yr -1
		}
		cal_day
		shown_dates
	}
	proc next_mth {} {
		# Set current shown month forward by 1 and redraws.
		variable c_mth
		incr c_mth
		if {$c_mth==13} {
			set c_mth 1
			variable c_yr
			incr c_yr
		}
		cal_day
		shown_dates
	}
	proc sel_RC {r c} {
		# Draw selection rect at row and column.
		sel_YX [expr $r*$Cal::SQS] [expr $c*$Cal::SQS]
	}
	proc sel_YX {y x} {
		# Draw selection rect at y,x coordinate.
		.fcal.can create rect [expr $x+1] [expr $y+1] [expr $x+$Cal::SQS] [expr $y+$Cal::SQS] -outline #666 -tags sel
	}
	proc cal_base {} {
		# Draws base of calender; the day-name header.
		.fcal.can create rect 1 1 [expr $Cal::SQS*7] $Cal::SQS -fill #000
		set y [set x [expr $Cal::SQS/2]]
		foreach d {S M T W T F S} {
			.fcal.can create text $x $y -text $d -font $Cal::dfont -fill #fff
			incr x $Cal::SQS
		}
	}
	proc cal_day {} {
		# Draws days of the c_mth and c_yr.
		.fcal.can delete -tag ndays
		.fcal.can delete -tag odays
		.fcal.can delete -tag edays
		set m $Cal::c_mth
		set y $Cal::c_yr
		.fcal.mth.mthl configure -text "[lindex $Cal::imth $Cal::c_mth] $Cal::c_yr"
		set f [Cal::dowMY $m $y]
		set x 1
		set y [expr $Cal::SQS+1]
		# Past month's days
		if {$f>0} {
			if {$m==3 && [Cal::is_lpyr $y]} {
				set l 29
				set d [expr {29-$f}]
			} else {
				set l [lindex $Cal::mth [expr {$m-1}]]
				set d [expr {$l+1-$f}]
			}
			while {$d<=$l} {
				.fcal.can create rect $x $y [expr {$x+$Cal::SQS}] [expr {$y+$Cal::SQS}] -fill #eee -outline #ddd -tag ndays
				.fcal.can create text [expr {$x+$Cal::SQS/2}] [expr {$y+$Cal::SQS/2}] -text $d -font $Cal::dfont -fill #666 -tag ndays
				incr x $Cal::SQS
				incr d
			}
		}
		# Current month's days
		set d 1
		set l [expr ($m==2 && [Cal::is_lpyr $y]) ? 29 : [lindex $Cal::mth $m]]
		while {$d<=$l} {
			if {[expr {$d%2}]} {
				set T odays
				set c #ddd
			} else {
				set T edays
				set c #ccc
			}
 			.fcal.can create rect $x $y [expr {$x+$Cal::SQS}] [expr {$y+$Cal::SQS}] -fill $c -outline $c -tag $T
 			.fcal.can create text [expr {$x+$Cal::SQS/2}] [expr {$y+$Cal::SQS/2}] -text $d -font $Cal::dfont -tag $T
			incr f
			incr x $Cal::SQS
			incr d
			if {$f==7} {
				set x 1
				set f 0
				incr y $Cal::SQS
			}
		}
		# Next month's days
		set d 1
		while {$f<7} {
			.fcal.can create rect $x $y [expr {$x+$Cal::SQS}] [expr {$y+$Cal::SQS}] -fill #eee -outline #ddd -tag ndays
			.fcal.can create text [expr {$x+$Cal::SQS/2}] [expr {$y+$Cal::SQS/2}] -text $d -font $Cal::dfont -fill #666 -tag ndays
			incr f
			incr x $Cal::SQS
			incr d
		}
		if {$y!=[expr {6*$Cal::SQS}]} {
			incr y $Cal::SQS
			set f 0
			set x 1
			while {$f<7} {
				.fcal.can create rect $x $y [expr {$x+$Cal::SQS}] [expr {$y+$Cal::SQS}] -fill #eee -outline #ddd -tag ndays
				.fcal.can create text [expr {$x+$Cal::SQS/2}] [expr {$y+$Cal::SQS/2}] -text $d -font $Cal::dfont -fill #666 -tag ndays
				incr f
				incr x $Cal::SQS
				incr d
			}
		}
	}
	proc month_range {} {
		set a_date [rc_date 1 0]
		set b_date [rc_date 6 6]
		return "$a_date $b_date"
	}
	proc shown_dates {} {
		variable selA
		variable selB
		.fcal.can delete -tags sel
		if {$selA==0} { return }
		lassign $selA ma da ya ra ca
		lassign [month_range] m0 d0 y0 m1 d1 y1
		puts "Selected A: $selA"
		puts "Selected B: $selB"
		puts "MonBegin: $m0 $d0 $y0"
		puts "MonEnd  : $m1 $d1 $y1"
		if {$selB==0} {
			if {[dateC $m0 $d0 $y0 $ma $da $ya]!=1 && [dateC $ma $da $ya $m1 $d1 $y1]!=1} {
				#selA is on the current month
				sel_RC $ra $ca
			}
		} else {
			lassign $selB mb db yb rb cb
			if {[dateC $mb $db $yb $m0 $d0 $y0]==-1 || [dateC $ma $da $ya $m1 $d1 $y1]==1} {
				# Selected dates end before current month or start after current month.
				return
			}
			if {[dateC $m0 $d0 $y0 $ma $da $ya]==1} {
				# Selected dates begin before current month (but may end during or after).
				set ra 1
				set ca 0
			}
			if {[dateC $mb $db $yb $m1 $d1 $y1]==1} {
				# Selected dates end after current month (but may begin during or before).
				set rb 6
				set cb 6
			}
			while {$ra<$rb} {
				while {$ca<7} {
					sel_RC $ra $ca
					incr ca
				}
				incr ra
				set ca 0
			}
			while {$ca<=$cb} {
				sel_RC $ra $ca
				incr ca
			}
		}
	}

	proc rc_date {r c} {
		# Returns the date shown on canvas with row and column.
		variable c_mth
		variable c_yr
		set l [lindex $Cal::mth $c_mth]
		set f [dowMY $c_mth $c_yr]
		set d [expr {$c-$f+7*$r-6}]
		if {$d<1} {
			if {$c_mth==3 && [is_lpyr $c_yr]} { return [format "2 %d %d" [expr {29+$d}] $c_yr]
			} elseif {$c_mth==1} { return [format "12 %d %d" [expr {31+$d}] [expr {$c_yr-1}]]
			} else { return [format "%d %d %d" [expr {$c_mth-1}] [expr {[lindex $Cal::mth [expr {$c_mth-1}]]+$d}] $c_yr] }
		} elseif {$d>$l} {
			if {$c_mth==12} { return [format "1 %d %d" [expr {$d-$l}] [expr {$c_yr+1}]]
			} else { return [format "%d %d %d" [expr {$c_mth+1}] [expr {$d-$l}] $c_yr] }
		} else { return [format "%d %d %d" $c_mth $d $c_yr] }
	}
	proc numc {n m} {
		if {[expr $n<$m]} {
			return -1
		} elseif {[expr $n>$m]} {
			return 1
		} else {
			return 0
		}
	}
	proc dateC {m d y m2 d2 y2} {
		# returns 0 if equal -1 if < and 1 if >
		#foreach n [list [numc $y $y2] [numc $m $m2] [numc $d $d2]] {
		#	if {$n!=0} { return $n }
		#}
		#return 0
		if {$y<$y2} {
			puts "$y<$y2"
			return -1
		}
		if {$y>$y2} {
			puts "$y>$y2"
			return 1
		}
		if {$m<$m2} {
			puts "$m<$m2"
			return -1
		}
		if {$m>$m2} {
		return 1
		}
		if {$d<$d2} {
		return -1
		}
		if {$d>$d2} {
		return 1
		}
		# At this point the MDY must match.
		return 0
	}
	proc setSel {r c} {
		# Set the selection rect to the row and column.
		variable selB 0 selA "[rc_date $r $c] $r $c"
		.fcal.can delete -tags sel
		sel_RC $r $c
		.date_range delete 0 end
		.date_range insert end $Cal::selA
	}
	proc adSel {r c} {
		# Adjusts the selection rect(s) to expand/shrink to the row and column.
		variable selA
		if {$selA==0} {
			setSel $r $c
			return
		} else {
			variable selB
			if {$selB==0} {
				set selB "[rc_date $r $c] $r $c"
			} else {
				lassign [rc_date $r $c] m d y
				lassign $selA ma da ya
				lassign $selB mb db yb
				if {[dateC $m $d $y $ma $da $ya]==-1} {
					set selA "$m $d $y $r $c"
				} else {
					set selB "$m $d $y $r $c"
				}
			}
		}
		if {[dateC {*}[lrange $selA 0 2] {*}[lrange $selB 0 2]]==1} {
			# Swap selected indices so that selA occurs before selB
			set T $selA
			set selA $selB
			set selB $T
		}
		.date_range delete 0 end
		.date_range insert end "$selA - $selB"
		shown_dates
	}
}
grid [frame .fcal]
grid [frame .fcal.mth]
grid [canvas .fcal.can -width [expr 7*$Cal::SQS] -height [expr 7*$Cal::SQS] -bg white]
grid [button .fcal.mth.prv -text < -command Cal::prev_mth] -column 0 -row 0
grid [label .fcal.mth.mthl] -column 1 -row 0
grid [button .fcal.mth.nxt -text > -command Cal::next_mth] -column 2 -row 0
grid [entry .date_range]
grid [button .bquit -text Quit -command exit]

bind .fcal.can <ButtonPress-1> {
	# Selects a rect (a day) on the canvas (calender).
	if {[set r [expr int([%W canvasy %y]/$Cal::SQS)]]<1} { return }
	set c [expr int([%W canvasx %x]/$Cal::SQS)]
	Cal::setSel $r $c
}
bind .fcal.can <Shift-ButtonPress-1> {
	# Adds rect(s) (day(s)) to (previous) selection.
	if {[set r [expr int([%W canvasy %y]/$Cal::SQS)]]<1} { return }
	set c [expr int([%W canvasx %x]/$Cal::SQS)]
	Cal::adSel $r $c
}
bind .fcal.can <ButtonPress-3> {
	.date_range delete 0 end
	.date_range insert end [Cal::month_range]
}

# Init Cal with the current month and year.
lassign [clock format [clock seconds] -format "%N %Y"] m y
Cal::Init $m $y
