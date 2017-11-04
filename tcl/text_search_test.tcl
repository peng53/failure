
package require Tk

pack [text .text]
.text insert 1.0 CAPPY\n
.text insert 2.0 DARN
set r1 [.text search -- CAPPY 2.0]
set r2 [.text get 2.0 {2.0 lineend}]
puts $r1
puts $r2
if {$r1=={1.0}} {
	puts ??
}
#~ pathName search ?switches? pattern index ?stopIndex?

#~ Searches the text in pathName starting at index for a range of characters that
#~ matches pattern. If a match is found, the index of the first character in the match is
#~ returned as result; otherwise an empty string is returned. One or more of the
#~ following switches (or abbreviations thereof) may be specified to control the search: 
