#!/usr/bin/tclsh
package require sqlite3
if {[llength $argv]==0} {
	puts {Using test.db as filename}
	set s {test.db}
} else {
	set s [lindex $argv 0]
}
if {[file exists $s]} {
	puts {File already exists.}
	exit 1
}
sqlite3 conn $s
proc randint {a b} {
	return [expr {$a+int(rand()*b)}]
}
conn transaction {
	conn eval {CREATE TABLE prod_records(uid text,start_time datetime,end_time datetime,code text,desc text)}
	conn eval {CREATE TABLE employees(uid text,fname text,lname text,email text)}
	for {set i 97} {$i<123} {incr i} {
		set uid [format {%c%d} $i [expr {int(rand()*100)}]]
		set bmth [expr {int(rand()*12)}]
		set bday [expr {int(rand()*28)}]
		set byr [expr {int(rand()*99)}]
		set code [expr {int(rand()*700)}]
		for {set n 0} {$n<5} {incr n} {
			set bhr [expr {int(rand()*12)}]
			set bmin 0
			set d [format {%4d-%02d-%02d %02d:%02d} [expr {$byr+2000}] $bmth $bday $bhr $bmin]
			incr bmin [expr {int(rand()*60)+1}]
			set d2 [format {%4d-%02d-%02d %02d:%02d} [expr {$byr+2000}] $bmth $bday $bhr $bmin]
			conn eval {INSERT into prod_records VALUES($uid,$d,$d2,$code,$n)}
		}
		conn eval {INSERT into employees VALUES($uid,$uid_Fname,$uid_Lname,"$uid@email.com")}
	}
}

