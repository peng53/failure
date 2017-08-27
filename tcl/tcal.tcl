#!/usr/bin/env tclsh8.6
package require Tk
namespace eval Cal {
	array set days { Sun {0} Mon {1} Tue {2} Wed {3} Thu {4} Fri {5} Sat {6} }
	variable SQS 26 mth {31 31 28 31 30 31 30 31 31 30 31 30 31} imth {Null January Febuary March April May June July August September October November December}
	variable c_mth 1 c_yr 2000 selA 0 selB 0 dfont {Arial 10}
	variable tOff 13
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
	proc mth_day_ct {m y} {
		# Returns days in month and year.
		if {$m==2 && [is_lpyr $y]} { return 29 }
		return [lindex $Cal::mth $m]
	}
	proc mthyr {sub m y} {
		# Returns month and year applying subcommand.
		switch $sub {
			-- {
				if {$m==1} { return "12 [expr {$y-1}]" }
				return "[expr {$m-1}] $y"
			}
			++ {
				if {$m==12} { return "1 [expr {$y+1}]" }
				return "[expr {$m+1}] $y"
			}
		}
	}
	proc set_mth_yr {m y} {
		if {$m<1 || $m>12 | $y<1 || $y>9999} {
			return
		}
		variable c_mth $m c_yr $y
	}
	proc prev_mth {} {
		# Set current shown month back by 1 and redraws.
		variable c_mth
		variable c_yr
		lassign [mthyr -- $c_mth $c_yr] m y
		set c_mth $m
		set c_yr $y
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
		.fcal.can create rect [expr $x+1] [expr $y+1] [expr $x+$Cal::SQS] [expr $y+$Cal::SQS] -fill #fff -outline #fff -tags sel
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
		.fcal.can delete -tag days
		variable c_mth
		variable c_yr
		variable SQS
		#set SQH [expr {$SQS/2}]
		variable tOff
		.fcal.mth.mthl configure -text "[lindex $Cal::imth $c_mth] $c_yr"
		set f [dowMY $c_mth $c_yr]
		set x [expr {1+$f*$SQS}]
		set y [expr $SQS+1]
		# Current month's days
		set l [mth_day_ct $c_mth $c_yr]
		for {set d 1} {$d<=$l} {incr d} {
			if {$d%2} {
				set T odd
				set c #ddd
			} else {
				set T even
				set c #ccc
			}
 			.fcal.can create rect $x $y [incr x $SQS] [expr {$y+$SQS}] -fill $c -outline $c -tag "$T days"
 			.fcal.can create text [expr {$x-$tOff}] [expr {$y+$tOff}] -text $d -font $Cal::dfont -tag "$T days text"
			if {$f==6} {
				set x 1
				set f 0
				incr y $SQS
			} else {
				incr f
			}
		}
	}
	proc month_range {} {
		variable c_mth
		variable c_yr
		return "$c_mth 1 $c_yr $c_mth [mth_day_ct $c_mth $c_yr] $c_yr"
	}
	proc shown_dates {} {
		.date_range delete 0 end
		variable selA
		.fcal.can delete -tags sel
		if {$selA==0} { return }
		lassign $selA ma da ya ra ca
		variable selB
		if {$selB==0} {
			if {$Cal::c_yr==$ya && $Cal::c_mth==$ma} {
				sel_RC $ra $ca
				.fcal.can raise text sel
				.date_range insert end [format {%02d/%02d/%04d} $ma $da $ya]
			}
		} else {
			lassign $selB mb db yb rb cb
			if {$mb<$Cal::c_mth || $yb<$Cal::c_yr || $ma>$Cal::c_mth || $ya>$Cal::c_yr} {
				return
			}
			.date_range delete 0 end
			.date_range insert end [format {%02d/%02d/%04d - %02d/%02d/%04d} $ma $da $ya $mb $db $yb]
			if {$ma<$Cal::c_mth || $ya<$Cal::c_yr} {
				set da 1
				set ra 1
				set ca [dowMY $Cal::c_mth $Cal::c_yr]
			}
			if {$mb>$Cal::c_mth || $yb>$Cal::c_yr} {
				set db [mth_day_ct $Cal::c_mth $Cal::c_yr]
			}
			while {$da<=$db} {
				sel_RC $ra $ca
				incr da
				if {$ca==6} {
					incr ra
					set ca 0
				} else {
					incr ca
				}
			}
			.fcal.can raise text sel
		}
	}
	proc clearSel {} {
		variable selA 0 variable selB 0
		shown_dates
	}
	proc rc_date {r c} {
		# Returns the date shown on canvas with row and column.
		variable c_mth
		variable c_yr
		set d [expr {$c-[dowMY $c_mth $c_yr]+7*$r-6}]
		if {$d<1} {
			lassign [mthyr -- $c_mth $c_yr] m y
			return "$m [expr {[mth_day_ct $m $y]+$d}] $y"
		}
		if {$d>[set l [mth_day_ct $c_mth $c_yr]]} {
			lassign [mthyr ++ $c_mth $c_yr] m y
			return "$m [expr {$d-$l}] $y"
		}
		return "$c_mth $d $c_yr"
	}
	proc dateC {m d y m2 d2 y2} {
		# returns 0 if equal -1 if < and 1 if >
		if {$y<$y2} { return -1 }
		if {$y>$y2} { return 1 }
		if {$m<$m2} { return -1 }
		if {$m>$m2} { return 1 }
		if {$d<$d2} { return -1 }
		if {$d>$d2} { return 1 }
		# At this point the MDY must match.
		return 0
	}
	proc setSel {r c} {
		# Set the selection rect to the row and column.
		lassign [rc_date $r $c] m d y
		if {$m==$Cal::c_mth} {
			variable selB 0 selA "$m $d $y $r $c"
			shown_dates
		}
	}
	proc adSel {r c} {
		# Adjusts the selection rect(s) to expand/shrink to the row and column.
		lassign [rc_date $r $c] m d y
		if {$m!=$Cal::c_mth} { return }
		variable selA
		if {$selA==0} {
			setSel $r $c
			return
		} else {
			variable selB
			if {$selB==0} {
				set selB "$m $d $y $r $c"
			} else {
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
		shown_dates
	}
}
grid [frame .fcal]
grid [frame .fcal.mth]
grid [canvas .fcal.can -width [expr 7*$Cal::SQS] -height [expr 7*$Cal::SQS] -bg #eee]
grid [button .fcal.mth.prv -text < -command Cal::prev_mth] -column 0 -row 0
grid [label .fcal.mth.mthl] -column 1 -row 0
grid [button .fcal.mth.nxt -text > -command Cal::next_mth] -column 2 -row 0
grid [entry .date_range]
grid [button .bquit -text Quit -command exit]

bind .fcal.can <ButtonPress-1> {
	focus .fcal.can
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
	Cal::clearSel
}
bind .fcal.can <Control-Key-Left> {
	Cal::prev_mth
}
bind .fcal.can <Control-Key-Right> {
	Cal::next_mth
}
bind .fcal.can <Control-Key-Left> {
	Cal::prev_mth
}
bind .fcal.can <Control-Key-Up> {
	Cal::set_mth_yr $Cal::c_mth [expr $Cal::c_yr-1]
	Cal::cal_day
}
bind .fcal.can <Control-Key-Down> {
	Cal::set_mth_yr $Cal::c_mth [expr $Cal::c_yr+1]
	Cal::cal_day
}
bind .fcal.can <Key-Left> {
	if {$Cal::selA==0} { return }
	if {$Cal::selB!=0} { return }
	lassign $Cal::selA m d y r c
	if {$c==0} {
		Cal::prev_mth
		Cal::setSel $r 6
	} else {
		Cal::setSel $r [expr $c-1]
	}
}
bind .fcal.can <Key-Right> {
	if {$Cal::selA==0} { return }
	if {$Cal::selB!=0} { return }
	lassign $Cal::selA m d y r c
	if {$c==6} {
		Cal::next_mth
		Cal::setSel $r 0
	} else {
		Cal::setSel $r [expr $c+1]
	}
}
bind .fcal.can <Key-Up> {
	if {$Cal::selA==0} { return }
	if {$Cal::selB!=0} { return }
	lassign $Cal::selA m d y r c
	if {$r!=0} {
		Cal::setSel [expr $r-1] $c
	}
}
bind .fcal.can <Key-Down> {
	if {$Cal::selA==0} { return }
	if {$Cal::selB!=0} { return }
	lassign $Cal::selA m d y r c
	if {$r!=7} {
		Cal::setSel [expr $r+1] $c
	}
}

# Init Cal with the current month and year.
lassign [clock format [clock seconds] -format "%N %Y"] m y
Cal::Init $m $y
