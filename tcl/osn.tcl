#!/bin/tclsh8.6
package require Tk
namespace eval osnv {
	variable MAC off s {} locked 0 cur_file {}
}
wm title . \\\\
menu .men
. configure -menu .men
foreach {l c} {New new_text_file Open open_text_file Save save_text_file {Save As} save_as_text_file} {
	.men add command -label $l -command $c
}

pack [frame .maintext] -expand 1 -fill both
pack [frame .maintext.mac_words] -side right -fill y
pack [listbox .maintext.mac_words.lb -yscrollcommand {.maintext.mac_words.sb set} -width 10] -side left -expand 1 -fill y
pack [scrollbar .maintext.mac_words.sb -command {.maintext.mac_words.lb yview}] -side left -fill y
pack [scrollbar .maintext.sb_v -command {.maintext.t yview}] -side right -fill y
pack [text .maintext.t -wrap none -yscrollcommand {.maintext.sb_v set} -xscrollcommand {.maintext.sb_h set}] -expand 1 -fill both
pack [scrollbar .maintext.sb_h -orient horizontal -command {.maintext.t xview}] -side bottom -fill x
pack [labelframe .frame_m_bar -text Macro] -fill x
pack [radiobutton .frame_m_bar.off -indicatoron 0 -text Off -variable osnv::MAC -value off -command mac_off] -fill y -side left
pack [radiobutton .frame_m_bar.rep -indicatoron 0 -text Repeat -variable osnv::MAC -value rep -command mac_on] -fill y -side left
pack [radiobutton .frame_m_bar.reg -indicatoron 0 -text Register -variable osnv::MAC -value reg -command mac_on] -fill y -side left
pack [checkbutton .frame_m_bar.lock -indicatoron 0 -text Lock -variable osnv::locked -onvalue 1 -offvalue 0] -fill y -side left
pack [button .frame_m_bar.slck -text ShiftL -command shift_lock] -side left
#pack [button .frame_m_bar.nlck -text NumL] -side left
pack [button .frame_m_bar.all -text All -command {.maintext.t tag add sel 0.0 end}] -side left
pack [button .frame_m_bar.text -text Test -command {open_text_file}] -side left
pack [labelframe .frame_kb -text Keyboard] -expand 1 -fill x -side left
set r 0
foreach {s} {1234567890 qwertyuiop asdfghjkl zxcvbnm} {
	pack [frame .frame_kb.r$r] -fill x
	foreach {c} [split $s {}] {
		pack [button .frame_kb.r$r.b$c -text $c -command "wri_rep $c"] -fill both -expand 1 -side left
	}
	incr r
}
pack [frame .frame_kb.r$r] -fill x
pack [button .frame_kb.r$r.bBackspace -text >) -command kbackspace] -side left
foreach {l c} {==> \\t Space space LF Enter} {
	pack [button .frame_kb.r$r.b$l -text $l -command "pressed $c"] -side left
}
pack configure .frame_kb.r$r.bSpace -fill x -expand 1
pack [button .frame_kb.r$r.bShift -text q/Q -command shift_last] -side left -fill x
pack [button .frame_kb.r$r.bDelete -text (< -command {pressed Delete}] -side left

pack [labelframe .frame_sp -text Symbols] -side left -anchor ne
set r 0
foreach {s} {`'/ []* ;-+ ,} {
	pack [frame .frame_sp.r$r]
	foreach {c} [split $s {}] {
		pack [button .frame_sp.r$r.b$c -text $c -width 1 -command "wri_rep \\$c"] -side left
	}
	incr r
}
pack [button .frame_sp.r3.bPeriod -text . -width 1 -command {wri_rep .}] -side left
pack [button .frame_sp.r3.bBackslash -text \\ -width 1 -command {wri_rep \\}] -side left
proc rep_indices_with {b e c} {
	.maintext.t delete $b $e
	.maintext.t insert $b $c
}
pack [labelframe .frame_np -text Num] -side left -anchor ne
set r 0
foreach {s} {789 456 123 0} {
	pack [frame .frame_np.r$r] -fill x
	foreach {c} [split $s {}] {
		pack [button .frame_np.r$r.b$c -text $c -command "pressed $c"] -fill both -expand 1 -side left
	}
	incr r
}
pack [button .frame_np.r3.bDZero -text 00 -command {pressed 00}] -side left
pack [frame .frame_np.r4] -fill x
pack [button .frame_np.r4.bEnter -text = -command {pressed Enter}] -fill both -expand 1 -side left

proc have_selection {} {
	return [expr [string length [.maintext.t tag ranges sel]]>0]
}
proc alternate {s} {
	return [string map {
		! 1 @ 2 # 3 $ 4 % 5 ^ 6 & 7 * 8 ( 9 ) 0
		1 ! 2 @ 3 # 4 $ 5 % 6 ^ 7 & 8 * 9 ( 0 )
		a A b B c C d D e E f F g G h H i I j J k K l L m M 
		n N o O p P q Q r R s S t T u U v V w W x X y Y z Z
		A a B b C c D d E e F f G g H h I i J j K k L l M m 
		N n O o P p Q q R r S s T t U u V v W w X x Y y Z z
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
		foreach {p} {kb sp} {
			foreach {c} [winfo children .frame_$p.r$i] {
				set C [alternate [$c cget -text]]
				$c configure -text $C -command "wri_rep \\$C"
			}
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
proc wri_rep {c} {
	if {[have_selection]} {
		.maintext.t delete sel.first sel.last
	} 
	.maintext.t insert insert $c
}
proc pressed {c} {
	set o $osnv::MAC
	switch $o {
		off {
			switch $c {
				Delete { kdelete }
				Enter { .maintext.t insert insert \n }
				space { wri_rep { } }
				1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 0 { wri_rep $c }
			}
		}
		rep {
			switch $c {
				Enter { .maintext.t insert insert \n }
				Delete {
					if {!$osnv::locked} {
						mac_off
					} else {
						mac_on
					}
				}
				1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 0 {
					if [have_selection] {
						set s [.maintext.t get sel.first sel.last]
					} else {
						set s [.maintext.t get $osnv::s insert]
					}
					for {set i 0} {$i<$c} {incr i} {
						.maintext.t insert insert $s
					}
					if {!$osnv::locked} {
						mac_off
					} else {
						mac_on
					}
				}
			}
		}
		reg {
			switch $c {
				space {
					if [have_selection] {
						set s [.maintext.t get sel.first sel.last]
					} else {
						set s [.maintext.t get $osnv::s insert]
					}
					.maintext.mac_words.lb insert end $s
					if {!$osnv::locked} {
						mac_off
					} else {
						mac_on
					}
				}
				Enter {
					.maintext.t insert insert [.maintext.mac_words.lb get active]
					if {!$osnv::locked} {
						mac_off
					} else {
						mac_on
					}
				}
				Delete {
					.maintext.mac_words.lb delete end
					if {!$osnv::locked} {
						mac_off
					} else {
						mac_on
					}
				}
			}
		}
	}
}
proc mac_off {} {
	.frame_m_bar.off select
	set osnv::s {}
}
proc mac_on {} {
	set osnv::s [.maintext.t index insert]
}
proc clear_text {} {
	.maintext.t delete 0.0 end
}
proc close_text {} {
	set osnv::cur_file {}
	.maintext.t edit modified 0
	wm title . \\\\
}
proc new_text_file {} {
	if {[.maintext.t edit modified]} {
		switch [tk_messageBox -message {Modified body, save?} -type yesnocancel -icon question] {
			yes {
				if {[save_text_file]==1} {
					tk_messageBox -message {File was NOT saved} -type ok -icon error
					return
				}
			}
			cancel { return }
		}
	}
	clear_text
	close_text
}
proc save_as_text_file {} {
	set osnv::cur_file {}
	save_text_file
}
proc save_text_file {} {
	if {$osnv::cur_file=={}} {
		set s [tk_getSaveFile]
		if {$s=={}} {
			return 1
		}
		set osnv::cur_file $s
	}
	set fid [open $osnv::cur_file w]
	puts -nonewline $fid [.maintext.t get 0.0 end]
	close $fid
	wm title . $osnv::cur_file
	.maintext.t edit modified 0
	return 0
}
proc open_text_file {} {
	if {[.maintext.t edit modified]} {
		switch [tk_messageBox -message {Modified body, save?} -type yesnocancel -icon question] {
			yes {
				if {[save_text_file]==1} {
					tk_messageBox -message {File was NOT saved} -type ok -icon error
					return
				}
			}
			cancel { return }
		}
	}
	set s [tk_getOpenFile]
	if {$s!={}} {
		clear_text
		set osnv::cur_file $s
		set fp [open $s r]
		set file_data [read $fp]
		.maintext.t insert 0.0 $file_data
		.maintext.t edit modified 0
		wm title . $osnv::cur_file
		close $fp
	}
}
bind . <Control-Key-q> {
	exit
}
bind . <KeyPress-Shift_L> {
	shift_lock
}
bind .maintext.t <KeyPress-Delete> {
	pressed Delete
}
bind .maintext.t <KeyPress-space> {
	if {$osnv::MAC=={reg}} {
		pressed space
	}
}
bind .maintext.mac_words.lb <KeyPress-Return> {
	.maintext.t insert insert [.maintext.mac_words.lb get active]
}
bind .maintext.mac_words.lb <Button-3> {
	.maintext.mac_words.lb delete [.maintext.mac_words.lb nearest %y]
}
bind . <Control-Key-o> {
	open_text_file
}
bind . <Control-Key-s> {
	save_text_file
}
bind . <Control-Key-n> {
	new_text_file
}