#!/usr/bin/env tclsh8.6
package require Tk
namespace eval Cal {
	namespace eval v {
		variable MTH_DAYS {-1 31 28 31 30 31 30 31 31 30 31 30 31}
		variable MTH_NAME {NULL January Febuary March April May June July August September October November December}
		variable MTH 1 YR 2000
		variable selA 0 selB 0
		variable SQS 26 tOff 13 dfont {Arial 10}
		variable P
	}
	proc dowMY {m y} {
		# Returns first-day-of-the-week given month & year. Where Sun=0 .. Sat=6
		return [clock format [clock scan $m-01-$y -format %m-%d-%Y] -format %w]
	}
	proc is_lpyr {y} {
		# Returns whether a year is a leap-year.
		return [expr {(($y%4==0) && ($y%100!=0 || $y%400==0))}]
	}
	proc mth_day_ct {m y} {
		# Returns days in month and year.
		if {$m==2 && [is_lpyr $y]} { return 29 }
		return [lindex $v::MTH_DAYS $m]
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
		if {$m<1 || $m>12 | $y<1 || $y>9999} { return }
		set v::MTH $m
		set v::YR $y
	}
	proc Init {parent m y sqs toff dfont} {
		# Calls/sets init procs and vars for Cal
		set_mth_yr $m $y
		set v::SQS $sqs
		set v::tOff $toff
		set v::dfont $dfont
		set v::P $parent
		grid [frame $parent]
		grid [frame $parent.mth]
		grid [canvas $parent.can -width [expr 7*$v::SQS] -height [expr 7*$v::SQS] -bg #eee]
		grid [button $parent.mth.prv -text < -command Cal::prev_mth] -column 0 -row 0
		grid [label $parent.mth.mthl] -column 1 -row 0
		grid [button $parent.mth.nxt -text > -command Cal::next_mth] -column 2 -row 0
		grid [entry $parent.date_range]
		grid [button .bquit -text Quit -command exit]
		bind $parent.can <ButtonPress-1> {
			focus $Cal::v::P.can
			# Selects a rect (a day) on the canvas (calender).
			lassign [Cal::rc_canvas [%W canvasy %y] [%W canvasx %x]] r c
			if {$r<1} { return }
			Cal::setSel $r $c
		}
		bind $parent.can <Shift-ButtonPress-1> {
			# Adds rect(s) (day(s)) to (previous) selection.
			lassign [Cal::rc_canvas [%W canvasy %y] [%W canvasx %x]] r c
			if {$r<1} { return }
			Cal::adSel $r $c
		}
		bind $v::P.can <ButtonPress-3> {
			Cal::clearSel
		}
		bind $v::P.can <Control-Key-Left> {
			Cal::prev_mth
		}
		bind $v::P.can <Control-Key-Right> {
			Cal::next_mth
		}
		bind $v::P.can <Control-Key-Left> {
			Cal::prev_mth
		}
		bind $v::P.can <Control-Key-Up> {
			Cal::set_mth_yr $Cal::v::MTH [expr $Cal::v::YR-1]
			Cal::cal_day
		}
		bind $v::P.can <Control-Key-Down> {
			Cal::set_mth_yr $Cal::v::MTH [expr $Cal::v::YR+1]
			Cal::cal_day
		}
		bind $v::P.can <Key-Left> {
			if {$Cal::v::selA==0} { return }
			if {$Cal::v::selB!=0} { return }
			lassign $Cal::v::selA m d y r c
			if {$c==0} {
				Cal::prev_mth
				Cal::setSel $r 6
			} else {
				Cal::setSel $r [expr $c-1]
			}
		}
		bind $v::P.can <Key-Right> {
			if {$Cal::v::selA==0} { return }
			if {$Cal::v::selB!=0} { return }
			lassign $Cal::v::selA m d y r c
			if {$c==6} {
				Cal::next_mth
				Cal::setSel $r 0
			} else {
				Cal::setSel $r [expr $c+1]
			}
		}
		bind $v::P.can <Key-Up> {
			if {$Cal::v::selA==0 || $Cal::v::selB!=0 } { return }
			lassign $Cal::v::selA m d y r c
			if {$r!=0} {
				Cal::setSel [expr $r-1] $c
			}
		}
		bind $v::P.can <Key-Down> {
			if {$Cal::v::selA==0 || $Cal::v::selB!=0 } { return }
			lassign $Cal::v::selA m d y r c
			if {$r!=7} {
				Cal::setSel [expr $r+1] $c
			}
		}
		cal_base
		cal_day
	}
	proc prev_mth {} {
		# Set current shown month back by 1 and redraws.
		if {$v::MTH==1} {
			set v::MTH 12
			incr v::YR -1
		} else { incr v::MTH -1 }
		cal_day
		shown_dates
	}
	proc next_mth {} {
		# Set current shown month forward by 1 and redraws.
		if {$v::MTH==12} {
			set v::MTH 1
			incr v::YR
		} else { incr v::MTH }
		cal_day
		shown_dates
	}
	proc sel_RC {r c} {
		# Draw selection rect at row and column.
		sel_YX [expr $r*$v::SQS] [expr $c*$v::SQS]
	}
	proc sel_YX {y x} {
		# Draw selection rect at y,x coordinate.
		$v::P.can create rect [expr $x+1] [expr $y+1] [expr $x+$v::SQS] [expr $y+$v::SQS] -fill #fff -outline #fff -tags sel
	}
	proc cal_base {} {
		# Draws base of calender; the day-name header.
		$v::P.can create rect 1 1 [expr $v::SQS*7] $v::SQS -fill #000
		set y [set x [expr $v::SQS/2]]
		foreach d {S M T W T F S} {
			$v::P.can create text $x $y -text $d -font $v::dfont -fill #fff
			incr x $v::SQS
		}
	}
	proc cal_day {} {
		# Draws days of the MTH and YR.
		$v::P.can delete -tag days
		$v::P.mth.mthl configure -text "[lindex $v::MTH_NAME $v::MTH] $v::YR"
		set f [dowMY $v::MTH $v::YR]
		set x [expr {1+$f*$v::SQS}]
		set y [expr $v::SQS+1]
		# Current month's days
		set l [mth_day_ct $v::MTH $v::YR]
		for {set d 1} {$d<=$l} {incr d} {
			if {$d%2} {
				set T odd
				set c #ddd
			} else {
				set T even
				set c #ccc
			}
 			$v::P.can create rect $x $y [incr x $v::SQS] [expr {$y+$v::SQS}] -fill $c -outline $c -tag "$T days"
 			$v::P.can create text [expr {$x-$v::tOff}] [expr {$y+$v::tOff}] -text $d -font $v::dfont -tag "$T days text"
			if {$f==6} {
				set x 1
				set f 0
				incr y $v::SQS
			} else {
				incr f
			}
		}
	}
	proc month_range {} {
		return "$v::MTH 1 $v::YR $v::MTH [mth_day_ct $v::MTH $v::YR] $v::YR"
	}
	proc shown_dates {} {
		$v::P.date_range delete 0 end
		$v::P.can delete -tags sel
		if {$v::selA==0} { return }
		lassign $v::selA ma da ya ra ca
		if {$v::selB==0} {
			if {$v::YR==$ya && $v::MTH==$ma} {
				sel_RC $ra $ca
				$v::P.can raise text sel
				$v::P.date_range insert end [format {%02d/%02d/%04d} $ma $da $ya]
			}
		} else {
			lassign $v::selB mb db yb rb cb
			if {$mb<$v::MTH || $yb<$v::YR || $ma>$v::MTH || $ya>$v::YR} {
				return
			}
			$v::P.date_range insert end [format {%02d/%02d/%04d - %02d/%02d/%04d} $ma $da $ya $mb $db $yb]
			if {$ma<$v::MTH || $ya<$v::YR} {
				set da 1
				set ra 1
				set ca [dowMY $v::MTH $v::YR]
			}
			if {$mb>$v::MTH || $yb>$v::YR} {
				set db [mth_day_ct $v::MTH $v::YR]
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
			$v::P.can raise text sel
		}
	}
	proc clearSel {} {
		set v::selA 0 
		set v::selB 0
		shown_dates
	}
	proc rc_date {r c} {
		# Returns the date shown on canvas with row and column.
		set d [expr {$c-[dowMY $v::MTH $v::YR]+7*$r-6}]
		if {$d<1} {
			lassign [mthyr -- $v::MTH $v::YR] m y
			return "$m [expr {[mth_day_ct $m $y]+$d}] $y"
		}
		if {$d>[set l [mth_day_ct $v::MTH $v::YR]]} {
			lassign [mthyr ++ $v::MTH $v::YR] m y
			return "$m [expr {$d-$l}] $y"
		}
		return "$v::MTH $d $v::YR"
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
		if {$m==$v::MTH} {
			set v::selB 0 
			set v::selA "$m $d $y $r $c"
			shown_dates
		}
	}
	proc adSel {r c} {
		# Adjusts the selection rect(s) to expand/shrink to the row and column.
		lassign [rc_date $r $c] m d y
		if {$m!=$v::MTH} { return }
		if {$v::selA==0} {
			setSel $r $c
			return
		} else {
			if {$v::selB==0} {
				set v::selB "$m $d $y $r $c"
			} else {
				lassign $v::selA ma da ya
				lassign $v::selB mb db yb
				if {[dateC $m $d $y $ma $da $ya]==-1} {
					set v::selA "$m $d $y $r $c"
				} else {
					set v::selB "$m $d $y $r $c"
				}
			}
		}
		if {[dateC {*}[lrange $v::selA 0 2] {*}[lrange $v::selB 0 2]]==1} {
			# Swap selected indices so that selA occurs before selB
			set T $v::selA
			set v::selA $v::selB
			set v::selB $T
		}
		shown_dates
	}
	proc rc_canvas {y x} {
		return "[expr {int($y/$v::SQS)}] [expr {int($x/$v::SQS)}]"
	}
	bind . <Control-Key-q> {
		exit
	}
}
proc main {} {
	set square_size 26
	set text_offset 13
	set day_font {Arial 12}
	# Init Cal with the current month and year.
	lassign [clock format [clock seconds] -format "%N %Y"] m y
	set f .cal
	Cal::Init .cal $m $y $square_size $text_offset $day_font
}
main 