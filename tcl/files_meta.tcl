#!/usr/bin/env tclsh8.6
package require Tk
package require mysqltcl

namespace eval Gui {
	namespace eval v {
		variable createAsRootGroup 0 tagById {} idOfTag {} rowTagList {}
		### fix::name tags is a int->string and tags_id is string->ing	### fixed
	}
	proc initMenu {} {
		menu .men
		menu .men.app
		menu .men.new
		menu .men.edit
		menu .men.actions
		.men.app add command -label {Load} -command {}
		.men.app add command -label {Unload} -command {}
		.men.app add command -label {Reload} -command mainTreeview::reloadFromDBO
		.men.app add command -label {Quit} -command Gui::quitProgram
		.men.new add command -label {New Group} -command Gui::createNewGroupWindow
		.men.new add command -label {New Tag} -command Gui::createNewTagDialog
		.men.new add command -label {New Action} -command newActionWindow::open
		.men.edit add command -label {Modify} -command modifyWindow::open
		.men.edit add command -label {Delete} -command mainTreeview::delete
		.men add cascade -label {App} -menu .men.app
		.men add cascade -label {New} -menu .men.new
		.men add cascade -label {Edit} -menu .men.edit
		.men add command  -label {View} -command Gui::createViewWindow
		.men add cascade  -label {Actions} -menu .men.actions
		. configure -menu .men
	}
	proc tagSelectFrame {name} {
		pack [frame $name]
		pack [labelframe $name.left -text Selected] -side left -expand 1 -fill both
		pack [listbox $name.left.lb -width 16 -selectmode extended]
		pack [frame $name.buttons] -side left
		pack [button $name.buttons.add -text {<} -command [list Gui::tagSelectFrame.AddLeft $name]]
		pack [button $name.buttons.rem -text {>} -command [list Gui::tagSelectFrame.RemLeft $name]]
		pack [labelframe $name.right -text {All Tags}] -side left -expand 1 -fill both
		pack [listbox $name.right.lb -width 16 -selectmode extended]
		return $name
	}
	proc tagSelectFrame.AddLeft {name} {
		foreach i [lreverse [$name.right.lb curselection]] {
			$name.left.lb insert end [$name.right.lb get $i]
			$name.right.lb delete $i
		}
	}
	proc tagSelectFrame.RemLeft {name} {
		foreach i [lreverse [$name.left.lb curselection]] {
			$name.right.lb insert end [$name.left.lb get $i]
			$name.left.lb delete $i
		}
	}
	proc tagSelectFrame.init {frameWidget rid} {
		# Fills the tagSelectFrame widget with all possible tags
		set selected [list]
		if {[dict exist $v::rowTagList $rid]} {
			set selected [dict get $v::rowTagList $rid]
		}
		dict for {id val} $v::tagById {
			if {[lsearch $selected $id] == -1} {
				$name.right.lb insert end $val
			} else {
				$name.left.lb insert end $val
			}
		}
	}
	proc tagSelectFrameSingle {name} {
		# Creates a tag selection frame but with only a single listbox
		pack [frame $name] -expand 1 -fill both
		pack [labelframe $name.tags -text Tags] -expand 1 -fill both
		pack [listbox $name.tags.lb -width 16 -selectmode multiple -yscrollcommand [list $name.tags.sb set]] -side left -expand 1 -fill both
		pack [scrollbar $name.tags.sb -command [list $name.tags.lb yview]] -side left -fill y
		return $name
	}
	proc tagSelectFrameSingle.init {frameWidget rid} {
		# Fills the tagSelectFrame widget with all possible tags
		# where left are tags for the left side
		set selected [list]
		if {[dict exist $v::rowTagList $rid]} {
			set selected [dict get $v::rowTagList $rid]
		}
		dict for {id val} $v::tagById {
			$frameWidget.tags.lb insert end $val
			if {[lsearch $selected $id] != -1} {
				# if the item was selected, select in the listbox
				$frameWidget.tags.lb selection set end
			}
		}
	}
	proc createNewTagDialog {} {
		if {{.win} in [winfo children .]} { return }
		toplevel .win 
		wm attributes .win -topmost 1
		wm resizable .win 0 0
		wm title .win {New Tag}
		pack [labelframe .win.tag -text Tag] -expand 1 -fill x
		pack [entry .win.tag.e] -expand 1 -fill x
		pack [button .win.cancel -text Cancel -command {destroy .win}] -side right
		pack [button .win.new -text New -command Gui::createNewTagDialog.save] -side right
	}
	proc createNewTagDialog.save {} {
		set tag [.win.tag.e get]
		if {[string length $tag] > 15} {
			if {[tk_dialog .win_tcate {Too many characters} {Truncate tag to 15 chars?} questhead 0 Yes No] == 1} {
				return
			}
			set tag [string range $tag 0 15]
		}
		if {[dict exists $Gui::v::idOfTag $tag]} {
			tk_messageBox -detail {Tag already exists} -icon error -parent .win -title Error -type ok
			return
		}
		set r [DBO createTag $tag]
		dict append Gui::v::tagById $r $tag
		dict append Gui::v::idOfTag $tag $r
		destroy .win
	}
	proc createNewGroupWindow {} {
		if {{.win} in [winfo children .]} { return }
		toplevel .win
		wm attributes .win -topmost 1
		wm resizable .win 0 0
		wm title .win {New Group}
		pack [labelframe .win.name -text Name] -expand 1 -fill x
		pack [entry .win.name.e] -side left -expand 1 -fill x
		pack [labelframe .win.dir -text Directory] -expand 1 -fill x
		pack [entry .win.dir.e] -side left  -expand 1 -fill x
		pack [button .win.dir.select -text Open -command {Gui::setEntry .win.dir.e [tk_chooseDirectory]}]
		pack [checkbutton .win.grp -text {Create at Root} -variable Gui::v::createAsRootGroup]

		pack [button .win.cancel -text Cancel -command {destroy .win}] -side right
		pack [button .win.new -text New -command Gui::createNewGroupWindow.save] -side right
	}
	proc createNewGroupWindow.save {} {
		set name [.win.name.e get]
		if {[string length $name]==0} {
			tk_messageBox -type ok -icon error -title Error -message {Group name cannot be empty.}
			return
		}
		set folder [.win.dir.e get]
		if {[string length $folder] > 0 && ! [file isdirectory $folder] } {
			 tk_messageBox -type ok -icon error -title Error -message {Folder doesn't exist.}
			 return
		}
		if {$Gui::v::createAsRootGroup} {
			set parent 0
		} else {
			set parent [mainTreeview::getSelectedGroup]
		}
		set gid [DBO addGroupFromDirMaybe $name $folder $parent]
		if {$gid != 0} {
			mainTreeview::createRoot $gid $name [expr {$Gui::v::createAsRootGroup==1? {} : $parent }]
			mainTreeview::createLeaves $gid
			destroy .win
		}
	}
	proc setEntry {e text} {
		$e delete 0 end
		$e insert 0 $text
	}
	proc loadTagsFromDatabase {} {
		# Load tags as dict
		set Gui::v::tagById [dict create]
		set Gui::v::idOfTag [dict create]
		DBO tagAccumulate Gui::v::tagById Gui::v::idOfTag
	}
	proc quitProgram {} {
		DBO close
		puts {Ended App}
		destroy .
	}
	proc createViewWindow {} {
		# Creates a window with details on item with id itemId
		set itemId [.links.fnames focus]
		if {[string length $itemId] == 0 || [string match {[1-9]} $itemId]} {
			tk_messageBox -detail {Please select a filename to view} -icon error -title Error -type ok
			return
		}
		if [winfo exists .v$itemId] {
			return
		}
		toplevel .v$itemId
		set item [.links.fnames item $itemId -text]
		lassign [.links.fnames item $itemId -values] views tags
		wm resizable .v$itemId 0 0
		wm title .v$itemId "Item $itemId"
		pack [labelframe .v$itemId.name -text Filename] -expand 1 -fill x
		pack [label .v$itemId.name.l -text $item]
		pack [labelframe .v$itemId.views -text Views] -expand 1 -fill x
		pack [label .v$itemId.views.l -text $views]
		pack [labelframe .v$itemId.tags -text Tags] -expand 1 -fill x
		pack [label .v$itemId.tags.l -text $tags]
		pack [button .v$itemId.exit -text Close -command [list destroy .v$itemId]]
	}
}
namespace eval mainTreeview {
	namespace eval v {
		variable unloadedGroups {} loadedGroups {} actions [list]
	}
	proc init {} {
		# Initializes the mainTreeview widget .links
		pack [frame .links] -fill both -expand 1
		pack [ttk::treeview .links.fnames -columns {views tags} -yscrollcommand {.links.fnames_sb set}] -fill both -expand 1 -side left
		pack [scrollbar .links.fnames_sb -command {.links.fnames yview}] -side left -fill y
		foreach {c l w} [list #0 Group/Name 128 views Views 128 tags Tags 128] {
			# this loop adjusts the treeview's columns
			.links.fnames heading $c -text $l -anchor w
			.links.fnames column $c -minwidth 16 -width $w
			incr i
		}
		bind .links.fnames <<TreeviewOpen>> {
			set gid [.links.fnames focus]
			if {[dict exists $mainTreeview::v::unloadedGroups $gid]} {
				mainTreeview::createLeaves $gid
			}
		}

		## not actually used?
		#bind .links.fnames <<TreeviewSelect>> {
		#	set id [.links.fnames focus]
		#}
	}
	proc createDummy {id} {
		# Creates a dummy load item for group
		.links.fnames insert $id end -id d$id -text {(load)}
	}
	proc createRoot {id name {parent {}}} {
		# need to check if parent in .tv_links
		# ideally would accept a list of:
		# group_id name
		if {[.links.fnames exists $parent]} {
			dict append v::unloadedGroups $id $name
			.links.fnames insert $parent end -id $id -text $name
			createDummy $id
		}
	}
	proc reloadFromDBO {} {
		mainTreeview::createRoots
		fillActionsMenu
	}
	proc createRoots {} {
		# Loads only root groups and creates on demand
		pack forget .links.fnames
		.links.fnames delete [.links.fnames children {}]
		set v::loadedGroups [dict create]
		set v::unloadedGroups [dict create]
		set v::rowTagList [dict create]
		DBO forGroups mainTreeview::createRoot
		Gui::loadTagsFromDatabase
		pack .links.fnames -fill both -expand 1 -before .links.fnames_sb -side left
	}
	proc createLeaves {gid} {
		# Loads subgroups & filenames from a group
		# First delete dummy item
		.links.fnames delete d$gid
		# Get subgroups
		DBO forGroups mainTreeview::createRoot $gid
		# Get filenames and tags
		DBO forFilenames mainTreeview::createLeaf $gid

		dict append v::loadedGroups $gid [dict get $v::unloadedGroups $gid]
		dict unset v::unloadedGroups $gid
	}
	proc createLeaf {id name views tags {parent {}} } {
		# need to check if parent exists in widget first
		# ideally would accept a list of:
		# rownumber filename views parent
		if {[.links.fnames exists $parent]} {
			if {[string length $tags]>0} {
				dict append Gui::v::rowTagList $id [split $tags {,}]
			}
			set tagCol [list]
			if {[dict exist $Gui::v::rowTagList $id]} {
				foreach tagId [dict get $Gui::v::rowTagList $id] {
					lappend tagCol [dict get $Gui::v::tagById $tagId]
				}
			}
			return [.links.fnames insert $parent end -id $id -text $name -value [list $views [join $tagCol {, }]]]
		}
	}
	proc fillActionsMenu {} {
		if {[llength $v::actions] > 0} {
			.men.actions delete 0 [llength $v::actions]
			set v::actions [list]
		}
		DBO forActions mainTreeview::addActionMenuItem
	}
	proc addActionMenuItem {name cmd} {
		set id [llength $v::actions]
		lappend v::actions $cmd
		.men.actions add command -label $name -command "mainTreeview::actionMenuInvoke $id"
	}
	proc actionMenuInvoke {id} {
		puts "You called $id!~"
		set cmd [lindex $v::actions $id]
		foreach rid [.links.fnames selection] {
			puts [string map [list %1 [.links.fnames item $rid -text]] $cmd]
		}
	}
	proc getSelectedGroup {} {
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
	proc updateLeaf {rowId newName updatedViews newTagCol} {
		# Updates (leaf) row of treeview with new values
		.links.fnames item $rowId -text $newName -values [list $updatedViews $newTagCol]
	}
	proc delete {} {
		# Deletes the focused items. Prompt for groups.
		# If already deleted in same run, ignore
		set deleteList [.links.fnames selection]
		foreach d $deleteList {
			if {[.links.fnames exists $d]} {
				if {[string match {[1-9]} $d]} {
					set r [tk_messageBox -type yesnocancel -icon question -title Delete -message {Delete group? (yes/no) Or stop? (cancel)}]
					if {$r == {yes}} {
						# delete all its children from rowTagList
						deleteGroupChildren $d
					} elseif {$r == {no}} {
						continue
					} elseif {$r == {cancel}} {
						return
					}
				}
				.links.fnames delete $d
				puts $d
			}
		}
	}
	proc deleteGroupChildren {id} {
		# Removes children of group
		foreach c [.links.fnames children $id] {
			puts {TBD}
		}
	}
}
namespace eval modifyWindow {
	variable updatedViews 0 views 0 updatedGroup 0 group 0
	proc open {} {
		set rid [.links.fnames focus]
		if {[string length $rid] == 0 || [string match {[1-9]} $rid]} {
			tk_messageBox -detail {Please select a filename to modify} -icon error -title Error -type ok
			return
		}
		create $rid
		setup $rid
	}
	proc create {rid} {
		toplevel .win 
		wm attributes .win -topmost 1
		wm resizable .win 0 0
		wm title .win {Modify}
		pack [labelframe .win.name -text Name] -expand 1 -fill x
		pack [entry .win.name.e] -expand 1 -fill x
		### TODO: need validation to remove "'
		pack [labelframe .win.group -text Group] -expand 1 -fill x
		pack [ttk::combobox .win.group.cb -values [concat [dict values $mainTreeview::v::unloadedGroups] [dict values $mainTreeview::v::loadedGroups]]] -expand 1 -fill x
		pack [labelframe .win.views -text Views] -expand 1 -fill x
		pack [spinbox .win.views.sb -from 0 -to 32767 -textvariable modifyWindow::updatedViews] -expand 1 -fill x
		#pack [tagSelectFrame .win.tags]
		pack [Gui::tagSelectFrameSingle .win.tags]
		pack [frame .win.actions]
		pack [button .win.actions.save -text Save -command [list modifyWindow::save .win.tags $rid]] -side left
		pack [button .win.actions.cancel -text Cancel -command {destroy .win}] -side left
		bind .win <Escape> {
			destroy .win
		}
	}
	proc setup {rid} {
		# Prepares modify window for usage
		set modifyWindow::views [lindex [.links.fnames item $rid -values] 0]
		set modifyWindow::updatedViews $modifyWindow::views
		#Gui::tagSelectFrame.init .win.tags $rid
		Gui::tagSelectFrameSingle.init .win.tags $rid
		.win.name.e insert 0 [.links.fnames item $rid -text]
		.win.group.cb set [dict get $mainTreeview::v::loadedGroups [.links.fnames parent $rid]]
	}
	proc save {widgetFrame rid} {
	# Saves modify changes
		set changes [dict create]
		set newName [.win.name.e get]
		if {$newName != [.links.fnames item $rid -text]} {
			puts "changing name to $newName"
			dict set changes {name} $newName
		}
		if {$modifyWindow::updatedViews != $modifyWindow::views} {
			puts "$modifyWindow::views -> $modifyWindow::updatedViews"
			## needs sql update here
			dict set changes {views} $modifyWindow::updatedViews
		}
		## TAGs
		set previousTags [list]
		if {[dict exist $Gui::v::rowTagList $rid]} {
			set previousTags [dict get $Gui::v::rowTagList $rid]
		}
		set selectedTags [list]
		set newTagCol [list]
		foreach selected [$widgetFrame.tags.lb curselection] {
			# convert tags as string to their enum number
			set s [$widgetFrame.tags.lb get $selected]
			lappend newTagCol $s
			lappend selectedTags [dict get $Gui::v::idOfTag $s]
		}
		## TAGs
		dict set Gui::v::rowTagList $rid $selectedTags
		#.links.fnames item $rid -text $newName -values [list $modifyWindow::updatedViews [join $newTagCol {, }]]
		mainTreeview::updateLeaf $rid $newName $modifyWindow::updatedViews [join $newTagCol {, }]
		
		set tagDiff [tagChangeSet $previousTags $selectedTags]
		# the below needs to be seperated
		DBO updateFilename $rid $changes
		DBO updateFilenameTags $rid $tagDiff
	}
	proc tagChangeSet {old new} {
		set diff [dict create]
		foreach i $new {
			dict append diff $i +
		}
		foreach i $old {
			if {[dict exists $diff $i]} {
				dict unset diff $i
			} else {
				dict append diff $i -
			}
		}
		return $diff
	}
}
namespace eval newActionWindow {
	namespace eval v {
	}
	proc open {} {
		create
	}
	proc create {} {
		# Create the newActionWindow
		toplevel .win
		wm attributes .win -topmost 1
		wm resizable .win 0 0
		wm title .win {Create new action..}
		pack [labelframe .win.label -text Label] -expand 1 -fill x
		pack [entry .win.label.e] -expand 1 -fill x
		pack [labelframe .win.cmd -text Command] -expand 1 -fill x
		pack [entry .win.cmd.e] -expand 1 -fill x
		pack [frame .win.actions]
		pack [button .win.actions.browse -text Browse -command {newActionWindow::browse}] -side left
		pack [button .win.actions.save -text Save -command {newActionWindow::save}] -side left
		pack [button .win.actions.cancel -text Cancel -command {destroy .win}] -side left
	}
	proc browse {} {
		.win.cmd.e delete 0 end
		.win.cmd.e insert 0 [tk_getOpenFile]
	}
	proc save {} {
		if {[string length [set label [.win.label.e get]]] == 0} {
			tk_messageBox -detail {Label cannot be blank!} -icon error -parent .win -title Error -type ok
			return
		}
		if {[string first {%1} [set cmds [.win.cmd.e get]]] == -1} {
			tk_messageBox -detail {Command string missing %1 !} -icon error -parent .win -title Error -type ok
			return
		}
		set aid [DBO createAction $label $cmds]
		mainTreeview::addActionMenuItem $label $cmds
		destroy .win
	}
}

package require TclOO
oo::class create db {
	constructor {} {
		my variable dbhandle
		set dbhandle {}
	}
	method login {user pass} {
	# Logs into mysql
		my variable dbhandle
		if {[catch {set dbhandle [mysql::connect -user $user -pass $pass]}] != 0} {
			set dbhandle {}
		}
	}
	method createNewDatabase {dbname} {
		# Creates database and populates it with default tables
		######### NEEDS TESTING
		my variable dbhandle
		mysql::exec $dbhandle "create database if not exists $dbname"
		mysql::use $dbhandle $dbname
		my genBaseTables
		my genSpecialTables
		my genActionTable
	}
	method genActionTable {} {
		# Creates the action table
		my variable dbhandle
		mysql::exec $dbhandle {CREATE TABLE if not exists actions(aid int auto_increment primary key,label char(32) NOT NULL,cmdstr char(255) NOT NULL)}
	}
	method getHandle {} {
		# transitional method, will be removed later
		my variable dbhandle
		return $dbhandle
	}
	method genBaseTables {} {
		# Creates the base tables in database and procedures
		my variable dbhandle
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
	method genSpecialTables {} {
		# Creates tables specific to this application
		my variable dbhandle
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
	method forGroups {action {pgid {}}} {
		my variable dbhandle
		set where {pid is null}
		if {$pgid!={}} {
			set where [format {pid=%d and depth=1} $pgid]
		}
		mysql::receive $dbhandle [concat {select rel.gid,name from groups join rel on groups.gid=rel.gid where } $where] [list gid name] {
			$action $gid $name $pgid
		}
	}
	method forFilenames {action gid} {
		my variable dbhandle
		mysql::receive $dbhandle "select id,name,views,group_concat(tagid) from filenames left outer join tag_map on id=fileid where gid=$gid group by id order by id" [list id name views tags] {
			# tags here is a comma seperated list of ints
			$action r$id $name $views $tags $gid
		}
	}
	method forActions {doAction} {
		my variable dbhandle
		mysql::receive $dbhandle {select label,cmdstr from actions} [list label cmdstr] {
			$doAction $label $cmdstr
		}
	}
	method tagAccumulate {tagById idOfTag} {
		my variable dbhandle
		mysql::receive $dbhandle {select id, tag from tags} [list id tag] {
		dict append $tagById $id $tag
		dict append $idOfTag $tag $id
		}
	}
	method close {} {
		my variable dbhandle
		mysql::close $dbhandle
	}
	method addGroup {name {parent 0}} {
		my variable dbhandle
		set l [mysql::sel $dbhandle "call new_group('$name',$parent)" -flatlist]
		puts "Created group $name with parent $parent"
		return [lindex $l 0]
	}

	method addFilename {fname {group 0}} {
		my variable dbhandle
		set r [mysql::sel $dbhandle "call add_filename('$fname',$group)" -flatlist]
		return [lindex $r 0]
	}

	method addFilenamesFromDir {fdir {group 0}} {
		# Adds filenames in fdir to group
		set added [list]
		my variable dbhandle
		foreach f [glob -tails -types f -directory $fdir *] {
			lappend added $f
			lappend [my addFilename $f $group]
		}
		return $added
	}

	method addGroupFromDir {fdir name {group 0}} {
		# Adds a directory and its files to group
		my variable dbhandle
		if {! [file isdirectory $fdir]} { return 0 }
		set gid [my addGroup $name $group]
		puts "Made group $name with gid $gid parent $group"
		set filenames [my addFilenamesFromDir $fdir $gid]
		return $gid
	}
	method addGroupFromDirMaybe {name fdir {group 0}} {
		my variable dbhandle
		if {[string length $fdir]==0} {
			return [my addGroup $name $group]
		} else {
			return [my addGroupFromDir $fdir $name $group]
		}
	}
	method remGroup {d_gid} {
		my variable dbhandle
		mysql::exec $dbhandle "call delete_group($d_gid)"
	}
	method createTag {tag} {
		my variable dbhandle
		mysql::exec $dbhandle "insert into tags (tag) value ('$tag')"
		set r [mysql::sel $dbhandle {select last_insert_id()} -flatlist]
		return $r
	}
	method updateFilename {rid changes} {
		# where changes is a dict
		my variable dbhandle
		set id [string range $rid 1 end]
		set t [list]
		if {[dict exists $changes {name}]} {
			lappend t [format {name="%s"} [dict get $changes {name}]]
		}
		if {[dict exists $changes {views}]} {
			lappend t [format {views=%d} [dict get $changes {views}]]
		}
		if {[dict exists $changes {gid}]} {
			lappend t [format {gid=%d} [dict get $changes {gid}]]
		}
		if {[llength $t]>0} {
			mysql::exec $dbhandle [concat {update filenames set} [join $t {, }] "where id=$id"]
		}
	}
	method updateFilenameTags {rid changeSet} {
		# where changeSet is a dict of tagId : action
		my variable dbhandle
		set id [string range $rid 1 end]
		mysql::exec $dbhandle {BEGIN}
		dict for {tagId action} $changeSet {
			if {$action eq {-}} {
				mysql::exec $dbhandle [format {delete from tag_map where fileid=%d and tagid=%d} $id $tagId] 
			} else {
				mysql::exec $dbhandle [format {insert into tag_map (fileid,tagid) values (%d,%d)} $id $tagId]
			}
		}
		mysql::commit $dbhandle
	}
	method createAction {name cmds} {
		my variable dbhandle
		mysql::exec $dbhandle [format {insert into actions (label,cmdstr) value ('%s', '%s')} $name $cmds]
		set aid [mysql::sel $dbhandle {select last_insert_id()} -flatlist]
		return $aid
	}
}

proc open_database {username password database} {
	# Returns dbhandle if params result in a correct open
	if {[catch {set dbhandle [mysql::connect -user $username -pass $password -db $database]}] != 0} {
		return {}
	}
	return $dbhandle
}

mainTreeview::init
Gui::initMenu

set user $env(user)
set pass $env(pass)
db create DBO
DBO login $user $pass
DBO createNewDatabase {files_meta}
