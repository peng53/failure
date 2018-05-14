#!/bin/tclsh8.6
package require sqlite3
#package require Tk

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
	set new_gid [conn eval {
		INSERT INTO groups VALUES(NULL,:name);
		SELECT last_insert_rowid();
	}]
	if {$parent==0} {
		conn eval {
			INSERT INTO rel VALUES(NULL,:new_gid,NULL,1);
			INSERT INTO rel VALUES(NULL,:new_gid,:new_gid,0);
		}
	} else {
		conn eval {
			INSERT INTO rel
				SELECT NULL,:new_gid,:new_gid,0
				UNION ALL
				SELECT NULL,:new_gid,pid,depth+1 FROM rel
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
	set subgroups [conn eval {
		DELETE FROM groups WHERE rowid=:gid;
		DELETE FROM data WHERE gid=:gid;
		DELETE FROM rel WHERE gid=:gid;
		SELECT gid FROM rel WHERE pid=:gid;
	}]
	# First pass, delete group and its data,
	# its relation to root and gets its subgroups
	foreach sgid $subgroups {
		conn eval {
			DELETE FROM data WHERE gid=:sgid;
			DELETE FROM groups WHERE rowid=:sgid;
			DELETE FROM rel WHERE gid=:sgid;
		}
	}
	# Second pass, delete subgroups' data
	# and relation.
}
proc add_data {key value mtime {gid 0}} {
	# Adds a row to data table to group gid.
	# If gid is 0, assume root.
	if {$gid==0} {
		conn eval {
			INSERT INTO data VALUES(NULL,:key,:value,:mtime);
		}
	} else {
		conn eval {
			INSERT INTO data VALUES(:gid,:key,:value,:mtime);
		}
	}
	return [conn eval {SELECT last_insert_rowid();}]
}
proc rm_data {rowid} {
	# Removes a row from data with rowid
	conn eval {
		DELETE FROM data WHERE rowid=:rowid;
	}
}
proc get_tree {} {
	# Gets the tree of groups.
	set groups [dict create]
	conn eval {
		SELECT name,gid,pid FROM groups JOIN rel on groups.rowid=rel.gid WHERE depth=1 ORDER by pid,depth;
	} {
		#puts "$name $gid $pid"
		dict set groups $gid [list $name $pid]
	}
	return $groups
}
sqlite3 conn :memory:
build_tables
set cats_gid [add_group cats]
set dogs_gid [add_group dogs]
add_group large $cats_gid
add_group small $cats_gid
conn eval {
	SELECT name,gid FROM groups JOIN rel ON groups.rowid=rel.gid WHERE pid=:cats_gid ORDER BY pid;
} {
puts "$name $gid"
}
puts ----
#del_group $cats_gid
#
#puts DELETED
#conn eval {
#	SELECT name,gid,pid,depth from groups JOIN rel on groups.rowid=rel.gid;
#} {
#	puts "$name $gid $pid $depth"
#}
puts [get_tree]
