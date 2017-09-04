#!/usr/bin/env tclsh8.6
package require Tk
source "tcal.tcl"

proc main2 {} {
	bind . <Control-Key-q> {
		exit
	}
	set square_size 64
	set text_offset 12
	set day_font 11
	set hh 20
	# Init Cal with the current month and year.
	lassign [clock format [clock seconds] -format {%N %Y}] m y
	grid [frame .mframe]
	set f .mframe.cal
	Cal::Cal $f $m $y $square_size $text_offset $day_font $hh
	grid $f -column 0 -row 0
	#grid [entry .mydate]
	#bind $f.can <Key-Return> {
	#	focus .mydate
	#}
	#proc put_mydate {f} {
	#	.mydate delete 0 end
	#	.mydate insert 0 [Cal::$f.get_selected]
	#}
	grid [frame .mframe.ents] -column 1 -row 0
	grid [label .mframe.ents.l1 -text {Selected Entry} -font 16]
	grid [label .mframe.ents.l2 -text {Start Date}] -sticky w
	grid [entry .mframe.ents.da -width 40 -font 10]
	grid [label .mframe.ents.l3 -text {End Date}] -sticky w
	grid [entry .mframe.ents.db -width 40 -font 10]
	grid [label .mframe.ents.l4 -text {Event}] -sticky w
	grid [entry .mframe.ents.ev -width 40 -font 10]
	grid [label .mframe.ents.l5 -text {More}] -sticky w
	grid [text .mframe.ents.mr -width 40 -font 10 -height 7]
	grid [frame .mframe.ents.bt]
	grid [button .mframe.ents.bt.save -text Save] -column 0 -row 0
	grid [button .mframe.ents.bt.ok -text Ok] -column 1 -row 0
	grid [button .mframe.ents.bt.del -text Delete] -column 2 -row 0
	#grid [button .goo -text Gooo -command "put_mydate $f"]
	#grid [button .bquit -text Quit -command exit
}

main2