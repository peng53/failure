#!/usr/bin/env tclsh8.6
package require Tk
package provide Tcal 1.0
namespace eval Cal {
	namespace eval v {
		variable MTH_DAYS [list -1 31 28 31 30 31 30 31 31 30 31 30 31]
		variable MTH_NAME [list NULL January Febuary March April May June July August September October November December]
		variable MTH 1 YR 2000
		variable selA 0 selB 0
		variable SQS 26 tOffy 0.5 tOffx 0.5 hh 14 dfont {Arial 10}
		variable sy 26
		variable clr
		array set clr { 
			SELbg #76AF2C SELbd #C50080
			DAYs1 #E266B7 DAYs2 #E238A7
			DAYc1 #6D89D5 DAYc2 #476DD5 DAYtc #062270
			HDbg #062270 HDbd #6D89D5 HDtc #6D89D5
		}
	}
	proc is_lpyr {y} {
		# Returns whether a year is a leap-year.
		return [expr {(($y%4==0) && ($y%100!=0 || $y%400==0))}]
	}
	proc mthyr {sub m y} {
		# Calls subcommand with mth and year
		switch $sub {
			-- {
			# decr month
				if {$m==1} { return [list 12 [expr {$y-1}]] }
				return [list [expr {$m-1}] $y]
			}
			++ {
			# incr month
				if {$m==12} { return [list 1 [expr {$y+1}]] }
				return [list [expr {$m+1}] $y]
			}
			ct {
			# days in month
				return [expr {($m==2 && [is_lpyr $y]) ? 29 : [lindex $v::MTH_DAYS $m]}]
			}
			1st {
			# returns day-of-week for the first day-of-the-month
				return [clock format [clock scan $m-01-$y -format %m-%d-%Y] -format %w]
			}
			set {
			# set mth & year for Cal::v if valid
				if {$m<1 || $m>12 | $y<1 || $y>9999} { return }
				set v::MTH $m
				set v::YR $y
			}
		}
	}
	proc CalClrs {SELbg SELbd DAYs1 DAYs2 DAYc1 DAYc2 DAYtc HDbg HDbd HDtC} {
		# Changes the colors
		variable clr
		array set clr {
			SELbg $SELbg SELbd $SELbd
			DAYs1 $DAYs1 DAYs2 $DAYs2
			DAYc1 $DAYc1 DAYc2 $DAYc2 DAYtc $DAYtc
			HDbg $HDbg HDbd $HDbd HDtC
		}
	}
	proc CalVars {mth yr sy sx toffy toffx dfont hh} {
		# Sets vars for Cal
		mthyr set $mth $yr
		foreach {x y} [list SQS $sx sy $sy tOffy $toffy tOffx $toffx hh $hh dfont $dfont] {
			set v::$x $y
		}
	}
	proc Cal {parent} {
		# Creates Cal widget
		set v::P $parent
		frame $parent
		pack [frame $parent.mth] -expand 1 -fill x
		pack [button $parent.mth.prv -text < -command "Cal::prev_mth $parent"] -side left
		pack [label $parent.mth.mthl] -side left -expand 1 -fill x
		pack [button $parent.mth.nxt -text > -command "Cal::next_mth $parent"] -side left
		pack [canvas $parent.can -width [expr 7*$v::SQS] -height [expr $v::hh+6*$v::sy]]
		bind $parent.can <ButtonPress-1> {
			# Selects a rect (a day) on the canvas (calender).
			focus %W
			set C [winfo parent %W]
			if {[set y [%W canvasy %y]]>$Cal::v::hh} { Cal::setSel $C {*}[Cal::rc_canvas $C $y [%W canvasx %x]]}
		}
		bind $parent.can <Shift-ButtonPress-1> {
			# Adds rect(s) (day(s)) to (previous) selection.
			set C [winfo parent %W]
			if {[set y [%W canvasy %y]]>$Cal::v::hh} { Cal::adSel $C {*}[Cal::rc_canvas $C $y [%W canvasx %x]]}
		}
		bind $parent.can <ButtonPress-3> {
			Cal::clearSel [winfo parent %W]
		}
		bind $parent.can <Control-Key-Left> {
			Cal::prev_mth [winfo parent %W]
		}
		bind $parent.can <Control-Key-Right> {
			Cal::next_mth [winfo parent %W]
		}
		bind $parent.can <Control-Key-Up> {
			set C [winfo parent %W]
			Cal::mthyr set $Cal::v::MTH [expr {$Cal::v::YR-1}]
			Cal::cal_day $C
		}
		bind $parent.can <Control-Key-Down> {
			set C [winfo parent %W]
			Cal::mthyr set $Cal::v::MTH [expr {$Cal::v::YR+1}]
			Cal::cal_day $C
		}
		bind $parent.can <Key-Left> {
			set C [winfo parent %W]
			if {$Cal::v::selA==0 || $Cal::v::selB!=0} { return }
			lassign [lrange $Cal::v::selA 3 4] r c
			if {$c==0} {
				Cal::prev_mth $C
				Cal::setSel $C $r 6
			} else { Cal::setSel $C $r [expr {$c-1}] }
		}
		bind $parent.can <Key-Right> {
			set C [winfo parent %W]
			if {$Cal::v::selA==0 || $Cal::v::selB!=0} { return }
			lassign [lrange $Cal::v::selA 3 4] r c
			if {$c==6} {
				Cal::next_mth $C
				Cal::setSel $C $r 0
			} else { Cal::setSel $C $r [expr {$c+1}] }
		}
		bind $parent.can <Key-Up> {
			if {$Cal::v::selA==0 || $Cal::v::selB!=0 } { return }
			set C [winfo parent %W]
			lassign [lrange $Cal::v::selA 3 4] r c
			Cal::setSel $C [expr {$r-1}] $c
		}
		bind $parent.can <Key-Down> {
			if {$Cal::v::selA==0 || $Cal::v::selB!=0 } { return }
			set C [winfo parent %W]
			lassign [lrange $Cal::v::selA 3 4] r c
			Cal::setSel $C [expr {$r+1}] $c
		}
		proc $parent.get_selected {} {
			# Gets currently selected date span.
			if {$v::selB!=0} { return [list {*}[lrange [split $v::selA] 0 2] {*}[lrange [split $v::selB] 0 2]] }
			if {$v::selA!=0} { return [lrange [split $v::selA] 0 2] }
		}
		cal_base $parent
		cal_day $parent
		return $parent
	}
	proc prev_mth {C} {
		# Set current shown month back by 1 and redraws.
		if {$v::MTH==1} {
			set v::MTH 12
			incr v::YR -1
		} else { incr v::MTH -1 }
		cal_day $C
		shown_dates $C
	}
	proc next_mth {C} {
		# Set current shown month forward by 1 and redraws.
		if {$v::MTH==12} {
			set v::MTH 1
			incr v::YR
		} else { incr v::MTH }
		cal_day $C
		shown_dates $C
	}
	proc sel_RC {C r c} {
		# Draw selection rect at row and column.
		$C.can create rect [set x [expr {$c*$v::SQS+1}]] [set y [expr {$r*$v::sy+1+$v::hh}]] [incr x $v::SQS] [incr y $v::sy] -fill $v::clr(SELbg) -outline $v::clr(SELbd) -tags sel
	}
	proc cal_base {C} {
		# Draws base of calender; the day-name header.
		set x 1
		set x2 [expr {6*$v::SQS+1}]
		set y [expr {1+$v::hh}]
		for {set r 0} {$r<6} {incr r} {
			if {$r%2} {
				set f $v::clr(DAYs1)
				set f2 $v::clr(DAYs2)
			} else {
				set f $v::clr(DAYs2)
				set f2 $v::clr(DAYs1)
			}
			$C.can create rect $x $y [expr {$x+$v::SQS}] [expr {$y+$v::sy}] -fill $f -outline $f2 -tags {bg sun}
			$C.can create rect $x2 $y [expr {$x2+$v::SQS}] [expr {$y+$v::sy}] -fill $f2 -outline $f -tags {bg sat}
			incr y $v::sy
		}
		set y [expr 1+$v::hh]
		set x [expr $v::SQS+1]
		for {set r 0} {$r<6} {incr r} {
			for {set c 1} {$c<6} {incr c} {
				if {[expr {($c+$r)%2}]} {
					set t a
					set f $v::clr(DAYc1)
				} else {
					set t b
					set f $v::clr(DAYc2)
				}
				$C.can create rect $x $y [expr {$x+$v::SQS}] [expr {$y+$v::sy}] -fill $f -outline $f -tags "bg $t"
				incr x $v::SQS
			}
			incr y $v::sy
			set x [expr {$v::SQS+1}]
		}
		$C.can create rect 1 1 [expr {$v::SQS*7+2}] $v::hh -fill $v::clr(HDbg) -outline $v::clr(HDbd)
		set x [expr {$v::SQS/2}]
		set y [expr {$v::hh/2}]
		foreach d {SUN MON TUE WED THR FRI SAT} {
			$C.can create text $x $y -text $d -font $v::dfont -fill $v::clr(HDtc)
			incr x $v::SQS
		}
	}
	proc cal_day {C} {
		# Draws days of the MTH and YR.
		$C.can delete -tag days
		$C.mth.mthl configure -text "[lindex $v::MTH_NAME $v::MTH] $v::YR"
		set f [mthyr 1st $v::MTH $v::YR]
		set x [expr {1+$f*$v::SQS+int($v::tOffx*$v::SQS)}]
		set y [expr {$v::hh+int($v::tOffy*$v::sy)+1}]
		# Current month's days
		set l [mthyr ct $v::MTH $v::YR]
		for {set d 1} {$d<=$l} {incr d} {
			if {$d%2} {
				set T odd
			} else {
				set T even
			}
			$C.can create text $x $y -text $d -font $v::dfont -fill $v::clr(DAYtc) -tag "$T days text"
			if {$f==6} {
				set x [expr {int($v::tOffx*$v::SQS)+1}]
				set f 0
				incr y $v::sy
			} else {
				incr f
				incr x $v::SQS
			}
		}
	}
	proc month_range {C} {
		# Get current month day year span shown on calendar.
		return [list $v::MTH 1 $v::YR $v::MTH [mthyr ct $v::MTH $v::YR] $v::YR]
	}
	proc shown_dates {C} {
		# Draws selection rectangles in the selected range
		# for the current month.
		$C.can delete -tags sel
		$C.can delete -tags mark
		if {$v::selA==0} { return }
		lassign $v::selA ma da ya ra ca
		if {$v::selB==0} {
			if {$v::YR==$ya && $v::MTH==$ma} {
				sel_RC $C $ra $ca
				$C.can raise text sel
			}
		} else {
			lassign $v::selB mb db yb rb cb
			if {$mb<$v::MTH || $yb<$v::YR || $ma>$v::MTH || $ya>$v::YR} { return }
			if {$ma<$v::MTH || $ya<$v::YR} {
				set ra 0
				set da 1
				set ca [mthyr 1st $v::MTH $v::YR]
			}
			if {$mb>$v::MTH || $yb>$v::YR} { set db [mthyr ct $v::MTH $v::YR] }
			while {$da<=$db} {
				sel_RC $C $ra $ca
				incr da
				if {$ca==6} {
					incr ra
					set ca 0
				} else { incr ca }
			}
			$C.can raise text sel
		}
	}
	proc mark_days {C days sym ox oy col} {
		# Marks days on the current month with sym
		$C.can delete -tags mark
		set f [mthyr 1st $v::MTH $v::YR]
		set x [expr {1+$f*$v::SQS+int($ox*$v::SQS)}]
		set y [expr {$v::hh+int($oy*$v::sy)+1}]
		set n 1
		foreach d $days {
			while {$n<$d} {
				if {$f==6} {
					set x [expr {int($ox*$v::SQS)+1}]
					set f 0
					incr y $v::sy
				} else {
					incr f
					incr x $v::SQS
				}
				incr n
			}
			if {$n==$d || "0$n"==$d} {
				$C.can create text $x $y -text $sym -font $v::dfont -fill $col -tag mark
			}
		}
	}
	proc clearSel {C} {
		# Clears the selection, both visually and variable wise.
		set v::selA [set v::selB 0]
		shown_dates $C
	}
	proc rc_date {C r c} {
		# Returns the date shown on canvas with row and column.
		set d [expr {$c-[mthyr 1st $v::MTH $v::YR]+7*$r+1}]
		if {$d<1} {
			lassign [mthyr -- $v::MTH $v::YR] m y
			return [list $m [expr {[mthyr ct $m $y]+$d}] $y]
		}
		if {$d>[set l [mthyr ct $v::MTH $v::YR]]} {
			lassign [mthyr ++ $v::MTH $v::YR] m y
			return [list $m [expr {$d-$l}] $y]
		}
		return [list $v::MTH $d $v::YR]
	}
	proc dateC {m d y m2 d2 y2} {
		# returns 0 if equal -1 if < and 1 if >
		return [expr {($y<$y2 ? -1 : ($y>$y2 ? 1 : ($m<$m2 ? -1 : ($m>$m2 ? 1 : ($d<$d2 ? -1 : ($d>$d2 ? 1 : 0))))))}]
	}
	proc setSel {C r c} {
		# Set the selection rect to the row and column.
		set d [expr {$c-[mthyr 1st $v::MTH $v::YR]+7*$r+1}]
		if {$d>0 && $d<=[mthyr ct $v::MTH $v::YR]} {
			set v::selB 0
			set v::selA [list $v::MTH $d $v::YR $r $c]
			shown_dates $C
		}
	}
	proc adSel {C r c} {
		# Adjusts the selection rect(s) to expand/shrink to the row and column.
		lassign [rc_date $C $r $c] m d y
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
		shown_dates $C
	}
	proc rc_canvas {C y x} {
		# Given y & x coords, return row & col.
		return [list [expr {int(($y-$v::hh)/$v::sy)}] [expr {int($x/$v::SQS)}]]
	}
}
if {[string match *tcal.tcl $argv0]} {
	bind . <Control-Key-q> {
		exit
	}
	set square_w 64
	set square_h 48
	set text_offset_y 0.2
	set text_offset_x 0.8
	set day_font {Arial 10}
	set hh 16
	# Init Cal with the current month and year.
	lassign [clock format [clock seconds] -format {%N %Y}] m y
	Cal::CalVars $m $y $square_h $square_w $text_offset_y $text_offset_x $day_font $hh
	pack [Cal::Cal .cal]
	pack [entry .mydate]
	bind .cal.can <Key-Return> {
		focus .mydate
	}
	proc put_mydate {f} {
		.mydate delete 0 end
		.mydate insert 0 [Cal::.cal.get_selected]
	}
	pack [button .goo -text Gooo -command "put_mydate .cal"] -side left
	pack [button .bquit -text Quit -command exit] -side right
}
