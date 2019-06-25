#!/usr/bin/env tclsh8.6
#package require Tk
package require mysqltcl

#pack [ttk::treeview .tv_links -columns {views tags} -yscrollcommand {.tv_links_sb set}] -side left -fill both -expand 1
#pack [scrollbar .tv_links_sb -command {.tv_links yview}] -side left -fill y

#foreach {c l w} [list #0 Group/Name 128 views Views 128 tags Tags 128] {
	## this loop adjusts the treeview's columns
	#.tv_links heading $c -text $l -anchor w
	#.tv_links column $c -minwidth 16 -width $w
	#incr i
#}

#proc insert_group {name {parent {}}} {
	## need to check if parent in .tv_links
	#return [.tv_links insert $parent end -text $name]
#}

#proc insert_filename {parent name {views 0}} {
	## need to check if parent in .tv_links
	#return [.tv_links insert $parent end -text $name -value [list $views]]
#}

#set g1 [insert_group cookies]
#set g2 [insert_group bagels $g1]
#set i1 [insert_filename $g2 my_file 200]
#set i2 [insert_filename $g1 my_other_file 111]
#puts $i1
#puts $i2

##mysqlsel $dbhandle {select * from keywords} -flatlist
#mysql::sel $dbhandle {select * from keywords}
#while {[llength [set row [mysql::fetch $dbhandle]]]>0} {
	#puts [lindex $row 1]
#}
#set ext {}
#set my_dir {}
#foreach $file [glob -directory $my_dir $ext] {
	#mysql
#}

proc open_database {username password database} {
	# Returns dbhandle if params result in a correct open
	if {[catch {set dbhandle [mysql::connect -user $username -pass $password -db $database]}] != 0} {
		return {}
	}
	return $dbhandle
}

proc create_new_database {dbhandle dbname} {
	# Creates database and populates it with default tables
	# Can this be injected? Need to test.
	# Have not found any way to make prepared stmts
	mysql::exec $dbhandle "create database if not exists $dbname"
	mysql::use $dbhandle $dbname
	base_tables $dbhandle
}

proc base_tables {dbhandle} {
	# Creates the base tables in database
	mysql::exec $dbhandle {CREATE TABLE rel(gid int,pid int,depth int) }
	mysql::exec $dbhandle {CREATE TABLE group(
		gid int auto_increment primary key,
		name char(255))
	}
}

delimiter $$
create procedure new_group (in name char(255), in parent int)
begin
	declare rid int;
	insert into groups (name) value (name);
	select last_insert_id() into rid;
	insert into rel (gid,pid,depth) values (rid,rid,0);
	if (parent is NULL) then insert into rel (gid,pid,depth) values (rid,NULL,1);
	else insert into rel SELECT rid,pid,depth+1 FROM rel where gid=parent AND pid IS NOT NULL;
	end if;
end$$
delmiter ;
