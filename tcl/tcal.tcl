#!/usr/bin/env tclsh8.6
package require Tk
package provide Tcal 1.0
namespace eval Cal {
	namespace eval v {
		variable MTH_DAYS [list -1 31 28 31 30 31 30 31 31 30 31 30 31]
		variable MTH_NAME [list NULL January Febuary March April May June July August September October November December]
		variable MTH 1 YR 2000
		variable selA 0 selB 0
		variable SQS 26 tOff 13 hh 14 dfont {Arial 10}
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
		# Returns days for month & year.
		return [expr {($m==2 && [is_lpyr $y]) ? 29 : [lindex $v::MTH_DAYS $m]}]
	}
	proc mthyr {sub m y} {
		# Returns month and year applying subcommand.
		switch $sub {
			-- {
				if {$m==1} { return [list 12 [expr {$y-1}]] }
				return [list [expr {$m-1}] $y]
			}
			++ {
				if {$m==12} { return [list 1 [expr {$y+1}]] }
				return [list [expr {$m+1}] $y]
			}
		}
	}
	proc set_mth_yr {m y} {
		# Sets the current month & year if valid
		if {$m<1 || $m>12 | $y<1 || $y>9999} { return }
		set v::MTH $m
		set v::YR $y
	}
	proc CalVars {mth yr sqs toff dfont hh} {
		# Sets vars for Cal
		set_mth_yr $mth $yr
		set v::SQS $sqs
		set v::tOff $toff
		set v::hh $hh
		set v::dfont $dfont
	#	set v::P $parent
	}
	proc Cal {parent} {
		#m y sqs toff dfont hh 
		# Calls/sets init procs and vars for Cal
		#set_mth_yr $m $y
		#set v::SQS $sqs
		#set v::tOff $toff
		#set v::hh $hh
		#set v::dfont $dfont
		set v::P $parent
		frame $parent
		pack [frame $parent.mth] -expand 1 -fill x
		pack [button $parent.mth.prv -text < -command Cal::prev_mth] -side left
		pack [label $parent.mth.mthl] -side left -expand 1 -fill x
		pack [button $parent.mth.nxt -text > -command Cal::next_mth] -side left
		pack [canvas $parent.can -width [expr 7*$v::SQS] -height [expr $v::hh+6*$v::SQS]]
		bind $parent.can <ButtonPress-1> {
			# Selects a rect (a day) on the canvas (calender).
			focus $Cal::v::P.can
			if {[set y [%W canvasy %y]]>$Cal::v::hh} { Cal::setSel {*}[Cal::rc_canvas $y [%W canvasx %x]]}
		}
		bind $parent.can <Shift-ButtonPress-1> {
			# Adds rect(s) (day(s)) to (previous) selection.
			if {[set y [%W canvasy %y]]>$Cal::v::hh} { Cal::adSel {*}[Cal::rc_canvas $y [%W canvasx %x]]}
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
		bind $v::P.can <Control-Key-Up> {
			Cal::set_mth_yr $Cal::v::MTH [expr {$Cal::v::YR-1}]
			Cal::cal_day
		}
		bind $v::P.can <Control-Key-Down> {
			Cal::set_mth_yr $Cal::v::MTH [expr {$Cal::v::YR+1}]
			Cal::cal_day
		}
		bind $v::P.can <Key-Left> {
			if {$Cal::v::selA==0 || $Cal::v::selB!=0} { return }
			lassign [lrange $Cal::v::selA 3 4] r c
			if {$c==0} {
				Cal::prev_mth
				Cal::setSel $r 6
			} else { Cal::setSel $r [expr {$c-1}] }
		}
		bind $v::P.can <Key-Right> {
			if {$Cal::v::selA==0 || $Cal::v::selB!=0} { return }
			lassign [lrange $Cal::v::selA 3 4] r c
			if {$c==6} {
				Cal::next_mth
				Cal::setSel $r 0
			} else { Cal::setSel $r [expr {$c+1}] }
		}
		bind $v::P.can <Key-Up> {
			if {$Cal::v::selA==0 || $Cal::v::selB!=0 } { return }
			lassign [lrange $Cal::v::selA 3 4] r c
			Cal::setSel [expr {$r-1}] $c
		}
		bind $v::P.can <Key-Down> {
			if {$Cal::v::selA==0 || $Cal::v::selB!=0 } { return }
			lassign [lrange $Cal::v::selA 3 4] r c
			Cal::setSel [expr {$r+1}] $c
		}
		proc $v::P.get_selected {} {
			# Gets currently selected date span.
			if {$v::selB!=0} { return [list {*}[lrange [split $v::selA] 0 2] {*}[lrange [split $v::selB] 0 2]] }
			if {$v::selA!=0} { return [lrange [split $v::selA] 0 2] }
		}
		cal_base
		cal_day
		return $parent
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
		$v::P.can create rect [set x [expr {$c*$v::SQS+1}]] [set y [expr {$r*$v::SQS+1+$v::hh}]] [incr x $v::SQS] [incr y $v::SQS] -fill #76AF2C -outline #C50080 -tags sel
	}
	proc cal_base {} {
		# Draws base of calender; the day-name header.
		set x 1
		set x2 [expr {6*$v::SQS+1}]
		set y [expr {1+$v::hh}]
		for {set r 0} {$r<6} {incr r} {
			if {$r%2} {
				set f #E266B7
				set f2 #E238A7
			} else {
				set f #E238A7
				set f2 #E266B7
			}
			$v::P.can create rect $x $y [expr {$x+$v::SQS}] [expr {$y+$v::SQS}] -fill $f -outline $f2 -tags {bg sun}
			$v::P.can create rect $x2 $y [expr {$x2+$v::SQS}] [expr {$y+$v::SQS}] -fill $f2 -outline $f -tags {bg sat}
			incr y $v::SQS
		}
		set y [expr 1+$v::hh]
		set x [expr $v::SQS+1]
		for {set r 0} {$r<6} {incr r} {
			for {set c 1} {$c<6} {incr c} {
				if {[expr {($c+$r)%2}]} {
					set t a
					set f #6D89D5
				} else {
					set t b
					set f #476DD5
				}
				$v::P.can create rect $x $y [expr {$x+$v::SQS}] [expr {$y+$v::SQS}] -fill $f -outline $f -tags "bg $t"
				incr x $v::SQS
			}
			incr y $v::SQS
			set x [expr {$v::SQS+1}]
		}
		$v::P.can create rect 1 1 [expr {$v::SQS*7}] $v::hh -fill #062270 -outline #6D89D5
		set x [expr {$v::SQS/2}]
		set y [expr {$v::hh/2}]
		foreach d {SUN MON TUE WED THR FRI SAT} {
			$v::P.can create text $x $y -text $d -font $v::dfont -fill #6D89D5
			incr x $v::SQS
		}
	}
	proc cal_day {} {
		# Draws days of the MTH and YR.
		$v::P.can delete -tag days
		$v::P.mth.mthl configure -text "[lindex $v::MTH_NAME $v::MTH] $v::YR"
		set f [dowMY $v::MTH $v::YR]
		set x [expr {1+$f*$v::SQS+$v::tOff}]
		set y [expr {$v::hh+$v::tOff+1}]
		# Current month's days
		set l [mth_day_ct $v::MTH $v::YR]
		for {set d 1} {$d<=$l} {incr d} {
			if {$d%2} {
				set T odd
			} else {
				set T even
			}
			$v::P.can create text $x $y -text $d -font $v::dfont -fill #062270 -tag "$T days text"
			if {$f==6} {
				set x [expr {$v::tOff+1}]
				set f 0
				incr y $v::SQS
			} else {
				incr f
				incr x $v::SQS
			}
		}
	}
	proc month_range {} {
		# Get current month day year span shown on calendar.
		return [list $v::MTH 1 $v::YR $v::MTH [mth_day_ct $v::MTH $v::YR] $v::YR]
	}
	proc shown_dates {} {
		# Draws selection rectangles in the selected range
		# for the current month.
		$v::P.can delete -tags sel
		if {$v::selA==0} { return }
		lassign $v::selA ma da ya ra ca
		if {$v::selB==0} {
			if {$v::YR==$ya && $v::MTH==$ma} {
				sel_RC $ra $ca
				$v::P.can raise text sel
			}
		} else {
			lassign $v::selB mb db yb rb cb
			if {$mb<$v::MTH || $yb<$v::YR || $ma>$v::MTH || $ya>$v::YR} { return }
			if {$ma<$v::MTH || $ya<$v::YR} {
				set ra 0
				set da 1
				set ca [dowMY $v::MTH $v::YR]
			}
			if {$mb>$v::MTH || $yb>$v::YR} { set db [mth_day_ct $v::MTH $v::YR] }
			while {$da<=$db} {
				sel_RC $ra $ca
				incr da
				if {$ca==6} {
					incr ra
					set ca 0
				} else { incr ca }
			}
			$v::P.can raise text sel
		}
	}
	proc clearSel {} {
		# Clears the selection, both visually and variable wise.
		set v::selA [set v::selB 0]
		shown_dates
	}
	proc rc_date {r c} {
		# Returns the date shown on canvas with row and column.
		set d [expr {$c-[dowMY $v::MTH $v::YR]+7*$r+1}]
		if {$d<1} {
			lassign [mthyr -- $v::MTH $v::YR] m y
			return [list $m [expr {[mth_day_ct $m $y]+$d}] $y]
		}
		if {$d>[set l [mth_day_ct $v::MTH $v::YR]]} {
			lassign [mthyr ++ $v::MTH $v::YR] m y
			return [list $m [expr {$d-$l}] $y]
		}
		return [list $v::MTH $d $v::YR]
	}
	proc dateC {m d y m2 d2 y2} {
		# returns 0 if equal -1 if < and 1 if >
		return [expr {($y<$y2 ? -1 : ($y>$y2 ? 1 : ($m<$m2 ? -1 : ($m>$m2 ? 1 : ($d<$d2 ? -1 : ($d>$d2 ? 1 : 0))))))}]
	}
	proc setSel {r c} {
		# Set the selection rect to the row and column.
		set d [expr {$c-[dowMY $v::MTH $v::YR]+7*$r+1}]
		if {$d>0 && $d<=[mth_day_ct $v::MTH $v::YR]} {
			set v::selB 0
			set v::selA [list $v::MTH $d $v::YR $r $c]
			shown_dates
		}
	}
	proc adSel {r c} {
		# Adjusts the selection rect(s) to expand/shrink to the row and column.
		lassign [rc_date $r $c] m d y
		if {$m!=$v::MTH} { return }
		if {$v::selA==0} { return [setSel $r $c] }
		if {$v::selB==0 || [dateC $m $d $y {*}[lrange $v::selB 0 2]]==1} {
			set v::selB [list $m $d $y $r $c]
		} else { 
			set v::selA [list $m $d $y $r $c]
		}
		if {[dateC {*}[lrange $v::selA 0 2] {*}[lrange $v::selB 0 2]]==1} {
			# Swap selected indices so that selA occurs before selB
			set v::selA $v::selB[set v::selB $v::selA; list]
		}
		shown_dates
	}
	proc rc_canvas {y x} {
		# Given y & x coords, return row & col.
		return [list [expr {int(($y-$v::hh)/$v::SQS)}] [expr {int($x/$v::SQS)}]]
	}
}
proc main {} {
	bind . <Control-Key-q> {
		exit
	}
	set square_size 32
	set text_offset 8
	set day_font {Arial 10}
	set hh 16
	# Init Cal with the current month and year.
	lassign [clock format [clock seconds] -format {%N %Y}] m y
	set f .cal
	Cal::CalVars $m $y $square_size $text_offset $day_font $hh
	Cal::Cal $f
	#$m $y $square_size $text_offset $day_font $hh
	pack $f
	pack [entry .mydate]
	bind $f.can <Key-Return> {
		focus .mydate
	}
	proc put_mydate {f} {
		.mydate delete 0 end
		.mydate insert 0 [Cal::$f.get_selected]
	}
	pack [button .goo -text Gooo -command "put_mydate $f"] -side left
	pack [button .bquit -text Quit -command exit] -side right
}
if {[string match *tcal.tcl $argv0]} {
	main
}
