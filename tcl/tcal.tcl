#!/usr/bin/wish

namespace eval Cal {
	variable days
	variable mth
	array set days { Sun {0} Mon {1} Tue {2} Wed {3} Thu {4} Fri {5} Sat {6} }
	set mth {31 28 31 30 31 30 31 31 30 31 30 31}
	proc dowMY {m y} {
		set w [clock format [clock scan $m-01-$y -format %m-%d-%Y] -format %a]
		return $Cal::days($w)
	}
	proc is_lpyr {y} {
		return [expr $y%4==0 && ($y%100!=0 || $y%400==0)]
	}
}
namespace eval CL {
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
	proc cal_days {m y} {
		set f [Cal::dowMY $m $y]
		set x 1
		set y [expr $CL::SQS+1]
		set p_f [expr $f-1]
		if {$m==1} {
			set d [expr 32-$f]
			set l 31
		} elseif {$m==2 && [Cal::is_lpyr $y]} {
			set d [expr 29-$f]
			set l 29
		} else {
			set l [lindex $Cal::mth $f]
			set d [expr $l+1-$f]
		}
		set nr 0
		while {$d<=$l} {
			.fcal.can create text [expr $x+$CL::SQS/2] [expr $y+$CL::SQS/2] -text $d -fill #eee -tag days
			incr nr
			incr x $CL::SQS
			incr d
			if {$nr==7} {
				set x 1
				set nr 0
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
			incr nr
			incr x $CL::SQS
			incr d
			if {$nr==7} {
				set x 1
				set nr 0
				incr y $CL::SQS
			}
		}
		set d 1
		while {$nr<=7} {
			.fcal.can create text [expr $x+$CL::SQS/2] [expr $y+$CL::SQS/2] -text $d -fill #eee -tag days
			incr nr
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
button .fcal.mth.prv -text <
button .fcal.mth.nxt -text >
label .fcal.mth.mthl -text NULL
canvas .fcal.can -width [expr 7*$CL::SQS] -height [expr 6*$CL::SQS] -bg white
set bset false


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


proc main {} {
	CL::cal_base
	CL::cal_days 2 2017
}
main
