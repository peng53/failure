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
proc change_pgroup {gid {new_pid 0}} {
	# Changes the parent group of gid to new_pid.
	# Assume root if 0.
	# ATM branch completely if root
	if {$new_pid==0} {
		# Replace gid's relation with one to root
		conn eval {
			DELETE FROM rel WHERE gid=:gid AND depth>0;
			INSERT INTO rel VALUES(NULL,:gid,NULL,1);
		}
		# Get subgroups whose relations should also change
		set children [conn eval {
			SELECT gid FROM rel WHERE pid=:gid AND depth>0 ORDER BY depth DESC;
		}]
		set ch_parents [list]
		# Get the subgroups intermediate parent
		foreach c $children {
			conn eval {
				SELECT pid FROM rel WHERE gid=:c AND depth=1 LIMIT 1;
			} {
				lappend ch_parents $pid
				puts "pid $pid"
				puts "ch_parents is: $ch_parents"
			}
			#puts [lappend ch_parent [conn eval {
			#	SELECT pid FROM rel WHERE gid=:c;
			#}
			#
		}
		puts {children were:}
		puts $children
		puts {parents were:}
		puts $ch_parents
		lmap c $children p $ch_parents {
			# For each subgroup, clear all relations except identity
			# Then re-associate with its previous parent
			puts "c= $c | p= $p"
			conn eval {
				DELETE FROM rel WHERE gid=:c AND depth>0;
				INSERT INTO rel SELECT NULL,:c,pid,depth+1 FROM rel WHERE gid=:p AND pid IS NOT NULL;
			}
		}
	}
}
sqlite3 conn :memory:
build_tables
set cats_gid [add_group cats]
set dogs_gid [add_group dogs]
set large_gid [add_group large $cats_gid]
add_group small $cats_gid
add_group tammy $large_gid
#conn eval {
#	SELECT name,gid FROM groups JOIN rel ON groups.rowid=rel.gid WHERE pid=:cats_gid ORDER BY pid;
#} {
#puts "$name $gid"
#}
#puts ----
#del_group $cats_gid
#
#puts DELETED
#conn eval {
#	SELECT name,gid,pid,depth from groups JOIN rel on groups.rowid=rel.gid;
#} {
#	puts "$name $gid $pid $depth"
#}
#puts [get_tree]
change_pgroup $large_gid
conn eval {
	SELECT name,groups.rowid as rid,pid,depth FROM groups JOIN rel ON groups.rowid=rel.gid ORDER BY pid ASC;
} {
	puts "$name | $rid | $pid | $depth"
}
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
		SELECT name,gid,pid,depth FROM groups JOIN rel ON groups.rowid=rel.gid WHERE rel.pid=:g;
	} {
		puts [format {%10s | %3d | %3d | %3d} $name $gid $pid $depth]
#		puts "$name | $gid | $pid | $depth"
	}
	puts ---------------
}
# First we made this tree:
# {
# Cats
# 	* Large
# 		* Tammy
# 	* Small
# Dogs
# }
#
# Then we moved large to be a root child
# {
# Cats
# 	* Small
# Dogs
# Large
# 	* Tammy
# }
# Should be the result
#
