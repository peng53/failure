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
	conn eval {
		INSERT INTO rel VALUES(NULL,:new_gid,:new_gid,0);
	}
	if {$parent==0} {
		conn eval {
			INSERT INTO rel VALUES(NULL,:new_gid,NULL,1);
		}
	} else {
		conn eval {
			INSERT INTO rel SELECT NULL,:new_gid,pid,depth+1 FROM rel
				WHERE rel.gid=:parent AND rel.pid IS NOT NULL;
		}
	}
	return $new_gid
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
	set subgroups [conn eval {
		DELETE FROM groups WHERE rowid=:gid LIMIT 1;
		DELETE FROM data WHERE gid=:gid;
		DELETE FROM rel WHERE gid=:gid;
		SELECT gid FROM rel WHERE pid=:gid;
	}]
	dict unset DBConn::groups $gid
	dict unset DBConn::groups $subgroups
	# First pass, delete group and its data,
	# its relation to root and gets its subgroups
	foreach s_gid $subgroups {
		conn eval {
			DELETE FROM data WHERE gid=:s_gid;
			DELETE FROM groups WHERE rowid=:s_gid LIMIT 1;
			DELETE FROM rel WHERE gid=:s_gid;
		}
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
	set children [list [conn eval {
		DELETE FROM groups WHERE rowid=:gid;
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
	if {$gid==0} {
		conn eval {
			INSERT INTO data VALUES(NULL,:key,:value,:mtime);
		}
	} elseif {[dict exists $DBConn::groups $gid]} {
		conn eval {
			INSERT INTO data VALUES(:gid,:key,:value,:mtime);
		}
	} else {
		# how did we get here?
		return 0
	}
	return [conn eval {SELECT last_insert_rowid();}]
}
proc data_group {rowid {gid 0}} {
	# Simplified version of update_data that only changes group
	if {$gid==0} {
		conn eval {
			UPDATE data SET gid=NULL WHERE rowid=:rowid LIMIT 1;
		}
	} else {
		conn eval {
			UPDATE data SET gid=:gid WHERE rowid=:rowid LIMIT 1;
		}
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
	} elseif {$new_pid==0} {
		# Replace gid's relation with one to root
		conn eval {
			DELETE FROM rel WHERE gid=:gid AND depth>1;
			UPDATE rel SET pid=NULL WHERE gid=:gid AND depth=1 LIMIT 1;
		}
	# Get subgroups whose relations should also change
		set children [list]
		conn eval {
			SELECT gid AS c FROM rel WHERE pid=:gid AND depth>0 ORDER BY depth DESC;
		} { lappend children $c }
		# Delete while perserving relation.
		# Rows with depth>1 ignore identity rows & NULL parent.
		foreach c $children {
			set p [conn eval {
				SELECT pid FROM rel WHERE gid=:c AND depth=1 LIMIT 1;
			}]
			conn eval {
				DELETE FROM rel WHERE gid=:c AND depth>1;
				INSERT INTO rel
					SELECT NULL,:c,pid,depth+1 FROM rel WHERE gid=:p AND depth>1;
			}
		}
	} elseif {[dict exists $DBConn::groups $new_pid]} {
		# .. to new_pid.
		if {[conn eval {
				SELECT 1 FROM rel WHERE gid=:new_pid AND pid=:gid LIMIT 1;
			}]==1} {
			puts {Attempted to change parent to child}
			return
			# Can't change parent to child
		}
		conn eval {
			DELETE FROM rel WHERE gid=:gid AND depth>1;
			UPDATE rel SET pid=:new_pid WHERE gid=:gid AND depth=1 LIMIT 1;
			INSERT INTO rel
				SELECT NULL,:c,pid,depth+1 FROM rel WHERE gid=:new_pid AND
					pid IS NOT NULL AND depth>0;
		}
		set children [list]
		conn eval {
			SELECT gid AS c FROM rel WHERE pid=:gid AND depth>0 ORDER BY depth DESC;
		} { lappend children $c }
		foreach c $children {
			set p [conn eval {
				SELECT pid FROM rel WHERE gid=:c AND depth=1 LIMIT 1;
			}]
			conn eval {
				DELETE FROM rel WHERE gid=:c AND depth>0;
				INSERT INTO rel
					SELECT NULL,:c,pid,depth+1 FROM rel WHERE gid=:p AND pid IS NOT NULL;
			}
		}
	}
}
proc update_data {rowid key value mtime ngid} {
	# Updates properties of a row in data.
	conn eval {
		UPDATE data SET key=:key, value=:value, mtime=:mtime WHERE rowid=:rowid LIMIT 1;
	}
	if {$ngid==0} {
		conn eval { UPDATE data SET gid=NULL WHERE rowid=:rowid LIMIT 1; }
	} elseif {[dict exists $DBConn::groups $ngid]} {
		conn eval { UPDATE data SET gid=:ngid WHERE rowid=:rowid LIMIT 1; }
	} else {
		# ngid is not 0 or a group
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
				FROM PL.moz_bookmarks JOIN PL.moz_places ON PL.moz_bookmarks.id=PL.moz_places.id
					WHERE url LIKE 'http%';
			END TRANSACTION;
			DETACH PL;
		}
	}
	return $ngid
}
proc import_json {fname} {
	#set fp [open $myfile r]
	#set lines [split [read $fp] ","]
	#foreach l $lines {
	#}

}

proc auto_group {parent pattern new_parent {dry_run false}} {
	# Where parent is the gid of the bookmarks.
	# pattern is a matching regex,
	# and new_parent to where to move matches to.
	# if dry_run is true, no movement will occur.
	if {!$DBConn::is_open} {
		puts {DB is not open!}
		return -1
	}
	if {$parent == $new_parent && !$dry_run} {
		puts {From and to cannot match!}
		return -1
	}
	if {$parent != 0 && ![dict exists $DBConn::groups $parent]} {
		puts {Parent does not exist!}
		return -1
	}
	if {!$dry_run && $new_parent != 0 && ![dict exists $DBConn::groups $new_parent]} {
		puts {New parent does not exist!}
		return -1
	}
	conn function regexp -deterministic { regexp --}
	set new_mtime [clock format [clock seconds] -format {%Y-%m-%d %T.000}]
	if {$dry_run} {
		puts {dry run}
		set to_move [conn eval {
			SELECT rowid FROM data WHERE key REGEXP :pattern;
		}]
		return $to_move
	} else {
		conn eval {
			UPDATE data SET gid=:new_parent WHERE key REGEXP :pattern
		}
	}
	
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

