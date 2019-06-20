#!/usr/bin/env tclsh8.6
package require Tk

pack [ttk::treeview .tv_links -columns {views tags} -yscrollcommand {.tv_links_sb set}] -side left -fill both -expand 1
pack [scrollbar .tv_links_sb -command {.tv_links yview}] -side left -fill y

foreach {c l w} [list #0 Group/Name 128 views Views 128 tags Tags 128] {
	# this loop adjusts the treeview's columns
	.tv_links heading $c -text $l -anchor w
	.tv_links column $c -minwidth 16 -width $w
	incr i
}

proc insert_group {name {parent {}}} {
	# need to check if parent in .tv_links
	return [.tv_links insert $parent end -text $name]
}

proc insert_filename {parent name {views 0}} {
	# need to check if parent in .tv_links
	return [.tv_links insert $parent end -text $name -value [list $views]]
}

set g1 [insert_group cookies]
set g2 [insert_group bagels $g1]
set i1 [insert_filename $g2 my_file 200]
set i2 [insert_filename $g1 my_other_file 111]
puts $i1
puts $i2
