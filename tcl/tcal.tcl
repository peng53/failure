
array set days {
	Sun {0}
	Mon {1}
	Tue {2}
	Wed {3}
	Thu {4}
	Fri {5}
	Sat {6}
}
set today [clock format [clock seconds] -format {%a %B %Y %m %d}]
set dow $days([lindex $today 0])
set first [expr ($days([lindex $today 0])-[lindex $today 4]+1)%7]
frame .fcal
frame .fcal.mth
button .fcal.mth.prv -text <
button .fcal.mth.nxt -text >
label .fcal.mth.mthl -text [lrange $today 1 2]
canvas .fcal.can -width 168 -height 142 -bg white
bind .fcal.can <ButtonPress-1> {
	set x [expr int([%W canvasx %x]/24)*24]
	set y [expr int([%W canvasy %y]/24)*24]
	.fcal.can create rect $x $y [expr $x+24] [expr $y+24] -outline red
}
grid .fcal.mth.prv -column 0 -row 0
grid .fcal.mth.mthl -column 1 -row 0
grid .fcal.mth.nxt -column 2 -row 0
grid .fcal.mth
grid .fcal.can
grid .fcal

set x 13
set y 13
.fcal.can create rect 1 1 170 24 -fill pink -outline blue
foreach d {S M T W T F S} {
	.fcal.can create text $x $y -text $d
	incr x 24
}

#set x [expr 12+12*0]
set x [expr 37+12*($first)]
#set y [expr 0+12]
set y [expr 25+12]
#set nr 0
set nr $first
for {set d 1} {$d<=31} {incr d} {
	set c [expr ($d % 2==1)?"white":"#CCCCCC"]
	.fcal.can create rect [expr $x-12] [expr $y-12] [expr $x+12] [expr $y+12] -fill $c -outline $c
	.fcal.can create text $x $y -text $d
	incr x 24
	incr nr
	if {$nr==7} {
		set x 13
		set nr 0
		incr y 24
	}
}

