#!/usr/bin/env tclsh8.6
package require Tk
package require mysqltcl

proc init_filenames_tv {parent} {
	pack [frame $parent] -fill both -expand 1
	pack [ttk::treeview $parent.fnames -columns {views tags} -yscrollcommand "$parent.fnames_sb set"] -side left -fill both -expand 1
	pack [scrollbar $parent.fnames_sb -command "$parent.fnames yview"] -side left -fill y
	foreach {c l w} [list #0 Group/Name 128 views Views 128 tags Tags 128] {
		# this loop adjusts the treeview's columns
		$parent.fnames heading $c -text $l -anchor w
		$parent.fnames column $c -minwidth 16 -width $w
		incr i
	}
}

proc insert_group {widget name {parent {}}} {
	# need to check if parent in .tv_links
	# ideally would accept a list of:
	# group_id name
	if {[$widget.fnames exists $parent]} {
		return [$widget.fnames insert $parent end -text $name]
	}
}

proc insert_filename {widget parent name {views 0}} {
	# need to check if parent exists in widget first
	# ideally would accept a list of:
	# rownumber filename views tags
	if {[$widget.fnames exists $parent]} {
		return [$widget.fnames insert $parent end -text $name -value [list $views]]
	}
}

init_filenames_tv .links
puts [insert_group .links test]
puts [insert_filename .links 11 my_file 200]

proc open_database {username password database} {
	# Returns dbhandle if params result in a correct open
	if {[catch {set dbhandle [mysql::connect -user $username -pass $password -db $database]}] != 0} {
		return {}
	}
	return $dbhandle
}

proc login_database {user pass} {
	# Logs into mysql
	if {[catch {set dbhandle [mysql::connect -user $user -pass $pass]}] != 0} {
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
	special_tables $dbhandle
}

proc base_tables {dbhandle} {
	# Creates the base tables in database and procedures
	mysql::exec $dbhandle {CREATE TABLE if not exists rel(gid int NOT NULL,pid int,depth int NOT NULL) }
	mysql::exec $dbhandle {CREATE TABLE if not exists groups (gid int auto_increment primary key, name char(255) NOT NULL)}
	mysql::exec $dbhandle {create procedure if not exists new_group (in name char(255), in parent int) 
		begin
			declare rid int;
			insert into groups (name) value (name);select last_insert_id() into rid;
			insert into rel (gid,pid,depth) values (rid,rid,0);
			if (parent = 0) then insert into rel (gid,pid,depth) values (rid,NULL,1);
			else insert into rel SELECT rid,pid,depth+1 FROM rel where gid=parent AND pid IS NOT NULL;
			end if;
			select rid;
		end;
	}
}

proc special_tables {dbhandle} {
	# Creates tables specific to this application
	mysql::exec $dbhandle {create table if not exists filenames
			(id int auto_increment primary key, gid int, name char(255) NOT NULL, views int default 0)}
	mysql::exec $dbhandle {create table if not exists tags
			(id int auto_increment primary key, tag char(16) NOT NULL)}
	mysql::exec $dbhandle {create table if not exists tag_map
			(fileid int, tagid int)}

	mysql::exec $dbhandle {create procedure if not exists add_filename (in name char(255), in parent int)
		begin
			insert into filenames (gid,name) VALUES (nullif(parent,0),name);
			select last_insert_id();
		end;
	}
	mysql::exec $dbhandle {create procedure if not exists delete_group (in d_gid int)
		begin
			declare fgid int;
			declare done int default 0;
			declare GIDS cursor for select gid from rel where pid=d_gid;
			declare continue handler for not found set done = 1;
			open GIDS;
			delete_loop: LOOP
				fetch GIDS into fgid;
				if done = 1 THEN LEAVE delete_loop;
				end if;

				delete tag_map from tag_map join filenames on fileid=id where gid=fgid;

				delete from filenames where gid=fgid;
				delete from groups where gid=fgid;
				delete from rel where gid=fgid;
			END LOOP delete_loop;
			close GIDS;
		end;
	}
}

proc new_group {dbhandle name {parent 0}} {
	set l [mysql::sel $dbhandle "call new_group('$name',$parent)" -flatlist]
	puts "Created group $name with parent $parent"
	return [lindex $l 0]
}

proc new_filename {dbhandle fname {group 0}} {
	set r [mysql::sel $dbhandle "call add_filename('$fname',$group)" -flatlist]
	return [lindex $r 0]
}

proc add_filenames {dbhandle fdir {group 0}} {
	# Adds filenames in fdir to group
	set added [list]
	foreach f [glob -tails -types f -directory $fdir *] {
		lappend added $f
		lappend [new_filename $dbhandle $f $group]
	}
	return $added
}

proc add_directory {dbhandle fdir name {group 0}} {
	# Adds a directory and its files to group
	set gid [new_group $dbhandle $name $group]
	puts "Made group $name with gid $gid parent $group"
	set filenames [add_filenames $dbhandle $fdir $gid]
	return $gid
}

proc delete_group {dbhandle d_gid} {
	mysql::exec $dbhandle "call delete_group($d_gid)"
}

proc test_database_creation {} {
	set user $env(user)
	set pass $env(pass)
	set dbhandle [login_database $user $pass]
	create_new_database $dbhandle {files_meta}

	set docs [new_group $dbhandle docs]
	set osimg [add_directory $dbhandle ~/os_images OS $docs]
	set pet [add_directory $dbhandle ~/pet pet $docs]
	set music [add_directory $dbhandle ~/Music Music $pet]

	mysql::close $dbhandle
}


