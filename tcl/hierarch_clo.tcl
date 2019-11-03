#!/bin/tclsh8.6
package require sqlite3

namespace eval DBConn {
	variable is_open 0
	variable groups [dict create]
	variable current_file {}
	variable penalty [dict create add 1 del 1 mod 1 gadd 2 gdel 10 gmod 1 move 1 gmove 4]
	variable max_penalty 20
	variable in_trans 0
}

proc prepare_memory {} {
	# Sets up 'conn' as handler for below procs.
	sqlite3 conn :memory:
	set DBConn::is_open 1
	build_tables
}
proc build_tables {} {
	# Builds required tables for hierarch action
	# Ensure that a file in open and bound to 'conn'
	conn eval {
		CREATE TABLE groups(rowid INTEGER primary key,name TEXT);
		CREATE TABLE rel(rowid INTEGER primary key, gid INTEGER,pid INTEGER,depth INTEGER);
		CREATE TABLE data(gid INTEGER,key TEXT,value TEXT,mtime TEXT);
	}
}
# sql stmts BEG
proc sql_self_rel {gid} {
	# Creates a self relation row for gid.
	conn eval {INSERT INTO rel VALUES(NULL,:gid,:gid,0);}
}
proc sql_root_rel {gid} {
	# Creates relation that indicates a root group.
	conn eval {INSERT INTO rel VALUES(NULL,:gid,NULL,1);}
}
proc sql_child_rel {gid pid} {
	# Creates relations for gid under pid
	conn eval {
		INSERT INTO rel SELECT NULL,:gid,pid,depth+1 FROM rel
			WHERE gid=:pid AND pid IS NOT NULL;
	}
}
proc sql_del_grp_rel {gid} {
	# Removes al relations of gid.
	conn eval {DELETE FROM rel WHERE gid=:gid;}
}
proc sql_del_grp_data {gid} {
	# Removes data belonging to gid.
	conn eval {DELETE FROM data WHERE gid=:gid;}
}
proc sql_del_grp {gid} {
	# Remove group gid.
	conn eval {DELETE FROM group WHERE rowid=:gid LIMIT 1;}
}
proc sql_unlink {gid} { ## MACRO ##
	# Removes all relations of gid, then recreates the self_rel.
	sql_del_grp_rel $gid
	sql_self_rel $gid
}
proc sql_cp_pairs {gid} {
	# Returns list of child_parent pairs of gid.
	set pairs [list]
	conn eval {
		SELECT g.gid as CHILD,p.pid as PARENT FROM
			(SELECT gid,pid FROM rel WHERE pid is not NULL AND depth=1) AS p 
		JOIN 
			(SELECT gid FROM rel WHERE pid=:gid AND depth>0 ORDER BY depth DESC) as g 
		ON g.gid=p.gid;
	} {
		lappend pairs $CHILD $PARENT
	}
	return $pairs
}
# sql stmts END
proc add_group {name {parent 0}} {
	# Add sub-group to DB.
	# If parent is 0, assume root.
	if {$parent!=0 && ![dict exists $DBConn::groups $parent]} {
		return 0
	}
	set new_gid [conn eval {
		INSERT INTO groups VALUES(NULL,:name);
		SELECT last_insert_rowid();
	}]
	dict set DBConn::groups $new_gid $name
	sql_self_rel $new_gid
	if {$parent==0} {
		sql_root_rel $new_gid
	} else {
		sql_child_rel $new_gid $parent
	}
	return $new_gid
}
proc del_group_helper {gid} {
	# Wipes group from tables.
	sql_del_grp_rel $gid
	sql_del_grp $gid
	sql_del_grp_data $gid
}
proc del_group {gid} {
	# Deletes group matching gid.
	# This also deletes:
	# 	data belonging to that group
	# 	and groups inside.
	if {![dict exists $DBConn::groups $gid]} {
		return
	}
	conn eval {BEGIN TRANSACTION;}
	del_group_helper $gid
	set subgroups [conn eval {
		SELECT gid FROM rel WHERE pid=:gid;
	}]
	dict unset DBConn::groups $gid $subgroups
	# First pass, delete group and its data,
	# its relation to root and gets its subgroups
	foreach s_gid $subgroups {
		del_group_helper $s_gid
	}
	conn eval {END TRANSACTION;}
	# Second pass, delete subgroups' data
	# and relation.
}
proc del_group_alt {gid {new_parent {}}} {
	# Alternative delete group that preserves
	# data and sub-groups by dropping them to
	# gid's parent.
	if {![dict exists $DBConn::groups $gid]} {
		return
	}
	if {$new_parent!={} && [dict exist $DBConn::groups $new_parent]} {
		if {[conn eval {
			SELECT 1 FROM rel WHERE gid=:new_parent AND pid=:gid LIMIT 1;
		}]==1} {
			# Tried to change_pgroup to a child, not valid ATM
			return
		}
	} else {
		set new_parent [conn eval {
			SELECT ifnull(pid,0) FROM rel WHERE gid=:gid AND depth=1 LIMIT 1;
		}]
	}
	conn eval {BEGIN TRANSACTION;}
	dict unset DBConn::groups $gid
	if {$new_parent==0} {
		conn eval {UPDATE data SET gid=NULL WHERE gid=:gid;}
	} else {
		conn eval {UPDATE data SET gid=:new_parent WHERE gid=:gid;}
	}
	sql_del_grp $gid
	set children [list [conn eval {
		SELECT gid FROM rel WHERE pid=:gid AND depth=1;
	}]]
	foreach c $children {
		change_pgroup $c $new_parent
	}
	conn eval {END TRANSACTION;}
	return $new_parent
}
proc add_data {key value mtime {gid 0}} {
	# Adds a row to data table to group gid.
	# If gid is 0, assume root.
	if {$gid==0 || [dict exists $DBConn::groups $gid]} {
		return [conn eval {
			INSERT INTO data (gid,key,value,mtime) VALUES(nullif(:gid,0),:key,:value,:mtime);
			SELECT last_insert_rowid();
		}]
	}
	return 0
}
proc data_group {rowid {gid 0}} {
	# Simplified version of update_data that only changes group
	conn eval {
		UPDATE data SET gid=nullif(:gid,0) WHERE rowid=:rowid LIMIT 1;
	}
}
proc rm_data {rowid} {
	# Removes a row from data with rowid
	conn eval {
		DELETE FROM data WHERE rowid=:rowid LIMIT 1;
	}
}
proc get_tree {} {
	# Gets the tree of groups.
	# Might be changed later on.
	set groups [dict create]
	conn eval {
		SELECT name,gid,pid FROM groups JOIN rel on groups.rowid=rel.gid WHERE depth=1 ORDER by pid,depth;
	} {
		#puts "$name $gid $pid"
		dict set groups $gid [list $name $pid]
	}
	return $groups
}
proc change_pgroup {gid {new_pid 0}} {
	# Changes the parent group of gid to new_pid.
	# Assume root if 0.
	# ATM branch completely if root
	if {$new_pid==$gid} {
		puts {Attempted to change parent to self}
		return
	}
	if {$new_pid==0} {
		sql_unlink $gid
		sql_root_rel $gid
	} elseif {[dict exists $DBConn::groups $new_pid]} {
		if {[conn eval {
				SELECT 1 FROM rel WHERE gid=:new_pid AND pid=:gid LIMIT 1;
			}]==1} {
			puts {Attempted to change parent to child.}
			return
			# Can't change parent to child
		}
		sql_unlink $gid
		sql_child_rel $gid $new_pid
	} else {
		puts {Attempted to change to invalid parent.}
		return
	}
	set pair [sql_cp_pairs $gid]
	foreach {CHD PAR} $pair {
		sql_unlink $CHD
		sql_child_rel $CHD $PAR
	}
}
proc update_data {rowid key value mtime ngid} {
	# Updates properties of a row in data.
	conn eval {
		UPDATE data SET key=:key, value=:value, mtime=:mtime WHERE rowid=:rowid LIMIT 1;
	}
	if {$ngid==0 || [dict exists $DBConn::groups $ngid]} {
		data_group $rowid $ngid
	}
}
proc update_grp {gid name} {
	# Changes a group's name
	conn eval {
		UPDATE groups SET name=:name WHERE rowid=:gid;
	}
}
proc close_db {} {
	# 'Closes' the db
	if {$DBConn::is_open} {
		set DBConn::is_open 0
		set DBConn::current_file {}
		set DBConn::groups [dict create]
		conn close
	}
}
proc open_db_i {fname} {
	# Opens an existing DB
	if {$DBConn::is_open} {
		return
	}
	set DBConn::current_file $fname
	prepare_memory
	conn eval {
		ATTACH :fname AS M;
		BEGIN TRANSACTION;
		INSERT INTO groups SELECT * FROM M.groups;
		INSERT INTO rel SELECT * FROM M.rel;
		INSERT INTO data SELECT * FROM M.data;
		END TRANSACTION;
		DETACH M;
	}
	set DBConn::groups [dict create]
	conn eval {
		SELECT rowid,name FROM groups;
	} {
		dict set DBConn::groups $rowid $name
	}
}
proc save_db_i {} {
	set fname $DBConn::current_file
	conn eval {
		ATTACH :fname AS M;
		BEGIN TRANSACTION;
		DROP TABLE IF EXISTS M.groups;
		DROP TABLE IF EXISTS M.rel;
		DROP TABLE IF EXISTS M.data;
		CREATE TABLE M.groups(rowid INTEGER primary key,name TEXT);
		CREATE TABLE M.rel(rowid INTEGER primary key, gid INTEGER,pid INTEGER,depth INTEGER);
		CREATE TABLE M.data(gid INTEGER,key TEXT,value TEXT,mtime TEXT);
		INSERT INTO M.groups SELECT * from groups;
		INSERT INTO M.rel SELECT * from rel;
		INSERT INTO M.data SELECT * from data;
		END TRANSACTION;
		PRAGMA M.optimize;
		DETACH M;
	}
}
proc export_tswv {fname} {
	if {!$DBConn::is_open} {
		return
	}
	set fh [open $fh w]
	set G [list]

	conn eval {
		SELECT gid FROM rel WHERE pid IS NULL;
	} {
		lappend G $gid
	}
}
proc import_places {fname mtime} {
	if {!$DBConn::is_open} {
		return -1
	}
	# only imports bookmarks (and not structure) ATM
	set ngid [add_group [format {places.sqlite %s} $mtime]]
	if {$ngid!=0} {
		conn eval {
			ATTACH :fname AS PL;
			BEGIN TRANSACTION;
			INSERT INTO data
				SELECT :ngid,PL.moz_bookmarks.title,PL.moz_places.url,:mtime
				FROM PL.moz_bookmarks JOIN PL.moz_places ON PL.moz_bookmarks.fk=PL.moz_places.id
					WHERE url LIKE 'http%';
			END TRANSACTION;
			DETACH PL;
		}
	}
	return $ngid
}
proc import_json {fname} {
	set fp [open $fname r]
	if {[string length fp]<0} {
		puts {File read was unsuccessful.}
		return -1
	}
	set lines [split [read $fp] {{,}}]
	foreach l $lines {
		if {[regexp {^"title"} $l]} {
			set t [split $l {:}]
			puts [lrange $t 1 end]
		} elseif {[regexp {^"uri"} $l]} {
			set u [split $l {:}]
			puts [join [lrange $u 1 end] :]
		} elseif {[regexp {^"children"} $l]} {
			set c [split $l {:}]
			puts $c
		} elseif {[regexp {^"type"} $l]} {
			set t [split $l {:}]
			puts $t
		}
	}
}

proc search_where {area parent prefix} {
	# builds the 'where' given an area and parent
	set s $prefix
	if {[string equal $parent NULL]} {
		# the root group.
		append s {gid IS NULL AND }
	} elseif {[dict exists $DBConn::groups $parent]} {
		# a specific group.
		append s {gid=:parent AND }
	}
	switch $area {
		key {
			append s {key REGEXP :pattern}
		}
		value {
			append s {value REGEXP :pattern}
		}
		both {
			append s {(key REGEXP :pattern OR value REGEXP:pattern)}
		}
	}
	return $s
}
proc search_data {pattern area {parent NULL}} {
	# if parent is not in DBConn::groups, then do a full search
	# area can be 'key'. 'value', or 'both'
	if {!$DBConn::is_open} {
		puts {DB is not open!}
		return {}
	}
	puts "area is $area"
	puts "gid is $parent"
	puts "pattern is $pattern"
	#set stmt [list {SELECT rowid FROM data WHERE}]
	#if {[string equal $gid NULL]} {
		## the root group.
		#lappend stmt {gid is NULL AND}
	#} elseif {[dict exists $DBConn::groups $gid]} {
		## a specific group.
		#lappend stmt {gid=:gid AND}
	#}
	#switch $area {
		#key { lappend stmt {key REGEXP :pattern} }
		#value {	lappend stmt {value REGEXP :pattern} }
		#both { lappend stmt {(key REGEXP :pattern OR value REGEXP :pattern)} }
	#}
	conn function regexp -deterministic { regexp --}
	#set stmt_c [join $stmt { }]
	#set stmt_c [search_where $area $parent {SELECT rowid FROM data WHERE }]
	set stmt_c {SELECT rowid FROM data WHERE }
	if {[string equal $parent NULL]} {
		# the root group.
		append stmt_c {gid IS NULL AND }
	} elseif {[dict exists $DBConn::groups $parent]} {
		# a specific group.
		append stmt_c {gid=:parent AND }
	}
	switch $area {
		key {
			append stmt_c {key REGEXP :pattern}
		}
		value {
			append stmt_c {value REGEXP ':pattern'}
		}
		both {
			append stmt_c {(key REGEXP ':pattern' OR value REGEXP ':pattern')}
		}
	}
	
	puts $stmt_c
	set rowid [conn eval $stmt_c]
	return $rowid
}
proc auto_group {gid ngid pattern} {
	# Where parent is the gid of the bookmarks.
	# pattern is a matching regex,
	# and new_parent to where to move matches to.
	# if dry_run is true, no movement will occur.
	if {!$DBConn::is_open} {
		puts {DB is not open!}
		return {}
	}
	if {$gid==$ngid} { return }
	if {![string equal $gid NULL] && ![dict exists $DBConn::groups $gid]} {
		puts {Parent does not exist!}
		return {}
	}
	if {![string equal $ngid NULL] && ![dict exists $DBConn::groups $ngid]} {
		puts {New Parent does not exist!}
		return {}
	}
	set stmt "UPDATE data SET gid=:ngid, mtime=:new_mtime WHERE [search_where key $gid]"
	set new_mtime [clock format [clock seconds] -format {%Y-%m-%d %T.000}]
	conn function regexp -deterministic { regexp --}
	puts $stmt
	set rowid [conn eval $stmt]
	return $rowid

#	if {$dry_run} {
#		if {$parent==0} {
#			set to_move [conn eval {
#			SELECT rowid FROM data WHERE gid is NULL AND key REGEXP :pattern;
#			}]
#		} else {
#			set to_move [conn eval {
#			SELECT rowid FROM data WHERE gid=:parent AND key REGEXP :pattern;
#			}]
#		}
#		return $to_move
#	} else {
#		if {$parent==0} {
			#conn eval {
				#UPDATE data SET gid=:new_parent,mtime=:new_mtime WHERE gid is NULL AND key REGEXP :pattern
			#}
		#} else {
			#conn eval {
				#UPDATE data SET gid=:new_parent,mtime=:new_mtime WHERE gid=:parent AND key REGEXP :pattern
			#}
		#}
	#}
}

proc testing_db {} {
	set DBConn::is_open 1
	sqlite3 conn :memory:
	build_tables
	# Inital should be
	# cats
	# * blue
	# * red
	# * yellow
	# * * large
	# dogs
	# * black
	# * * small
	set cats [add_group cats]
	add_group blue $cats
	add_group red $cats
	set yellow [add_group yellow $cats]
	add_group large $yellow
	set dogs [add_group dogs]
	set black [add_group black $dogs]
	add_group small $black
	# Now lets delete_alt dogs
	# black
	# * small
	# should belong to root
	#del_group_alt $dogs
	# Let's do the same for 'yellow'
	# but at same time set parent group to black (large black)
	#del_group_alt $yellow $black
	add_data hello world t $black
	add_data root data bbb


	puts {BETTER PRINT}
	puts {      NAME | GID | PID | DEPTH}
	puts --------------------------------
	set root_c [list]
	conn eval {
		SELECT gid FROM groups JOIN rel on groups.rowid=rel.gid WHERE pid is NULL;
	} {
		lappend root_c $gid
	}
	foreach g $root_c {
		puts "GROUP NUMBER $g"
		conn eval {
			SELECT name,gid,pid,depth FROM groups JOIN rel ON groups.rowid=rel.gid WHERE rel.pid=:g ORDER BY depth;
		} {
			if {$depth==0} {
				puts [format {%10s | %3d | PAR   ENT} $name $gid]
			} else {
				puts [format {%10s | %3d | %3d | %3d} $name $gid $pid $depth]
			}
		}
		puts ---------------
	}
	puts {raw closure table with group names}
	conn eval {select * from rel join groups on groups.rowid=gid} {
		puts "$gid|$name {$pid} $depth"
	}
	conn eval {select * from groups} {
		puts "$rowid $name"
	}
}

