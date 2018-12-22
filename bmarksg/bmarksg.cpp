#include "bmarksg.h"
#include <queue>

using std::queue;
using std::pair;

int prepare_helper(sqlite3 *db,sqlite3_stmt **s,const char* str){
	// Helps prepare statements for an database.
	// Basically, it uses the full string without a ptr
	// to remaining chars (of which they are not.)
	return sqlite3_prepare_v2(
		db,	str,	-1,	s,	0
	);
}

bool successful_stmt(sqlite3_stmt *s){
	// Given a statement (with no intended output), repeat
	// step until 'done'. Then it resets the statement.
	int R;
	do {
		R = sqlite3_step(s);
	} while (R==SQLITE_ROW);
	sqlite3_reset(s);
	return R==SQLITE_DONE;
}
DB_Connection::DB_Connection(){
	// will use :memory: in future.
	sqlite3_open("/mnt/ramdisk/db.db",&db);
	create_tables();
	prepare_stmts();
}
DB_Connection::~DB_Connection(){
	finalize_stmts();
	sqlite3_close(db);
}

void DB_Connection::create_tables(){
	// groups.gid is an alias for groups.rowid
	sqlite3_exec(
		db,
		"CREATE TABLE groups(gid INTEGER primary key,name TEXT);"
			"CREATE TABLE rel(gid INTEGER,pid INTEGER,depth INTEGER);"
			"CREATE TABLE data(gid INTEGER,key TEXT,value TEXT,mtime TEXT);",
		0, 0, 0
	);
}
void DB_Connection::prepare_stmts(){
	// Prepares all stmts used by the connection.
	prepare_helper(db,&new_grp,
		"INSERT INTO groups (name) VALUES(?1);");
	prepare_helper(db,&self_rel,
		"INSERT INTO rel (gid,pid,depth) VALUES(?1,?1,0);");
	prepare_helper(db,&root_grp,
		"INSERT INTO rel (gid,pid,depth) VALUES(?1,NULL,1);");
	prepare_helper(db,&child_grp,
		"INSERT INTO rel SELECT ?1,pid,depth+1 FROM rel WHERE rel.gid=?2 AND rel.pid IS NOT NULL;");
	// For deleting groups
	prepare_helper(db,&del_grp,
		"DELETE FROM groups WHERE gid=?1 LIMIT 1;");
	prepare_helper(db,&del_grp_data,
		"DELETE FROM data WHERE gid=?1;");
	prepare_helper(db,&del_grp_rel,
		"DELETE FROM rel WHERE gid=?1");
	prepare_helper(db,&get_child_grp,
		"SELECT gid FROM rel WHERE pid=?1;");
	// Transaction stmts
	prepare_helper(db,&begin_trans,
		"BEGIN TRANSACTION;");
	prepare_helper(db,&end_trans,
		"END TRANSACTION;");
	// Modify stmts
	prepare_helper(db,&set_grp_name,
		"UPDATE groups SET name=?1 WHERE gid=?2;");
	prepare_helper(db,&childNparent,
		"SELECT g.gid,p.pid FROM "
			"(SELECT gid,pid FROM rel WHERE pid is not NULL AND depth=1) AS p "
		"JOIN "
			"(SELECT gid FROM rel WHERE pid=?1 AND depth>0 ORDER BY depth DESC) as g "
		"ON g.gid=p.gid;");
	// about childNparent..
	// LHS returns all non root child-parents pairs.
	// RHS returns all sub-groups of ?1 in breadth order.
	// Joining them returns child-parent pairs of subgroups of gid in breadth order.
}
void DB_Connection::finalize_stmts(){
	// For creating groups.
	sqlite3_finalize(new_grp);
	sqlite3_finalize(self_rel);
	sqlite3_finalize(root_grp);
	sqlite3_finalize(child_grp);
	// For deleting groups.
	sqlite3_finalize(del_grp);
	sqlite3_finalize(del_grp_data);
	sqlite3_finalize(del_grp_rel);
	sqlite3_finalize(get_child_grp);
	// Transaction stmts
	sqlite3_finalize(begin_trans);
	sqlite3_finalize(end_trans);
	// Modify
	sqlite3_finalize(set_grp_name);
	sqlite3_finalize(childNparent);
}
bool DB_Connection::child_link(const int root,const int child){
	// Links the 'child' group under 'root' group
	if (root==0 || root==child || child==0){
		// root and child cannot match nor be the NULL group
		return 0;
	}
	// should check if root is subgroup of child before.
	sqlite3_bind_int(child_grp,1,child);
	sqlite3_bind_int(child_grp,2,root);
	return successful_stmt(child_grp);
}
bool DB_Connection::unlink(const int gid){
	// Deletes all rows in rel table with gid of gid.
	// Since this includes the self relation, it recreates it.
	sqlite3_bind_int(del_grp_rel,1,gid);
	sqlite3_bind_int(self_rel,1,gid);
	return (successful_stmt(del_grp_rel) && successful_stmt(self_rel));
}
int DB_Connection::create_group(const string& name,const int parent_gid){
	// Creates a group with name. If parent_gid is included, create under it.
	// Returns gid of created group.
	trans_act('B');
	sqlite3_bind_text(new_grp,1,name.c_str(),-1,SQLITE_STATIC); // STATIC is okay
	if (!successful_stmt(new_grp)){
		return 0;
	}
	int gid = sqlite3_last_insert_rowid(db);
	sqlite3_bind_int(self_rel,1,gid);
	if (!successful_stmt(self_rel)){
		return 0;
	}
	bool R;
	if (parent_gid!=0){
		// has a parent // not a root group.
		R = child_link(parent_gid,gid);
	} else {
		// a root group // no parent.
		sqlite3_bind_int(root_grp,1,gid);
		R = successful_stmt(root_grp);
	}
	if (!R){
		return 0;
	}
	trans_act('E');
	return gid;
}
void DB_Connection::print_groups(){
	sqlite3_stmt *groups;
	prepare_helper(db,&groups,
		"SELECT gid,name FROM groups;");
	int R = sqlite3_step(groups);
	std::cout << "GID|NAME\n";
	while (R==SQLITE_ROW){
		std::cout 
			<< sqlite3_column_int(groups,0)
			<< '|'
			<< sqlite3_column_text(groups,1)
			<< '\n';
		R = sqlite3_step(groups);
	}
	sqlite3_finalize(groups);
	std::cout << "GID|PID|DEPTH\n";
	sqlite3_stmt *relations;
	prepare_helper(db,&relations,
		"SELECT gid,pid,depth FROM rel;");
	R = sqlite3_step(relations);
	while (R==SQLITE_ROW){
		std::cout
			<< sqlite3_column_int(relations,0)
			<< '|'
			<< sqlite3_column_int(relations,1)
			<< '|'
			<< sqlite3_column_int(relations,2)
			<< '\n';
		R = sqlite3_step(relations);
	}
	sqlite3_finalize(relations);
}

bool DB_Connection::delete_group_helper(const int gid){
	// Deletes a group from:
	// the groups table,
	// its data from the data table,
	// and its relations from the rel table.
	sqlite3_bind_int(del_grp,1,gid);
	sqlite3_bind_int(del_grp_data,1,gid);
	sqlite3_bind_int(del_grp_rel,1,gid);
	if (!successful_stmt(del_grp) ||
		!successful_stmt(del_grp_data) ||
		!successful_stmt(del_grp_rel)
	){
		// If any fails, it will short-circuit and return 0.
		return 0;
	}
	return 1;
}

bool DB_Connection::delete_group(const int gid){
	// Deletes group with gid. If it doesn't exist,
	// nothing will be deleted.
	// If this function returns 0, you should rollback.
	trans_act('B');
	if (!delete_group_helper(gid)){
		return 0;
	}

	sqlite3_bind_int(get_child_grp,1,gid);
	for (
		int R = sqlite3_step(get_child_grp);
		R==SQLITE_ROW;
		R = sqlite3_step(get_child_grp)
	){
		// Loop to delete subgroups.
		if (!delete_group_helper(sqlite3_column_int(get_child_grp,0))){
			return 0;
		}
	}
	sqlite3_reset(get_child_grp);
	trans_act('E');
	return 1;
}
void DB_Connection::trans_act(const char B_E){
	sqlite3_stmt* s = ((B_E=='B') ? (begin_trans) : (end_trans));
	sqlite3_step(s);
	sqlite3_reset(s);
}
bool DB_Connection::rename_group(const int gid,const string& s){
	sqlite3_bind_text(set_grp_name,1,s.c_str(),-1,SQLITE_STATIC);
	sqlite3_bind_int(set_grp_name,2,gid);
	return successful_stmt(set_grp_name);
}
bool DB_Connection::change_parent(const int gid,const int new_par){
	trans_act('B');
	if (new_par==0){
		sqlite3_bind_int(root_grp,1,gid);
		if (!unlink(gid) || successful_stmt(root_grp)){
			return 0;
		}
	} else {
		if (!unlink(gid) || !child_link(new_par,gid)){
			return 0;
		}
	}
	// Above we unlink gid from its current parent
	// Then we create link to root OR new_par depending on new_par.
	// Next we handle subgroups of gid.
	queue<pair<int,int>> cp_pairs;
	sqlite3_reset(childNparent);
	std::cout << sqlite3_bind_int(childNparent,1,gid);
	int R;
	for (R = sqlite3_step(childNparent);
		R == SQLITE_ROW;
		R = sqlite3_step(childNparent)
	){
		cp_pairs.emplace(sqlite3_column_int(childNparent,0),
			sqlite3_column_int(childNparent,1));
	}
	std::cout << "code was " << R << '\n';
	if (R!=SQLITE_DONE){
		return 0;
	}
	int c,p;
	while (!cp_pairs.empty()){
		c = cp_pairs.front().first;
		p = cp_pairs.front().second;
		cp_pairs.pop();
		std::cout << "child-par: " << c << ',' << p << '\n';
		if (!unlink(c) || !child_link(p,c)){
			return 0;
		}
	}
	trans_act('E');
	return 1;
}
