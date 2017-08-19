#!/usr/bin/tclsh8.6
package require Tk
namespace eval Cal {
	variable days
	variable mth
	variable c_mth
	variable c_yr
	variable selA
	variable selB
	array set days { Sun {0} Mon {1} Tue {2} Wed {3} Thu {4} Fri {5} Sat {6} }
	set mth {31 31 28 31 30 31 30 31 31 30 31 30 31}
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
		CL::cal_base
		CL::cal_day
	}
	proc nmth {} {
		variable c_mth
		incr c_mth
		if {$c_mth==13} {
			set c_mth 1
			variable c_yr
			incr c_yr
		}
	}
	proc pmth {} {
		variable c_mth
		incr c_mth -1
		if {$c_mth==0} {
			set c_mth 12
			variable c_yr
			incr c_yr -1
		}
	}
}
namespace eval CL {
	variable imth
	set imth {Null January Febuary March April May June July August September October November December}
	variable SQS
	set SQS 64
	proc sel_RC {r c} {
		sel_SQ [expr $c*$CL::SQS] [expr $r*$CL::SQS]
	}
	proc sel_YX {y x} {
		.fcal.can create rect [expr $x+1] [expr $y+1] [expr $x+$SQS] [expr $y+$SQS] -outline red -tags sel
	}
	proc cal_base {} {
		.fcal.can create rect 1 1 [expr $CL::SQS*7] $CL::SQS -fill #000
		set x [expr $CL::SQS/2]
		set y $x
		foreach d {S M T W T F S} {
			.fcal.can create text $x $y -text $d -fill #fff
			incr x $CL::SQS
		}
	}
	proc prev_mth {} {
		Cal::pmth
		cal_day
	}
	proc next_mth {} {
		Cal::nmth
		cal_day
	}
	proc cal_day {} {
		.fcal.can delete -tag days
		set m $Cal::c_mth
		set y $Cal::c_yr
		.fcal.mth.mthl configure -text "[lindex $CL::imth $Cal::c_mth] $Cal::c_yr"
		set f [Cal::dowMY $m $y]
		set x 1
		set y [expr $CL::SQS+1]
		if {$f>0} { # past month days
			if {$m==3 && [Cal::is_lpyr $y]} {
				set l 29
				set d [expr 29-$f]
			} else {
				set l [lindex $Cal::mth [expr $m-1]]
				set d [expr $l+1-$f]
			}
			while {$d<=$l} {
				.fcal.can create rect $x $y [expr $x+$CL::SQS] [expr $y+$CL::SQS] -fill #eee -outline #ddd -tag days
				.fcal.can create text [expr $x+$CL::SQS/2] [expr $y+$CL::SQS/2] -text $d -fill #666 -tag days
				incr x $CL::SQS
				incr d
			}
		}
		set d 1
		set l [expr ($m==2 && [Cal::is_lpyr $y]) ? 29 : [lindex $Cal::mth $m]]
		while {$d<=$l} {
			set c [expr ($d % 2==0)?"#ccc":"#ddd"]
			.fcal.can create rect $x $y [expr $x+$CL::SQS] [expr $y+$CL::SQS] -fill $c -outline $c -tag days
			.fcal.can create text [expr $x+$CL::SQS/2] [expr $y+$CL::SQS/2] -text $d -tag days
			incr f
			incr x $CL::SQS
			incr d
			if {$f==7} {
				set x 1
				set f 0
				incr y $CL::SQS
			}
		}
		set d 1
		while {$f<7} {
			.fcal.can create rect $x $y [expr $x+$CL::SQS] [expr $y+$CL::SQS] -fill #eee -outline #ddd -tag days
			.fcal.can create text [expr $x+$CL::SQS/2] [expr $y+$CL::SQS/2] -text $d -fill #666 -tag days
			incr f
			incr x $CL::SQS
			incr d
		}
		if {$y!=[expr 6*$CL::SQS]} {
			incr y $CL::SQS
			set f 0
			set x 1
			while {$f<7} {
				.fcal.can create rect $x $y [expr $x+$CL::SQS] [expr $y+$CL::SQS] -fill #eee -outline #ddd -tag days
				.fcal.can create text [expr $x+$CL::SQS/2] [expr $y+$CL::SQS/2] -text $d -fill #666 -tag days
				incr f
				incr x $CL::SQS
				incr d
			}
		}
	}
	proc cal_days {m y} {
		set f [Cal::dowMY $m $y]
		if {$m==1} {
			set l 31
			set d [expr 32-$f]
		} elseif {$m==2 && [Cal::is_lpyr $y]} {
			set l 29
			set d [expr 29-$f]
		} else {
			set l [lindex $Cal::mth $f]
			set d [expr $l+1-$f]
		}
		set x 1
		set y [expr $CL::SQS+1]
		set w 0
		while {$d<=$l} {
			.fcal.can create text [expr $x+$CL::SQS/2] [expr $y+$CL::SQS/2] -text $d -fill #eee -tag days
			incr w
			incr x $CL::SQS
			incr d
			if {$w==7} {
				set x 1
				set w 0
				incr y $CL::SQS
			}
		}
		if {$m==2 && [Cal::is_lpyr $y]} {
			set l 29
		} else {
			set l [lindex $Cal::mth [expr $m+1]]
		}
		set d 1
		while {$d<=$l} {
			set c [expr ($d % 2==1)?"white":"#eee"]
			.fcal.can create rect $x $y [expr $x+$CL::SQS] [expr $y+$CL::SQS] -fill $c -outline $c -tag days
			.fcal.can create text [expr $x+$CL::SQS/2] [expr $y+$CL::SQS/2] -text $d -tag days
			incr w
			incr x $CL::SQS
			incr d
			if {$w==7} {
				set x 1
				set w 0
				incr y $CL::SQS
			}
		}
		set d 1
		while {$w<=7} {
			.fcal.can create text [expr $x+$CL::SQS/2] [expr $y+$CL::SQS/2] -text $d -fill #eee -tag days
			incr w
			incr x $CL::SQS
			incr d
		}
	}
	proc tiledown {N sq} {
		set y 1
		for {set n 0} {$n<=$N} {incr n} {
			set col [expr ($n % 2==1)?"red":"blue"]
			.fcal.can create rect 1 $y [expr 1+$sq] [expr $y+$sq-1] -outline $col -tag chk
			incr y $sq
		}
	}
	proc tileright {N sq} {
		set x 1
		for {set n 0} {$n<=$N} {incr n} {
			set col [expr ($n % 2==1)?"red":"blue"]
			.fcal.can create rect $x 25 [expr $x+$sq-1] [expr 25+$sq-1] -outline $col -tag chk
			incr x $sq
		}
	}

}
frame .fcal
frame .fcal.mth
button .fcal.mth.prv -text < -command CL::prev_mth
button .fcal.mth.nxt -text > -command CL::next_mth
label .fcal.mth.mthl -text NULL
canvas .fcal.can -width [expr 7*$CL::SQS] -height [expr 7*$CL::SQS] -bg white
button .bquit -text Quit -command exit



#bind .fcal.can <ButtonPress-1> {
	#if {[set r [expr int([%W canvasy %y]/24)]]<1} { return }
	#set c [expr int([%W canvasx %x]/24)]
	#set bset "$r $c"
	#.fcal.can delete -tags sel
	#sel_at $r $c
#}
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
grid .fcal.mth.prv -column 0 -row 0
grid .fcal.mth.mthl -column 1 -row 0
grid .fcal.mth.nxt -column 2 -row 0
grid .fcal.mth
grid .fcal.can
grid .fcal
grid .bquit

proc main {} {
	lassign [clock format [clock seconds] -format "%N %Y"] m y
	puts $m
	Cal::draw_cal $m $y
	#CL::cal_base
	#CL::cal_day $m $y
}
main
