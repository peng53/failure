
namespace eval Cal {
	variable days
	variable mth
	array set days {
		Sun {0}
		Mon {1}
		Tue {2}
		Wed {3}
		Thu {4}
		Fri {5}
		Sat {6}
	}
	set mth {31 28 31 30 31 30 31 31 30 31 30 31}
	proc dowMY {m y} {
		set w [clock format [clock scan $m-01-$y -format %m-%d-%Y] -format %a]
		return $Cal::days($w)
	}
	proc is_lpyr {y} {
		return [expr $y%4==0 && ($y%100!=0 || $y%400==0)]
	}
}

#set x [expr 37+12*($first)]
#set y [expr 25+12]
#set nr $first
#for {set d 1} {$d<=31} {incr d} {
	#set c [expr ($d % 2==1)?"white":"#CCCCCC"]
	#.fcal.can create rect [expr $x-12] [expr $y-12] [expr $x+12] [expr $y+12] -fill $c -outline $c
	#.fcal.can create text $x $y -text $d
	#incr x 24
	#incr nr
	#if {$nr==7} {
		#set x 13
		#set nr 0
		#incr y 24
	#}
#}
#set today [clock format [clock seconds] -format {%a %B %Y %m %d}]
#set dow $days([lindex $today 0])
#set first [expr ($days([lindex $today 0])-[lindex $today 4]+1)%7]
#set cur_yr [lindex $today 2]
#proc is_lpyr {yr} {
	## python to tcl
	#return [expr [expr $yr%4]==0 && [expr $yr%100]!=0 || [expr $yr%400]==0]
#}
frame .fcal
frame .fcal.mth
button .fcal.mth.prv -text <
button .fcal.mth.nxt -text >
label .fcal.mth.mthl -text NULL
canvas .fcal.can -width 170 -height 142 -bg white
set bset false
proc sel_at {r c} {
	set x [expr $c*24]
	set y [expr $r*24]
	sel_24 $y $x
	#.fcal.can create rect $x $y [expr $x+24] [expr $y+24] -outline red -tags sel
}
proc sel_24 {y x} {
	.fcal.can create rect $x $y [expr $x+24] [expr $y+24] -outline red -tags sel
}
bind .fcal.can <ButtonPress-1> {
	if {[set r [expr int([%W canvasy %y]/24)]]<1} { return }
	#if {$r<1} { return }
	set c [expr int([%W canvasx %x]/24)]
	set bset "$r $c"
	.fcal.can delete -tags sel
	sel_at $r $c
}
bind .fcal.can <Shift-ButtonPress-1> {
	if {[set r [expr int([%W canvasy %y]/24)]]<1} { return }
	#if {$r<1} { return }
	set c [expr int([%W canvasx %x]/24)]
	.fcal.can delete -tags sel
	if {$bset=="false"} {
		set bset "$r $c"
		set x [expr $c*24]
		set y [expr $r*24]
		sel_24 $y $x
		#sel_at $r $c
	} else {
		set r2 [lindex $bset 0]
		set c2 [lindex $bset 1]
		set bset "$r $c"
		if {$r2<$r} {
			set rt $r2
			set ct $c2
			set r2 $r
			set c2 $c
			set r $rt
			set c $ct
		}
		set x [expr $c*24]
		set y [expr $r*24]
		while {$r<$r2} {
			while {$c<7} {
				sel_24 $y $x
				#sel_at $r $c
				incr c
				incr x 24
			}
			set c 0
			set x 0
			incr r
			incr y 24
		}
		while {$c<=$c2} {
			#sel_at $r $c
			sel_24 $y $x
			incr c
			incr x 24
		}
	}
}
grid .fcal.mth.prv -column 0 -row 0
grid .fcal.mth.mthl -column 1 -row 0
grid .fcal.mth.nxt -column 2 -row 0
grid .fcal.mth
grid .fcal.can
grid .fcal

proc cal_base {} {
	.fcal.can create rect 1 1 170 24 -fill pink -outline blue
	set x 13
	set y 13
	foreach d {S M T W T F S} {
		.fcal.can create text $x $y -text $d
		incr x 24
	}
}
proc cal_days {m y} {
	set f [Cal::dowMY $m $y]
	set x 1
	set y 25
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
		.fcal.can create rect $x $y [expr $x+24] [expr $y+24] -outline #fff -tag days
		.fcal.can create text [expr $x+12] [expr $y+12] -text $d -fill #ccc
		incr nr
		incr x 24
		incr d
		if {$nr==7} {
			set x 1
			set nr 0
			incr y 24
		}
	}
	if {$m==2 && [Cal::is_lpyr $y]} {
		set l 29
	} else {
		set l [lindex $Cal::mth [expr $m+1]]
	}
	set d 1
	while {$d<=$l} {
		set c [expr ($d % 2==1)?"white":"#CCCCCC"]
		.fcal.can create rect $x $y [expr $x+24] [expr $y+24] -fill $c -outline $c -tag days
		.fcal.can create text [expr $x+12] [expr $y+12] -text $d
		incr nr
		incr x 24
		incr d
		if {$nr==7} {
			set x 1
			set nr 0
			incr y 24
		}
	}
	set d 1
	while {$nr<=7} {
		.fcal.can create rect $x $y [expr $x+24] [expr $y+24] -outline #fff -tag days
		.fcal.can create text [expr $x+12] [expr $y+12] -text $d -fill #ccc
		incr nr
		incr x 24
		incr d
	}
}

#for {set d 1} {$d<=31} {incr d} {
	#set c [expr ($d % 2==1)?"white":"#CCCCCC"]
	#.fcal.can create rect [expr $x-12] [expr $y-12] [expr $x+12] [expr $y+12] -fill $c -outline $c
	#.fcal.can create text $x $y -text $d
	#incr x 24
	#incr nr
	#if {$nr==7} {
		#set x 13
		#set nr 0
		#incr y 24
	#}
#}


proc main {} {
	cal_base
	cal_days 2 2017
}
main
