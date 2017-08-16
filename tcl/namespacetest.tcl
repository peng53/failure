# namespacetest.tcl

namespace eval ::myspace {
	namespace export say create
	variable name
}
proc ::myspace::create {s} {
	variable name
	set name $s
}
proc ::myspace::say {} {
	variable name
	puts $name
}

set M [myspace::create hello]
myspace::say
namespace import myspace::say
namespace current
say
namespace delete myspace
