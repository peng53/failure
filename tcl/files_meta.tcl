#!/usr/bin/env tclsh8.6
package require Tk
package require mysqltcl

namespace eval Gui {
	namespace eval v {
		variable unloaded
		variable groups
	}
	proc initMainView {} {
		pack [frame .links] -fill both -expand 1
		pack [ttk::treeview .links.fnames -columns {views tags} -yscrollcommand {.links.fnames_sb set}] -fill both -expand 1
		pack [scrollbar .links.fnames_sb -command {.links.fnames yview}] -side left -fill y
		foreach {c l w} [list #0 Group/Name 128 views Views 128 tags Tags 128] {
			# this loop adjusts the treeview's columns
			.links.fnames heading $c -text $l -anchor w
			.links.fnames column $c -minwidth 16 -width $w
			incr i
		}
		bind .links.fnames <<TreeviewOpen>> {
			set gid [.links.fnames focus]
			if {[dict exists $Gui::v::unloaded $gid]} {
				Gui::loadGroupItems $gid
			}
		}
	}
	proc initMenu {} {
		menu .men
		.men add command -label {New Group} -command Gui::winNewGroup
		.men add command -label {Reload} -command Gui::loadRootGroups
		.men add command -label {Quit} -command Gui::quit
		. configure -menu .men
	}
	proc winNewGroup {} {
		toplevel .new_group
		wm attributes .new_group -topmost 1
		wm resizable .new_group 0 0
		wm title .new_group {New Group}
		pack [labelframe .new_group.name -text Name] -expand 1 -fill x
		pack [entry .new_group.name.e] -side left -expand 1 -fill x
		pack [labelframe .new_group.dir -text Directory] -expand 1 -fill x
		pack [entry .new_group.dir.e] -side left  -expand 1 -fill x
		pack [button .new_group.dir.select -text Open -command {Gui::setEntry .new_group.dir.e [tk_chooseDirectory]}]

		pack [button .new_group.cancel -text Cancel -command {destroy .new_group}] -side right
		pack [button .new_group.new -text New -command Gui::newGroupAction] -side right
	}
	proc newGroupAction {} {
		set dbhandle $App::v::dbhandle
		set parent [Gui::selectedGroup]
		set name [.new_group.name.e get]
		set folder [.new_group.dir.e get]
	#	set gid [new_group $dbhandle $name

	#	Gui::newGroup $master 123 $name $parent
	}
	proc setEntry {e text} {
		$e delete 0 end
		$e insert 0 $text
	}
	proc dummyLoadItem {id} {
		# Creates a dummy load item for group
		.links.fnames insert $id end -id d$id -text {(load)}
	}
	proc loadRootGroups {} {
		# Loads only root groups and creates on demand
		pack forget .links.fnames
		.links.fnames delete [.links.fnames children {}]
		set Gui::v::groups [dict create]
		set Gui::v::unloaded [dict create]
		mysql::receive $App::v::dbhandle {select rel.gid,name from groups join rel on groups.gid=rel.gid where pid is null} [list gid name] {
			Gui::newGroup $gid $name
		}
		pack .links.fnames -fill both -expand 1 -before .links.fnames_sb -side left
	}
	proc loadGroupItems {gid} {
		# Loads subgroups & filenames from a group
		# First delete dummy item
		.links.fnames delete d$gid
		# Get subgroups
		mysql::receive $App::v::dbhandle "select rel.gid,name from rel join groups on rel.gid=groups.gid where pid=$gid and depth=1" [list sgid name] {
			Gui::newGroup $sgid $name $gid
		}
		# Get filenames and tags
		mysql::receive $App::v::dbhandle "select id,name,views,group_concat(tagid) from filenames left outer join tag_map on id=fileid where gid=$gid group by id order by id" [list id name views tags] {
			if {[string length $tags]==0} {
				Gui::newFilename r$id $name [list $views] $gid
			} else {
				Gui::newFilename r$id $name [list $views $tags] $gid
			}
		}

		dict append Gui::v::groups $gid [dict get $Gui::v::unloaded $gid]
		dict unset Gui::v::unloaded $gid
		puts "Loaded group #$gid!"
	}
	proc quit {} {
		mysql::close $App::v::dbhandle
		puts {Ended App}
		destroy .
	}
	### Following procs only add rows visually {
	proc newGroup {id name {parent {}}} {
		# need to check if parent in .tv_links
		# ideally would accept a list of:
		# group_id name
		if {[.links.fnames exists $parent]} {
			dict append Gui::v::unloaded $id $name
			.links.fnames insert $parent end -id $id -text $name
			Gui::dummyLoadItem $id
		}
	}

	proc newFilename {id name meta {parent {}} } {
		# need to check if parent exists in widget first
		# ideally would accept a list of:
		# rownumber filename views tags
		if {[.links.fnames exists $parent]} {
			set tags [lassign $meta views]
			set stag [list]
			foreach tag [split $tags ,] {
				lappend stag [dict get $App::v::tags $tag]
			}
			return [.links.fnames insert $parent end -id $id -text $name -value [list $views [join $stag {, }]]]
		}
	}
	### }
	proc selectedGroup {} {
		set select [list [.links.fnames selection]]
		if {[llength $select] == 0} {
			return {}
		} else {
			set item [lindex $select 0]
			if {[string match r* $item]} {
				return [.links.fnames parent $item]
			} else {
				return $item
			}
		}
	}
}

namespace eval App {
	namespace eval v {
		variable dbhandle
		variable tags
	}
}

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

proc retrive_tags {dbhandle} {
	set App::v::tags [dict create]
	mysql::receive $dbhandle {select id,tag from tags} [list id tag] {
		dict append App::v::tags $id $tag 
	}
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



Gui::initMainView
Gui::initMenu

set user $env(user)
set pass $env(pass)
set App::v::dbhandle [login_database $user $pass]
create_new_database $App::v::dbhandle {files_meta}
retrive_tags $App::v::dbhandle
Gui::loadRootGroups
