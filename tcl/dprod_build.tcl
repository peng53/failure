#!/usr/bin/tclsh
package require sqlite3
if {[llength $argv]==0} {
	puts {Using test.db as filename}
	set s "test.db"
} else {
	set s [lindex $argv 0]
}
if {[file exists $s]} {
	puts "File already exists."
	exit 1
}
sqlite3 conn $s
set t(1) "2022-01-04 03:27"
set t(2) "2022-01-04 03:33"
set t(3) "2022-01-04 03:46"
set t(4) "2022-01-04 05:02"
conn transaction {
	conn eval {CREATE TABLE prod_records(uid text,start_time datetime,end_time datetime,code text,desc text)}
	conn eval {INSERT into prod_records VALUES("s203",$t(1),$t(2),"112","work")}
	conn eval {INSERT into prod_records VALUES("s203",$t(2),$t(3),"222","break")}
	conn eval {INSERT into prod_records VALUES("s203",$t(4),$t(4),"112","work")}
	conn eval {INSERT into prod_records VALUES("b777",$t(1),$t(3),"112","basic desc")}
	conn eval {INSERT into prod_records VALUES("b777",$t(3),$t(4),"908","lunch")}
	conn eval {INSERT into prod_records VALUES("s203",$t(3),$t(4),"112","work")}
	conn eval {INSERT into prod_records VALUES("t421",$t(1),$t(2),"222","break")}
	conn eval {INSERT into prod_records VALUES("t421",$t(2),$t(3),"908","lunch")}
	conn eval {CREATE TABLE employees(uid text,fname text,lname text,email text)}
	conn eval {INSERT into employees VALUES("s203","John","Speck","js@atemail.com")}
	conn eval {INSERT into employees VALUES("b777","Marz","Tindaz","mtz@atemail.com")}
	conn eval {INSERT into employees VALUES("t421","Nxy","Rne","nr@atemail.com")}
	conn eval {CREATE TABLE arc_rec(uid text,int,lname text,email text)}
}

