#!/bin/tclsh8.6
package require Tk
bind . <Control-Key-q> {
	exit
}
bind . <Shift_L> {
	shift_lock
}
pack [frame .maintext] -expand 1 -fill both
pack [scrollbar .maintext.sb_v -command {.maintext.t yview}] -side right -fill y
pack [text .maintext.t -wrap none -yscrollcommand {.maintext.sb_v set} -xscrollcommand {.maintext.sb_h set}] -expand 1 -fill both
pack [scrollbar .maintext.sb_h -orient horizontal -command {.maintext.t xview}] -side bottom -fill x
pack [labelframe .frame_kb -text Keyboard] -expand 1 -fill x -side left
#~ bind .maintext.t <Key> {
	#~ if {{%K}=={Shift_L}} {
		#~ shift_lock
	#~ } else {
		#~ break
	#~ }
#~ }
set r 0
foreach {s} {{1234567890} {qweryuiop} {asdfghjkl} {zxcvbnm}} {
	pack [frame .frame_kb.r$r] -fill x
	foreach {c} [split $s {}] {
		pack [button .frame_kb.r$r.b$c -text $c -command ".maintext.t insert insert $c"] -fill both -expand 1 -side left
	}
	incr r
}
pack [frame .frame_kb.r$r] -fill x
pack [button .frame_kb.r$r.bBackspace -text Backspace -command ".maintext.t delete {insert - 1 chars} {insert}"] -side left
set SPACE { }
foreach {l c} {Tab \\t Space $SPACE Line \\n} {
	pack [button .frame_kb.r$r.b$l -text $l -command ".maintext.t insert insert $c"] -side left
}
pack configure .frame_kb.r$r.bSpace -fill x -expand 1
pack [button .frame_kb.r$r.bShift -text Shift -command shift_last] -side left -fill x
pack [button .frame_kb.r$r.bShiftL -text L -command shift_lock] -side left -fill x
pack [button .frame_kb.r$r.bDelete -text Delete -command ".maintext.t delete {insert} {insert + 1 chars}"] -side left

pack [labelframe .frame_sp -text Symbols] -side left -anchor ne
set r 0
foreach {s} {{`-=} {[]} {;'} {,/}} {
	pack [frame .frame_sp.r$r] -fill x
	foreach {c} [split $s {}] {
		pack [button .frame_sp.r$r.b$c -text $c -command ".maintext.t insert insert \{$c\}"] -fill both -expand 1 -side left
	}
	incr r
}
pack [button .frame_sp.r1.bBackslash -text \\ -command ".maintext.t insert insert \\"] -fill both -expand 1 -side left
pack [button .frame_sp.r3.bPeriod -text . -command ".maintext.t insert insert ."] -fill both -expand 1 -side left
proc rep_indices_with {b e c} {
	.maintext.t delete $b $e
	.maintext.t insert $b $c
}
pack [labelframe .frame_np -text Num] -side left -anchor ne
set r 0
foreach {s} {{789/} {456*} {123-} {0+}} {
	pack [frame .frame_np.r$r] -fill x
	foreach {c} [split $s {}] {
		pack [button .frame_np.r$r.b$c -text $c -command ".maintext.t insert insert $c"] -fill both -expand 1 -side left
	}
	incr r
}
pack [frame .frame_np.r4] -fill x
pack [button .frame_np.r4.bLock -text L] -side left
pack [button .frame_np.r4.bEnter -text = -command ".maintext.t insert insert \\n"] -fill both -expand 1 -side left

proc sel_or_insert {} {
	lassign [.maintext.t tag ranges sel] b e
	if {[string length $b]==0} {
		set e [.maintext.t index insert]
		lassign [split $e .] r c
		if {$c==0} { return "0 0" }
		set b $r.[expr {$c-1}]
	}
	return "$b $e"
}
proc alternate {s} {
##
 #set S {}
 # 	foreach {c} [split $s {}] {
 # 		if {[string is lower $c]} {
 # 			append S [string toupper $c]
 # 		} elseif {[string is upper $c]} {
 # 			append S [string tolower $c]
 # 		} elseif {[string is digit $c]} {
 # 			append S [string map {1 ! 2 @ 3 # 4 $ 5 % 6 ^ 7 & 8 * 9 ( 0 )} $c]
 # 		} else {
 # 			append S [string map {! 1 @ 2 # 3 $ 4 % 5 ^ 6 & 7 * 8 ( 9 ) 0} $c]
 # 		}
 # 	}
 # 	rep_indices_with $b $e $S
 ##
	return [string map {
		! 1 @ 2 # 3 $ 4 % 5 ^ 6 & 7 * 8 ( 9 ) 0
		1 ! 2 @ 3 # 4 $ 5 % 6 ^ 7 & 8 * 9 ( 0 )
		a A b B c C d D e E f F g G h H i I j J k K l L m M n N o O p P q Q r R s S t T u U v V w W x X y Y z Z
		A a B b C c D d E e F f G g H h I i J j K k L l M m N n O o P p Q q R r S s T t U u V v W w X x Y y Z z
		- _ = + [ \{ ] \} ; : ' \" , < . > / ? ` ~ \\ |
		_ - + = \{ [ \} ] : ; \" ' < , > . ? / ~ ` | \\
	} $s]
	
}
proc shift_last {} {
	lassign [.maintext.t tag ranges sel] b e
	if {[string length $b]!=0} {
		rep_indices_with $b $e [alternate [.maintext.t get $b $e]]
	} else {
		set e [.maintext.t index insert]
		lassign [split $e .] r c
		if {$c==0} { return }
		set b $r.[expr {$c-1}]
		rep_indices_with $b $e [alternate [.maintext.t get $b $e]]
	}
}
proc shift_lock {} {
	for {set i 0} {$i<4} {incr i} {
		foreach {c} [winfo children .frame_kb.r$i] {
			set C [alternate [$c cget -text]]
			$c configure -text $C -command ".maintext.t insert insert $C"
		}
		foreach {c} [winfo children .frame_sp.r$i] {
			set C [alternate [$c cget -text]]
			$c configure -text $C -command ".maintext.t insert insert \{$C\}"
		}
	}
}
