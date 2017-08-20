#!/usr/bin/env tclsh8.6
package require Tk
namespace eval Cal {
	array set days { Sun {0} Mon {1} Tue {2} Wed {3} Thu {4} Fri {5} Sat {6} }
	variable SQS 64 mth {31 31 28 31 30 31 30 31 31 30 31 30 31} imth {Null January Febuary March April May June July August September October November December}
	variable c_mth
	variable c_yr
	variable selA
	variable selB
	proc dowMY {m y} {
		set w [clock format [clock scan $m-01-$y -format %m-%d-%Y] -format %a]
		return $Cal::days($w)
	}
	proc is_lpyr {y} {
		return [expr $y%4==0 && ($y%100!=0 || $y%400==0)]
	}
	proc draw_cal { m y } {
		variable c_mth
		variable c_yr
		set c_mth $m
		set c_yr $y
		cal_base
		cal_day
	}
	proc prev_mth {} {
		variable c_mth
		incr c_mth -1
		if {$c_mth==0} {
			set c_mth 12
			variable c_yr
			incr c_yr -1
		}
		cal_day
	}
	proc next_mth {} {
		variable c_mth
		incr c_mth
		if {$c_mth==13} {
			set c_mth 1
			variable c_yr
			incr c_yr
		}
		cal_day
	}
	proc date_at {r c} {
		variable c_mth
		variable c_yr
		set c [Cal::dowMY $c_mth $c_yr]

	}
	proc sel_RC {r c} {
		sel_YX [expr $r*$Cal::SQS] [expr $c*$Cal::SQS]
	}
	proc sel_YX {y x} {
		.fcal.can create rect [expr $x+1] [expr $y+1] [expr $x+$Cal::SQS] [expr $y+$Cal::SQS] -outline #666 -tags sel
	}
	proc cal_base {} {
		.fcal.can create rect 1 1 [expr $Cal::SQS*7] $Cal::SQS -fill #000
		set x [expr $Cal::SQS/2]
		set y $x
		foreach d {S M T W T F S} {
			.fcal.can create text $x $y -text $d -fill #fff
			incr x $Cal::SQS
		}
	}
	proc cal_day {} {
		.fcal.can delete -tag days
		set m $Cal::c_mth
		set y $Cal::c_yr
		.fcal.mth.mthl configure -text "[lindex $Cal::imth $Cal::c_mth] $Cal::c_yr"
		set f [Cal::dowMY $m $y]
		set x 1
		set y [expr $Cal::SQS+1]
		if {$f>0} { # past month days
			if {$m==3 && [Cal::is_lpyr $y]} {
				set l 29
				set d [expr 29-$f]
			} else {
				set l [lindex $Cal::mth [expr $m-1]]
				set d [expr $l+1-$f]
			}
			while {$d<=$l} {
				.fcal.can create rect $x $y [expr $x+$Cal::SQS] [expr $y+$Cal::SQS] -fill #eee -outline #ddd -tag days
				.fcal.can create text [expr $x+$Cal::SQS/2] [expr $y+$Cal::SQS/2] -text $d -fill #666 -tag days
				incr x $Cal::SQS
				incr d
			}
		}
		set d 1
		set l [expr ($m==2 && [Cal::is_lpyr $y]) ? 29 : [lindex $Cal::mth $m]]
		while {$d<=$l} {
			set c [expr ($d % 2==0)?"#ccc":"#ddd"]
			.fcal.can create rect $x $y [expr $x+$Cal::SQS] [expr $y+$Cal::SQS] -fill $c -outline $c -tag days
			.fcal.can create text [expr $x+$Cal::SQS/2] [expr $y+$Cal::SQS/2] -text $d -tag days
			incr f
			incr x $Cal::SQS
			incr d
			if {$f==7} {
				set x 1
				set f 0
				incr y $Cal::SQS
			}
		}
		set d 1
		while {$f<7} {
			.fcal.can create rect $x $y [expr $x+$Cal::SQS] [expr $y+$Cal::SQS] -fill #eee -outline #ddd -tag days
			.fcal.can create text [expr $x+$Cal::SQS/2] [expr $y+$Cal::SQS/2] -text $d -fill #666 -tag days
			incr f
			incr x $Cal::SQS
			incr d
		}
		if {$y!=[expr 6*$Cal::SQS]} {
			incr y $Cal::SQS
			set f 0
			set x 1
			while {$f<7} {
				.fcal.can create rect $x $y [expr $x+$Cal::SQS] [expr $y+$Cal::SQS] -fill #eee -outline #ddd -tag days
				.fcal.can create text [expr $x+$Cal::SQS/2] [expr $y+$Cal::SQS/2] -text $d -fill #666 -tag days
				incr f
				incr x $Cal::SQS
				incr d
			}
		}
	}
	proc rc_date {r c} {
		variable c_mth
		variable c_yr
		set l [lindex $Cal::mth $c_mth]
		set f [dowMY $c_mth $c_yr]
		set d [expr {$c-$f+7*$r-6}]
		if {$d<1} {
			if {$c_mth==3 && [is_lpyr $c_yr]} { return [format "2 %d %d" [expr {29+$d}] $c_yr]
			} elseif {$c_mth==1} { return [format "1 %d %d" [expr {31+$d}] [expr {$c_yr-1}]]
			} else { return [format "%d %d %d" [expr {$c_mth-1}] [expr {[lindex $Cal::mth [expr {$c_mth-1}]]+$d}] $c_yr] }
		} elseif {$d>$l} {
			if {$c_mth==12} { return [format "1 %d %d" [expr {$l-$d}] [expr {$c_yr+1}]]
			} else { return [format "%d %d %d" [expr {$c_mth+1}] [expr {$d-$l}] $c_yr] }
		} else { return [format "%d %d %d" $c_mth $d $c_yr] }
	}
	proc setSel {r c} {
		variable selB 0 selA [rc_date $r $c]
		sel_RC $r $c
		.date_range delete 0 end
		#.date_range insert 0 "$r $c |"
		.date_range insert end $Cal::selA
	}
}
frame .fcal
frame .fcal.mth
button .fcal.mth.prv -text < -command Cal::prev_mth
button .fcal.mth.nxt -text > -command Cal::next_mth
label .fcal.mth.mthl -text NULL
canvas .fcal.can -width [expr 7*$Cal::SQS] -height [expr 7*$Cal::SQS] -bg white
button .bquit -text Quit -command exit
entry .date_range
grid .fcal.mth.prv -column 0 -row 0
grid .fcal.mth.mthl -column 1 -row 0
grid .fcal.mth.nxt -column 2 -row 0
grid .fcal.mth
grid .fcal.can
grid .fcal
grid .date_range
grid .bquit

bind .fcal.can <ButtonPress-1> {
	if {[set r [expr int([%W canvasy %y]/$Cal::SQS)]]<1} { return }
	set c [expr int([%W canvasx %x]/$Cal::SQS)]
	.fcal.can delete -tags sel
	Cal::setSel $r $c
}
#bind .fcal.can <Shift-ButtonPress-1> {
	#if {[set r [expr int([%W canvasy %y]/24)]]<1} { return }
	#set c [expr int([%W canvasx %x]/24)]
	#.fcal.can delete -tags sel
	#if {$bset=="false"} {
		#set bset "$r $c"
		#set x [expr $c*24+1]
		#set y [expr $r*24+1]
		#sel_24 $y $x
	#} else {
		#set r2 [lindex $bset 0]
		#set c2 [lindex $bset 1]
		#set bset "$r $c"
		#if {$r2<$r} {
			#set rt $r2
			#set ct $c2
			#set r2 $r
			#set c2 $c
			#set r $rt
			#set c $ct
		#}
		#set x [expr $c*24+1]
		#set y [expr $r*24+1]
		#while {$r<$r2} {
			#while {$c<7} {
				#sel_24 $y $x
				#incr c
				#incr x 24
			#}
			#set c 0
			#set x 0
			#incr r
			#incr y 24
		#}
		#while {$c<=$c2} {
			#sel_24 $y $x
			#incr c
			#incr x 24
		#}
	#}
#}

proc main {} {
	lassign [clock format [clock seconds] -format "%N %Y"] m y
	Cal::draw_cal $m $y
}
main
