package require Tk
menu .men
. configure -menu .men
.men add command -label Open
.men add command -label Save
.men add command -label Close
.men add command -label {Add Link}
.men add command -label Modify
.men add command -label Delete
pack [ttk::treeview .tv_links -columns {title url mtime}] -side left -fill both -expand 1
pack [scrollbar .tv_links_sb] -side left -fill y
foreach {c l w} [list #0 # 32 title Name 128 url URL 256 mtime {Time Modified} 128] {
	.tv_links heading $c -text $l -anchor w
	.tv_links column $c -minwidth 16 -width $w
}


.tv_links insert "" end -text 1 -values [list {Some Site} http://123fakesite.456 18-04-28]

