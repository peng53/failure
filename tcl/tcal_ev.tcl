#!/usr/bin/env tclsh8.6
package require Tk
source "tcal.tcl"
source "tcalev_sql.tcl"

bind . <Control-Key-q> {
	exit
}
proc new_props_win {} {
	# Create a properties window
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
proc check_open {w} {
	# Checks whether window w is created yet.
	if {$w in [winfo children .]} {
		return 1
	}
	return 0
}
proc new_cal_win {square_size text_offset day_font hh m y} {
	# Creates a calendar window.
	toplevel .calwin
	wm transient .calwin .
	set size [expr {$square_size*7+48}]
	wm maxsize .calwin $size $size
	wm title .calwin Calender
	bind .calwin <Control-Key-q> {
		exit
	}
	wm protocol .calwin WM_DELETE_WINDOW {
		if {[tk_messageBox -message "Quit?" -type yesno] eq "yes"} {
			wm forget .calwin
		}
	}
	pack [Cal::Cal .calwin.cal $m $y $square_size $text_offset $day_font $hh]
	pack [frame .calwin.set]
	pack [label .calwin.set.mthl -text Month] -side left
	pack [ttk::combobox .calwin.set.mth -values {{1 January} {2 Febuary} {3 March} {4 April} {5 May} {6 June} {7 July} {8 August} {9 September} {10 October} {11 November} {12 December}} -state readonly] -side left
	.calwin.set.mth current 0
	proc set_my {} {
		lassign [split [.calwin.set.mth get]] n N
		if {[string length [set y [.calwin.set.yre get]]]==0} {
			set y $Cal::v::YR
		}
		Cal::set_mth_yr $n $y
		Cal::cal_day

	}
	pack [label .calwin.set.yrl -text Year] -side left
	pack [entry .calwin.set.yre -width 6] -side left
	pack [button .calwin.set.set -text Set -command set_my] -side left
	pack [button .calwin.b1 -text {Search M/Y} -command {search_by_cal .evets.evs} ] -side left
	pack [button .calwin.b2 -text {Properties Date}] -side left
	pack [button .calwin.b3 -text Hide] -side left
}
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
	# Gets spans of checked months from checkbutton array
	global cb
	set o [list]
	for {set I $A} {$I<=$B} {incr I 2} {
		while {$I<=$B && !$cb($I)} { incr I }
		if {$I<=$B && $cb($I)} {
			set a $I
			incr $I
			while {$I<=$B && $cb($I)} { incr I }
			incr I -1
			if {$a==$I} {
				lappend o [list $a]
			} else {
				lappend o [list $a $I]
			}
		}
	}
	return $o
}
proc insert_rows {w rs} {
	# Inserts of rows in rs to widget w.
	# The root children are the years followed by months
	set cy 0
	foreach r $rs {
		lassign [split [lindex $r 1] -] y m d
		if {$cy!=$y} {
			set cy $y
			set cyi [$w insert {} end -text $cy -open 1]
			set cm 0
		}
		if {$cm!=$m} {
			set cm $m
			set cmi [$w insert $cyi end -text $cm]
		}
		$w insert $cmi end -values [list $d [lindex $r 2]]
	}
}
proc all_events {w} {
	# Get all events from open database and
	# calls for them to be added to treeview
	set rs [EventStor::ps_get_basic]
	insert_rows $w $rs
}
proc search_by_date {w} {
	# Gather options for searching by date and
	# executes search.
	if {[string length [set y [.search.date.yr.e get]]]==0} {
		tk_messageBox -type ok -icon error -message {A year is required!}
		return
	}
	if {[llength [set mths [get_range_i 1 12]]]==0} {
		set mths [list [list 1 12]]
	}
	$w delete [$w children {}]
	set rs [list]
	if {[string length [set d [.search.date.dy.e get]]]==0} { set d 1}
	if {[string length [set y2 [.search.date.yr.e2 get]]]==0} { set y2 $y }
	foreach mm $mths {
		if {[llength $mm]==1} {
			set m2 [set m [lindex $mm 0]]
		} else {
			lassign $mm m m2
		}
		set d2 [Cal::mth_day_ct $m2 $y2]
		lappend rs {*}[EventStor::ps_get_date_range [list $y $m $d] [list $y2 $m2 $d2]]
	}
	insert_rows $w $rs
}
proc search_by_cal {w} {
	# Get date(-range) on calendar and
	# get all events in that range
	set D [Cal::.calwin.cal.get_selected]
	switch [llength $D] {
		0 {
			set m $Cal::v::MTH
			set y $Cal::v::YR
			set D1 [list $y $m 0]
			set D2 [list $y $m [Cal::mth_day_ct $m $y]]
		}
		3 {
			lassign $D m d y
			set D1 [list $y $m $d]
			set D2 $D1
		}
		6 {
			lassign $D m d y m2 d2 y2
			set D1 [list $y $m $d]
			set D2 [list $y2 $m2 $d2]
		}
	}
	set rs [EventStor::ps_get_date_range $D1 $D2]
	$w delete [$w children {}]
	insert_rows $w $rs
	
}
proc search_by_name {w} {
	# Get name field and execute search by name.
	# If it starts with the 'name', it is included.
	# Case does not matter
	set s [string tolower [.search.name.e get]]
	if {[string length $s]==0} {
		tk_messageBox -type ok -icon error -message {Event name is required!}
		return
	}
	$w delete [$w children {}]
	set rs [EventStor::ps_get_event $s%]
	insert_rows $w $rs
}
proc reset_ents {} {
	# Resets entries and checkbuttons
	global cb
	array set cb [list 1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 0 10 0 11 0 12 0]
	foreach w {date.yr.e date.yr.e2 date.dy.e date.dy.e2 name.e } {
		.search.$w delete 0 end
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
	menu .men
	menu .men.db
	menu .men.db.aut
	menu .men.search
	. configure -menu .men
	.men add cascade -label Database -menu .men.db
	.men.db add command -label {New file}
	.men.db add command -label {Load file}
	.men.db add command -label {Close file}
	.men.db add separator
	.men.db add cascade -label {Auto..} -menu .men.db.aut
	.men.db.aut add command -label {US Holidays}
	.men add command -label Calender -command "new_cal_win $square_size $text_offset $day_font $hh $m $y"
	.men add cascade -label Search -menu .men.search
	.men.search add command -label All -command {all_events .evets.evs}
	.men.search add separator
	.men.search add command -label {by date} -command {search_by_date .evets.evs}
	.men.search add command -label {by name} -command {search_by_name .evets.evs}
	.men.search add separator
	.men.search add command -label Reset -command reset_ents
	.men add command -label Quit -command exit
	pack [labelframe .search -text Search -font 16] -fill x
	pack [frame .search.date] -anchor w  -expand 1 -fill x
	pack [labelframe .search.date.mths -text Months] -side left -expand 1 -fill x
	global cb
	for {set i 1} {$i<13} {incr i} {
		pack [checkbutton .search.date.mths.cb_$i -variable cb($i) -text $i -width 2 -indicatoron 0] -side left
	}
	pack [labelframe .search.date.dy -text Day] -side left
	pack [entry .search.date.dy.e -width 3] -side left
	pack [label .search.date.dy.l -text :] -side left
	pack [entry .search.date.dy.e2 -width 3] -side left
	pack [labelframe .search.date.yr -text Year] -side left
	pack [entry .search.date.yr.e -width 5] -side left
	pack [label .search.date.yr.l2 -text - -font 10] -side left
	pack [entry .search.date.yr.e2 -width 5] -side left
	pack [labelframe .search.name -text {Event Name}] -side left -expand 1 -fill x
	pack [entry .search.name.e] -expand 1 -fill x

	pack [labelframe .evets -text Events -font 16] -expand 1 -fill both
	pack [ttk::treeview .evets.evs -columns {Day Event} -yscrollcommand {.evets.sb set}] -expand 1 -fill both -side left
	pack [scrollbar .evets.sb -command {.evets.evs yview}] -side left -fill y
	.evets.evs column #0 -width 100 -minwidth 100 -stretch 0
	.evets.evs heading #1 -text Day
	.evets.evs column #1 -width 40 -minwidth 40 -stretch 0
	.evets.evs heading #2 -text Event
	#new_props_win
	EventStor::build_db :memory:
	EventStor::holidays_us 2016
	EventStor::holidays_us 2017
	EventStor::holidays_us 2018
	#all_events .evets.evs
}

main2
