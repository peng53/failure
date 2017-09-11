#!/usr/bin/env tclsh8.6
package require Tk
source "tcal.tcl"

bind . <Control-Key-q> {
	exit
}

proc new_props_win {} {
	toplevel .props
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
	set size [expr {$square_size*7}]
	wm maxsize .calwin $size $size
	wm title .calwin Calender
	bind .calwin <Control-Key-q> {
		exit
	}
	pack [Cal::Cal .calwin.cal $m $y $square_size $text_offset $day_font $hh]

}
proc main2 {} {
	wm title . Events
	set square_size 64
	set text_offset 12
	set day_font 11
	set hh 20
	# Init Cal with the current month and year.
	lassign [clock format [clock seconds] -format {%N %Y}] m y
	grid [frame .mframe]
	new_cal_win $square_size $text_offset $day_font $hh $m $y
	#toplevel .calwin
	#set f .mframe.cal
	#set f .calwin.cal
	#pack [Cal::Cal $f $m $y $square_size $text_offset $day_font $hh]
	#pack $f 
	#-column 0 -row 0
	#grid [entry .mydate]
	#bind $f.can <Key-Return> {
	#	focus .mydate
	#}
	#proc put_mydate {f} {
	#	.mydate delete 0 end
	#	.mydate insert 0 [Cal::$f.get_selected]
	#}
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

	grid [frame .mframe.evets] -column 1 -row 0
	#	-sticky nesw
	grid rowconfigure .mframe.evets 1 -weight 1
	grid [label .mframe.evets.l -text Events -font 16]
	grid [ttk::treeview .mframe.evets.evs -columns {Date Event} -show headings] -column 0 -row 1 -sticky nesw
	.mframe.evets.evs heading #1 -text Date
	.mframe.evets.evs column #1 -width 100
	.mframe.evets.evs heading #2 -text Event
	new_props_win
}

main2