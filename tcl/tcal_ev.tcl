#!/usr/bin/env tclsh8.6
package require Tk
source "tcal.tcl"
source "tcalev_sql.tcl"

namespace eval Evv {
	variable cROWID -1
}
bind . <Control-Key-q> {
	exit
}
proc prompt_number {t d m M c} {
	# Creates a prompt with tITLE and dESCRIPTION that
	# with success call cOMMAND with a value between m and M
	toplevel .promptn
	wm title .promptn $t
	wm resizable .promptn 0 0
	pack [label .promptn.l -text $d -font 12]
	pack [entry .promptn.e]
	proc prompt_number_vlad {m M c} {
		set s [.promptn.e get]
		if {$m<$s && $s<$M} {
			$c $s
			destroy .promptn
		} else {
			tk_messageBox -type ok -icon info -message {Invalid value}
		}
	}
	pack [button .promptn.b_ok -command "prompt_number_vlad $m $M $c" -text Ok] -side left
	pack [button .promptn.b_cl -command {destroy .promptn} -text Cancel] -side left
}

proc new_props_win {} {
	# Create a properties window
	if {[check_open .props]} { return }
	toplevel .props
	wm geometry .props 400x350
	wm title .props Properties
	bind .props <Control-Key-q> {
		exit
	}
	wm protocol .props WM_DELETE_WINDOW {
		if {[tk_messageBox -message {Close properties?} -type yesno] eq "yes"} {
			destroy .props
		}
	}
	Props
}
proc Props {} {
	pack [label .props.l1 -text {Entry Properties} -font 16]
	foreach {w n} {da {Start Date} db {End Date}} {
		pack [label .props.l$w -text "$n (M/D/Y HH:MM)"] -anchor w
		pack [frame .props.$w] -fill x
		foreach {o f t l} {m 1 12 2 d 1 31 2 y 1000 9999 4 h 0 23 2 mm 0 59 2} {
			pack [ttk::spinbox .props.$w.$o -from $f -to $t -font 10 -wrap 1 -width $l -format %0$l.0f] -side left -fill x -expand 1
		}
	}
	pack [label .props.l4 -text {Event}] -anchor w
	pack [entry .props.ev -font 10 -width 10] -fill x
	pack [label .props.l5 -text {More}] -anchor w
	pack [text .props.mr -font 10 -height 2 -width 10] -fill both -expand 1
	pack [frame .props.bt] -side bottom
	pack [button .props.bt.save -text Save -command save_props] -side left
	pack [button .props.bt.new -text New -command new_props] -side left
	pack [button .props.bt.rev -text Revert -command fill_props] -side left
	pack [button .props.bt.del -text Delete -command delete_props] -side left
	pack [button .props.bt.ext -text Hide -command {destroy .props}] -side left
	return .props
}
proc fill_props {} {
	# Fill properties window fields with event details
	if {$Evv::cROWID!=-1} {
		#foreach r [EventStor::ps_get_more $Evv::cROWID] {
			lassign [lindex [EventStor::ps_get_more $Evv::cROWID] 0] d1 d2 n more
			#lassign $r d1 d2 n more
			foreach {w D} [list da $d1 db $d2] {
				lassign [split $D] ymd hmm
				lassign [split $ymd -] y m d
				lassign [split $hmm :] h mm
				.props.$w.m set $m
				.props.$w.d set $d
				.props.$w.y set $y
				.props.$w.h set $h
				.props.$w.mm set $mm
			}
			.props.ev delete 0 end
			.props.ev insert 0 $n
			.props.mr delete 1.0 end
			.props.mr insert 1.0 $more
		#}
	}
}
proc delete_props {} {
	# Deletes entry from events database. Does not update treeview
	# but advises user to refresh
	if {$Evv::cROWID!=-1} {
		EventStor::delete_row $Evv::cROWID
		set $Evv::cROWID -1
		tk_messageBox -type ok -icon info -message "Event Deleted\nPlease refresh."
	}
}
proc save_props {} {
	# Saves entry by creating if not made and updating elsewise.
	set d1 [EventStor::second_date [.props.da.y get] [.props.da.m get] [.props.da.d get] [.props.da.h get] [.props.da.mm get]]
	set d2 [EventStor::second_date [.props.db.y get] [.props.db.m get] [.props.db.d get] [.props.db.h get] [.props.db.mm get]]
	set n [.props.ev get]
	set dm [string trim [.props.mr get 1.0 7.0]]
	if {$Evv::cROWID==-1} {
		EventStor::add_row $d1 $d2 $n $dm
		set Evv::cROWID [EventStor::lastrow_added]
		tk_messageBox -type ok -icon info -message "Event Added\nPlease refresh."
	} else {
		EventStor::update_row $Evv::cROWID $d1 $d2 $n $dm
		tk_messageBox -type ok -icon info -message {Event Updated}
	}
}
proc new_props {} {
	# Set current event to 'new', meaning save will create
	# a new event
	set Evv::cROWID -1
	tk_messageBox -type ok -icon info -message "Remember to save this new event!\n(and refresh)."
}
proc check_open {w} {
	# Checks whether window w is created yet.
	if {$w in [winfo children .]} {
		return 1
	}
	return 0
}
proc new_cal_win {} {
	# Creates a calendar window.
	if {[check_open .calwin]} { return }
	toplevel .calwin
	set size [expr {$Cal::v::SQS*7+48}]
	wm maxsize .calwin $size $size
	wm title .calwin Calender
	bind .calwin <Control-Key-q> {
		destroy .calwin
	}
	Cals
}
proc wid {type name cmd {options {}}} {
	if {[check_open $name]} {
		return
	}
	lassign $options T
	switch $type {
		win {
			toplevel $name
			wm title $name $T
			#wm maxsize $x $y
		}
		fra {
			pack [labelframe $name -text $T] -expand 1 -fill both -side left
		}
	}
	$cmd
}
proc Cals {} {
	# Creates a calendar widget.
	pack [Cal::Cal .calwin.cal]
	pack [frame .calwin.set]
	pack [label .calwin.set.mthl -text Month] -side left
	pack [ttk::combobox .calwin.set.mth -values {{1 JAN} {2 FEB} {3 MAR} {4 APR} {5 MAY} {6 JUN} {7 JUL} {8 AUG} {9 SEP} {10 OCT} {11 NOV} {12 DEC}} -state readonly -width 6] -side left
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
	pack [button .calwin.b2 -text {Properties Date} -command props_cal_date] -side left
	pack [button .calwin.b3 -text Hide -command {destroy .calwin}] -side left
}
proc props_cal_date {} {
	# Get date selected in calendar and put it
	# in the properties window
	set D [Cal::.calwin.cal.get_selected]
	switch [llength $D] {
		0 {
			set m2 [set m $Cal::v::MTH]
			set y2 [set y $Cal::v::YR]
			set d 1
			set d2 [Cal::mth_day_ct $m $y]
		}
		3 {
			lassign $D m d y
			lassign $D m2 d2 y2
		}
		6 {
			lassign $D m d y m2 d2 y2
		}
	}
	new_props_win
	.props.da.y set $y
	.props.da.m set $m
	.props.da.d set $d
	.props.db.y set $y2
	.props.db.m set $m2
	.props.db.d set $d2
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
	set a $A
	while {$a<=$B && !$cb($a)} { incr a }
	if {$a>$B} { return $L }
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
		# r is DATE NAME ROWID
		lassign $r D name rowid
		lassign [split $D -] y m d
		if {$cy!=$y} {
			set cy $y
			set cyi [$w insert {} end -id y$y -text $cy -open 1]
			set cm 0
		}
		if {$cm!=$m} {
			set cm $m
			set cmi [$w insert $cyi end -id y$y$m  -text $cm]
		}
		$w insert $cmi end -id $rowid -values [list $d $name]
	}
}
proc all_events {w} {
	# Get all events from open database and
	# calls for them to be added to treeview
	set rs [EventStor::ps_get_basic]
	$w delete [$w children {}]
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
			set D1 [list $y $m 1]
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
proc load_file {} {
	# Selects a file of events to load.
	# Barebones ATM
	set s [tk_getOpenFile -defaultextension .db -filetypes {{{Database file} .db}} -title {Load database..}]
	if {[string length $s]>0} {
		#EventStor::close_db
		EventStor::open_instead $s
		tk_messageBox -type ok -icon info -message {Refresh to see any changes.}
	}
}
proc save_as_file {} {
	# Save the current events (via normal search) to
	# a file.
	set s [tk_getSaveFile -defaultextension .db -filetypes {{{Database file} .db}} -title {Save database}]
	if {[string length $s]>0} {
		EventStor::copy_to $s
		EventStor::open_instead $s
	}
}
proc save_file {} {
	# Commits changes to file
	#EventStor::save_changes
	# does not work ATM: use save as and overwrite
}
proc close_file {} {
	# Closes open file without commiting.
	EventStor::close_db
	EventStor::build_db :memory:
}
proc main2 {} {
	wm title . Events
# Init Cal with the current month and year.
	lassign [clock format [clock seconds] -format {%N %Y}] m y
	Cal::CalVars $m $y 32 8 {Arial 10} 16
	menu .men
	menu .men.db
	menu .men.db.aut
	menu .men.search
	. configure -menu .men
	.men add cascade -label Database -menu .men.db
	.men.db add command -label {New file} -command close_file
	.men.db add command -label {Load file} -command load_file
	.men.db add command -label {Save As file} -command save_as_file
	.men.db add command -label {Close file} -command close_file
	.men.db add command -label {Save changes} -command save_file
	.men.db add separator
	.men.db add cascade -label {Auto..} -menu .men.db.aut
	.men.db.aut add command -label {US Holidays} -command "prompt_number {US Holidays} {For which year?} 1000 3000 EventStor::holidays_us"
	.men add command -label Calender -command {wid fra .calwin Cals {Calendar}}
	.men add command -label Properties -command {wid fra .props Props {Properties}}
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
	pack [ttk::treeview .evets.evs -columns {Day Event} -selectmode browse -yscrollcommand {.evets.sb set}] -expand 1 -fill both -side left
	bind .evets.evs <Button-1> {
		if {[string index [set r [.evets.evs identify row %x %y]] 0]==y} {
			set Evv::cROWID -1
		} else {
			set Evv::cROWID $r
			if {![check_open .props]} { new_props_win }
			fill_props
		}
	}
	bind .evets.evs <Key-Return> {
		if {[string index [set r [.evets.evs selection]] 0]==y} {
			set Evv::cROWID -1
		} else {
			set Evv::cROWID $r
			if {![check_open .props]} { new_props_win }
			fill_props
		}
	}
	pack [scrollbar .evets.sb -command {.evets.evs yview}] -side left -fill y
	.evets.evs column #0 -width 100 -minwidth 100 -stretch 0
	.evets.evs heading #1 -text Day
	.evets.evs column #1 -width 40 -minwidth 40 -stretch 0
	.evets.evs heading #2 -text Event
	EventStor::build_db :memory:
}

main2
