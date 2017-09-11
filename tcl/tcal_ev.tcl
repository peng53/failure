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
	#set y [.mframe.evets.evs insert {} end -text {2017}]
	#set m [.mframe.evets.evs insert $y end -text {May}]
	#.mframe.evets.evs insert $m end -values {5}
	#puts $y
	#puts $m
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
		#$w insert {} end -values test
	}
}

proc main2 {} {
	wm title . Events
	set square_size 64
	set text_offset 12
	set day_font 11
	set hh 20
	# Init Cal with the current month and year.
	lassign [clock format [clock seconds] -format {%N %Y}] m y
	pack [frame .mframe] -expand 1 -fill both
	#~ proc hideshow {f} {
		#~ if {[string length [grid info $f]]==0} {
			#~ grid $f
		#~ } else {
			#~ grid remove $f
		#~ }
	#~ }
	menu .men
	. configure -menu .men
	menu .men.view
	set calpos Window
	.men.view add radiobutton -label Window -variable calpos
	.men.view add radiobutton -label Floating -variable calpos
	.men add cascade -menu .men.view -label View
	#~ .men add command -label Calender -command {hideshow .mframe.cal}
	#~ .men add command -label Events -command {hideshow .mframe.evets}
	.men add command -label Quit -command exit

	#grid [frame .mframe.evets] -column 1 -row 0
	pack [frame .mframe.evets] -expand 1 -fill both
	#	-sticky nesw
	#grid rowconfigure .mframe.evets 1 -weight 1
	pack [label .mframe.evets.l -text Events -font 16]
	pack [ttk::treeview .mframe.evets.evs -columns {Day Event} -yscrollcommand {.mframe.evets.sb set}] -expand 1 -fill both -side left
	
	#-show headings
	.mframe.evets.evs column #0 -width 100 -minwidth 100 -stretch 0
	.mframe.evets.evs heading #1 -text Day
	.mframe.evets.evs column #1 -width 40 -minwidth 40 -stretch 0
	.mframe.evets.evs heading #2 -text Event
	#new_props_win
	#new_cal_win $square_size $text_offset $day_font $hh $m $y
	EventStor::build_db :memory:
	EventStor::holidays_us 2016
	EventStor::holidays_us 2017
	EventStor::holidays_us 2018
	add_all_events .mframe.evets.evs
	pack [scrollbar .mframe.evets.sb -command {.mframe.evets.evs yview}] -side left -fill y
}

main2