#!/bin/tclsh8.6
package require Tk
namespace eval osnv {
	variable MAC off
	variable s {}
	variable locked 0
}
bind . <Control-Key-q> {
	exit
}
bind . <Shift_L> {
	shift_lock
}
pack [frame .maintext] -expand 1 -fill both
pack [frame .maintext.mac_words] -side right -fill y
pack [listbox .maintext.mac_words.lb -yscrollcommand {.maintext.mac_words.sb set} -width 10] -side left -expand 1 -fill y
pack [scrollbar .maintext.mac_words.sb -command {.maintext.mac_words.lb yview}] -side left -fill y
pack [scrollbar .maintext.sb_v -command {.maintext.t yview}] -side right -fill y
pack [text .maintext.t -wrap none -yscrollcommand {.maintext.sb_v set} -xscrollcommand {.maintext.sb_h set}] -expand 1 -fill both
pack [scrollbar .maintext.sb_h -orient horizontal -command {.maintext.t xview}] -side bottom -fill x
pack [labelframe .frame_m_bar -text Macro] -fill x
pack [radiobutton .frame_m_bar.off -text Off -variable osnv::MAC -value off -command mac_off] -side left
pack [radiobutton .frame_m_bar.rep -text Repeat -variable osnv::MAC -value rep] -side left
pack [radiobutton .frame_m_bar.reg -text Register -variable osnv::MAC -value reg] -side left
pack [checkbutton .frame_m_bar.lock -text Lock -variable osnv::locked -onvalue 1 -offvalue 0] -side left
pack [button .frame_m_bar.all -text All -command {.maintext.t tag add sel 0.0 end}] -side left
#pack [button .frame_m_bar.text -text Test -command {puts $osnv::MAC}] -side left
pack [labelframe .frame_kb -text Keyboard] -expand 1 -fill x -side left
set r 0
foreach {s} {{1234567890} {qwertyuiop} {asdfghjkl} {zxcvbnm}} {
	pack [frame .frame_kb.r$r] -fill x
	foreach {c} [split $s {}] {
		#pack [button .frame_kb.r$r.b$c -text $c -command ".maintext.t insert insert $c"] -fill both -expand 1 -side left
		pack [button .frame_kb.r$r.b$c -text $c -command "pressed $c"] -fill both -expand 1 -side left
	}
	incr r
}
pack [frame .frame_kb.r$r] -fill x
pack [button .frame_kb.r$r.bBackspace -text Backspace -command {pressed BackSpace}] -side left
set SPACE { }
foreach {l c} {Tab \\t Space $SPACE Line Enter} {
	pack [button .frame_kb.r$r.b$l -text $l -command "pressed $c"] -side left
}
pack configure .frame_kb.r$r.bSpace -fill x -expand 1
pack [button .frame_kb.r$r.bShift -text Shift -command shift_last] -side left -fill x
pack [button .frame_kb.r$r.bShiftL -text L -command shift_lock] -side left -fill x
pack [button .frame_kb.r$r.bDelete -text Delete -command {pressed Delete}] -side left

pack [labelframe .frame_sp -text Symbols] -side left -anchor ne
set r 0
foreach {s} {{`-=} {[]} {;'} {,/}} {
	pack [frame .frame_sp.r$r] -fill x
	foreach {c} [split $s {}] {
		pack [button .frame_sp.r$r.b$c -text $c -command "pressed \{$c\}"] -fill both -expand 1 -side left
	}
	incr r
}
pack [button .frame_sp.r1.bBackslash -text \\ -command "pressed \\"] -fill both -expand 1 -side left
pack [button .frame_sp.r3.bPeriod -text . -command "pressed ."] -fill both -expand 1 -side left
proc rep_indices_with {b e c} {
	.maintext.t delete $b $e
	.maintext.t insert $b $c
}
pack [labelframe .frame_np -text Num] -side left -anchor ne
set r 0
foreach {s} {{789/} {456*} {123-} {0+}} {
	pack [frame .frame_np.r$r] -fill x
	foreach {c} [split $s {}] {
		pack [button .frame_np.r$r.b$c -text $c -command "pressed $c"] -fill both -expand 1 -side left
	}
	incr r
}
pack [frame .frame_np.r4] -fill x
pack [button .frame_np.r4.bLock -text L] -side left
pack [button .frame_np.r4.bEnter -text = -command "pressed \\n"] -fill both -expand 1 -side left

proc sel_or_insert {} {
	lassign [.maintext.t tag ranges sel] b e
	if {[string length $b]==0} {
		set e [.maintext.t index insert]
		lassign [split $e .] r c
		if {$c==0} { return }
		set b $r.[expr {$c-1}]
	}
	return "$b $e"
}
proc have_selection {} {
	return [expr [string length [.maintext.t tag ranges sel]]>0]
}
proc alternate {s} {
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
			#$c configure -text $C -command ".maintext.t insert insert $C"
			$c configure -text $C -command "pressed $C"
		}
		foreach {c} [winfo children .frame_sp.r$i] {
			set C [alternate [$c cget -text]]
			#$c configure -text $C -command ".maintext.t insert insert \{$C\}"
			$c configure -text $C -command "pressed \{$C\}"
		}
	}
}
proc kbackspace {} {
	if {[have_selection]} {
		.maintext.t delete sel.first sel.last
	} else {
		.maintext.t delete {insert - 1 chars} insert
	}
}
proc kdelete {} {
	if {[have_selection]} {
		.maintext.t delete sel.first sel.last
	} else {
		.maintext.t delete insert {insert + 1 chars}
	}
}
proc pressed {c} {
	set o $osnv::MAC
	switch $o {
		off {
			switch $c {
				BackSpace {
					kbackspace
				}
				Delete {
					kdelete
				}
				Enter {
					.maintext.t insert insert \n
				}
				default {
					if {[have_selection]} {
						.maintext.t delete sel.first sel.last
					} 
					.maintext.t insert insert $c
				}
			}
		}
		rep {
			switch $c {
				BackSpace {
					set osnv::s [string range $osnv::s 0 [string length $osnv::s]-2]
					.maintext.t delete {insert - 1 chars} insert
				}
				Delete {
					set osnv::s {}
					if {!$osnv::locked} {
						.frame_m_bar.off select
					}
				}
				1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 0 {
					for {set i 0} {$i<$c} {incr i} {
						.maintext.t insert insert $osnv::s
					}
					set osnv::s {}
					if {!$osnv::locked} {
						.frame_m_bar.off select
					}
				}
				Enter {
					append osnv::s \n
					.maintext.t insert insert \n
					
				}
				default {
					append osnv::s $c
					.maintext.t insert insert $c
				}
			}
			#~ if {[string is digit $c]} {
				#~ for {set i 0} {$i<$c} {incr i} {
					#~ .maintext.t insert insert $osnv::s
				#~ }
				#~ set osnv::s {}
				#~ if {!$osnv::locked} {
					#~ .frame_m_bar.off select
				#~ }
			#~ } else {
				#~ append osnv::s $c
				#~ .maintext.t insert insert $c
			#~ }
		}
		reg {
			switch $c {
				{ } {
					.maintext.mac_words.lb insert end $osnv::s
					set osnv::s {}
					if {!$osnv::locked} {
						.frame_m_bar.off select
					}
				}
				Enter {
					.maintext.t insert insert [.maintext.mac_words.lb get active]
					if {!$osnv::locked} {
						.frame_m_bar.off select
					}
				}
				Delete {
					.maintext.mac_words.lb delete end
					set osnv::s {}
					if {!$osnv::locked} {
						.frame_m_bar.off select
					}
				}
				BackSpace {
					set osnv::s [string range $osnv::s 0 [string length $osnv::s]-2]
				}
				default {
					append osnv::s $c
					.maintext.t insert insert $c
				}
			}
		}
	}
}
proc mac_off {} {
	set osnv::s {}
}
bind .maintext.t <KeyPress> {
	if {[string length %K]==1 && [string is alnum %K]} {
		pressed %K
		break
	} elseif {{%K}=={space}} {
		pressed { }
		break
	}
}
bind .maintext.t <Control-Key-q> {
	exit
}
