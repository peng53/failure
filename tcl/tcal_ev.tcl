#!/usr/bin/env tclsh8.6
package require Tk
source "tcal.tcl"
source "tcalev_sql.tcl"

bind . <Control-Key-q> {
	exit
}

proc new_props_win {} {
	toplevel .props
	wm transient .props .
	wm geometry .props 350x350
	wm title .props Properties
	bind .props <Control-Key-q> {
		exit
	}
	pack [label .props.l1 -text {Entry Properties} -font 16]
	pack [label .props.l2 -text {Start Date}] -anchor w
	pack [entry .props.da -font 10] -fill x
	pack [label .props.l3 -text {End Date}] -anchor w
	pack [entry .props.db -font 10] -fill x
	pack [label .props.l4 -text {Event}] -anchor w
	pack [entry .props.ev -font 10] -fill x
	pack [label .props.l5 -text {More}] -anchor w
	pack [text .props.mr -font 10 -height 7] -fill x
	pack [frame .props.bt]
	pack [button .props.bt.save -text Save -width 6] -side left
	pack [button .props.bt.rev -text Revert -width 6] -side left
	pack [button .props.bt.del -text Delete -width 6] -side left
	pack [button .props.bt.ext -text Exit -command {destroy .props} -width 6] -side left
	return .props
}
proc check_open {} {
	if {{.props} in [winfo children .]} {
		return 1
	}
	return 0
}

proc new_cal_win {square_size text_offset day_font hh m y} {
	toplevel .calwin
	wm transient .calwin .
	set size [expr {$square_size*7}]
	wm maxsize .calwin $size $size
	wm title .calwin Calender
	bind .calwin <Control-Key-q> {
		exit
	}
	pack [Cal::Cal .calwin.cal $m $y $square_size $text_offset $day_font $hh]

}

proc add_all_events {w} {
	set cy 0
	set cm 0
	set cyi {}
	set cmi {}
	foreach r [EventStor::get_base_rows {} {start_date}] {
		lassign [split [lindex $r 1] -] y m d
		#puts "$y $m $d"
		if {$cy==$y} {
			if {$cm==$m} {
				$w insert $cmi end -values [list $d [lindex $r 2]]
			} else {
				set cm $m
				set cmi [$w insert $cyi end -text $cm]
				$w insert $cmi end -values [list $d [lindex $r 2]]
			}
		} else {
			set cy $y
			set cyi [$w insert {} end -text $cy -open 1]
			set cm $m
			set cmi [$w insert $cyi end -text $cm]
			$w insert $cmi end -values [list $d [lindex $r 2]]
		}
	}
}
#~ proc hideshow {f} {
	#~ if {[string length [grid info $f]]==0} {
		#~ grid $f
	#~ } else {
		#~ grid remove $f
	#~ }
#~ }

proc get_range_ac {A B {L {}}} {
	# Runs through checkbutton array to spans of 'trues'.
	# Returns List of ranges.
	# E.g {{1 2} 4} would mean 1 to 2 and just 4 are checked.
	# {1 3 5 7} would mean just 1, 3, 5, 7.
	# {1 12} would mean 1 to 12.
	# This algo uses recursive acumulator; since TCL does not
	# have tail call opt, it shouldn't be used for large B-A.
	# Loop version coming soon.
	if {$A>$B} { return $L }
	global cb
	set a 0
	for {set i $A} {$i<=$B} {incr i} {
		if {$cb($i)==1} {
			set a $i
			break
		}
	}
	if {$a==0} { return $L }
	for {set b [expr {$a+1}]} {$b<=$B} {incr b} {
		if {$cb($b)==0} {
			if {$b==[expr {$a+1}]} {
				return [get_range_ac [expr {$b+1}] $B [list {*}$L $a]]
			} else {
				return [get_range_ac [expr {$b+1}] $B [list {*}$L [list $a [expr $b-1]]]]
			}
		}
	}
	return [list {*}$L [list $a $B]]
}

proc get_range_i {A B} {
	global cb
	set o [list]
	for {set I $A} {$I<=$B} {incr I} {
		while {$I<=$B && $cb($I)==0} {
			incr I
		}
		if {$I>$B} {
			return $o
		}
		if {$cb($I)==1} {
			set a $I
			incr $I
			while {$I<=$B && $cb($I)==1} {
				incr I
			}
			if {$a==[expr {$I-1}]} {
				lappend o $a
			} else {
				lappend o [list $a [expr {min($B,$I-1)}]]
			}
		}
	}
	return $o
}

proc search_ops {} {
	puts [get_range_ac 1 12]
	puts [get_range_i 1 12]
}
proc main2 {} {
	wm title . Events
	set square_size 64
	set text_offset 12
	set day_font 11
	set hh 20
	# Init Cal with the current month and year.
	lassign [clock format [clock seconds] -format {%N %Y}] m y
	#pack [frame .mframe] -expand 1 -fill both
	menu .men
	. configure -menu .men
	#menu .men.view
	#set calpos Window
	#.men.view add radiobutton -label Window -variable calpos
	#.men.view add radiobutton -label Floating -variable calpos
	#.men add cascade -menu .men.view -label View
	.men add command -label Quit -command exit
	pack [labelframe .search -text Search -font 16] -fill x
	pack [frame .search.options] -side left -expand 1 -fill x
	pack [frame .search.options.date]
	pack [labelframe .search.options.date.mths -text Months] -side left
	#~ for {set m 1} {$m<13} {incr m} {
		#~ pack [checkbutton .search.options.date.mths.m$m -indicatoron 0 -variable t($m) -width 2 -text $m] -side left
	#~ }
	global cb
	for {set i 1} {$i<13} {incr i} {
		pack [checkbutton .search.options.date.mths.cb_$i -variable cb($i) -text $i -width 2 -indicatoron 0] -side left
	}
	pack [labelframe .search.options.date.yr -text Year] -side left
	pack [entry .search.options.date.yr.e -width 5] -side left
	pack [label .search.options.date.yr.l2 -text - -font 10] -side left
	pack [entry .search.options.date.yr.e2 -width 5] -side left
	pack [labelframe .search.options.wh -text Where]
	pack [ttk::combobox .search.options.wh.cb] -side left
	pack [ttk::combobox .search.options.wh.cb2 -width 4] -side left
	pack [entry .search.options.wh.e] -side left
	pack [button .search.options.wh.add -text Add] -side left
	pack [labelframe .search.options.or -text Order]
	pack [ttk::combobox .search.options.or.cb] -side left
	pack [button .search.options.or.add -text Add] -side left

	pack [labelframe .search.got -text Options]
	pack [listbox .search.got.lb -height 7 -yscrollcommand {.search.got.sb set}] -side left
	pack [scrollbar .search.got.sb -command {.search.got.lb yview}] -side left -fill y
	pack [button .search.ok -text Search -command search_ops] -side bottom
	pack [button .search.clear -text Clear] -side bottom

	pack [labelframe .evets -text Events -font 16] -expand 1 -fill both
	pack [ttk::treeview .evets.evs -columns {Day Event} -yscrollcommand {.evets.sb set}] -expand 1 -fill both -side left
	#-show headings
	pack [scrollbar .evets.sb -command {.evets.evs yview}] -side left -fill y
	.evets.evs column #0 -width 100 -minwidth 100 -stretch 0
	.evets.evs heading #1 -text Day
	.evets.evs column #1 -width 40 -minwidth 40 -stretch 0
	.evets.evs heading #2 -text Event
	#new_props_win
	#new_cal_win $square_size $text_offset $day_font $hh $m $y
	#EventStor::build_db :memory:
	#EventStor::holidays_us 2016
	#EventStor::holidays_us 2017
	#EventStor::holidays_us 2018
	#add_all_events .mframe.evets.evs
	search_ops

}

main2