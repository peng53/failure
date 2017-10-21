# hierarch_naive.tcl
package require sqlite3

namespace eval HierarchUrl {
	# Desired operations and how:
	# Add URL ok (add_url)
	# Add Group ok (add_group)
	# Edit URL (name,url,group)
	# Edit Group (name)
	# Delete URL ok (delete_url)
	# Delete Group (and children URL) ok (delete_group)
	# Get all URL ok (get_urls)
	# Get all URL under a group ok? (get_urls_tree)

	proc open_db {db filename} {
		# Open a database with filename and bind to db
		sqlite3 $db $filename
	}
	proc build_db {db filename} {
		# Builds a database with filename bound to db
		if {[file exists $filename]} { return -1 }
		open_db $db $filename
		$db transaction {
			$db eval {CREATE TABLE urls(rowid integer primary key autoincrement,name string,url string,gid integer)}
			$db eval {CREATE TABLE groups(rowid integer primary key autoincrement,name string,parent integer)}
		}
		return 1
	}
	proc add_url {db name url {gid NULL}} {
		# Insert a url to urls table.
		$db eval {INSERT into urls VALUES(null,:name,:url,:gid)}
	}
	proc add_group {db name {parent NULL}} {
		# Insert a group to groups table.
		$db eval {INSERT into groups VALUES(null,:name,:parent)}
	}
	proc get_groups {db} {
		# Get all rows in groups.
		set rs [list]
		$db eval {SELECT * from groups} {
			lappend rs [list $rowid $name $parent]
		}
		return $rs
	}
	proc get_tree {db gid} {
		# Returns all groups of which gid is ancestor to
		set gs [list $gid]
		set gi 0
		set gl 1
		while {$gi<$gl} {
			foreach i [lrange $gs $gi $gl] {
				$db eval {SELECT rowid from groups WHERE parent=:i} {
					lappend gs $rowid
				}
			}
			set gi $gl
			set gl [llength $gs]
		}
		return $gs
	}
	proc get_urls {db {gid -1}} {
		# Get all urls, with gid if included.
		set rs [list]
		set s {SELECT * from urls}
		if {$gid!=-1} { append s { WHERE gid=:gid} }
		$db eval $s {
			lappend rs [list $rowid $name $gid]
		}
		return $rs
	}
	proc get_urls_tree {db gid} {
		# Returns all rows from urls under group recursively.
		set gs [get_tree $db $gid]
		set rs [list]
		foreach i $gs {
			$db eval {SELECT * from urls WHERE gid=:i} {
				lappend rs [list $rowid $name $gid]
			}
		}
		return $rs
	}
	proc delete_url {db rowid} {
		# Deletes a row in urls with rowid
		$db eval {DELETE from urls WHERE rowid=:rowid}
	}
	proc delete_group {db gid} {
		# Deletes a group and all its children.
		set gs [get_tree $db $gid]
		foreach i $gs {
			$db eval {DELETE from groups where rowid=:i}
			$db eval {DELETE from urls WHERE gid=:i}
		}
	}
}