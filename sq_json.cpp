#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sqlite3.h>

using namespace std;

void get_tprop(string& s,fstream& f){
	char ch;
	while ((f >> noskipws >> ch) && ch!='"'){
		continue;
	}
	while ((f >> noskipws >> ch) && ch!='"'){
		s += ch;
	}
}
void get_dprop(string& s,fstream& f){
	char ch;
	while ((f >> noskipws >> ch) && ch<'0' && ch>'9'){
		continue;
	}
	while ((f >> noskipws >> ch) && ch>='0' && ch<='9'){
		s+=ch;
	}
}

int main(){
	char ch;
	fstream myfile("bookmarks-2018-07-07.json",fstream::in);
	string t_str;
	string title = "Bookmarks";
	string uri;
	string mtime;
	vector<int> gids;

	sqlite3* db;
	sqlite3_open("/mnt/ramdisk/wakaa.db",&db);
	sqlite3_exec(db,"CREATE TABLE groups(rowid INTEGER primary key,name TEXT);CREATE TABLE rel(rowid INTEGER primary key, gid INTEGER,pid INTEGER,depth INTEGER);CREATE TABLE data(gid INTEGER,key TEXT,value TEXT,mtime TEXT);",NULL,NULL,NULL);
	sqlite3_stmt *new_grp;
	sqlite3_prepare_v2(db,"INSERT INTO groups (name) VALUES(?1)",-1,&new_grp,0);
	sqlite3_stmt *new_data;
	sqlite3_prepare_v2(db,"INSERT INTO data VALUES(?1,?2,?3,?4)",-1,&new_data,0);
	sqlite3_stmt *rel1;
	sqlite3_prepare_v2(db,"INSERT INTO rel VALUES(NULL,?1,?1,0)",-1,&rel1,0);
	sqlite3_stmt *rel_null;
	sqlite3_prepare_v2(db,"INSERT INTO rel VALUES(NULL,?1,NULL,1);",-1,&rel_null,0);
	sqlite3_stmt *rel_par;
	sqlite3_prepare_v2(db,"INSERT INTO rel SELECT NULL,?1,pid,depth+1 FROM rel WHERE rel.gid=?2 AND rel.pid IS NOT NULL",-1,&rel_par,0);

	while (myfile >> noskipws >> ch){
		switch (ch){
			case '"':
				myfile >> noskipws >> ch;
				if (ch=='t'){ // title?
					while ((myfile >> noskipws >> ch) && ch!='"'){
						t_str += ch;
					}
					if (t_str.compare("itle")==0){
						title = "";
						get_tprop(title,myfile);
						//cout << "got title " << title << '\n';
					}
				} else if (ch=='u'){ //uri?
					while ((myfile >> noskipws >> ch) && ch!='"'){
						t_str += ch;
					}
					if (t_str.compare("ri")==0){ // add url
						uri = "";
						get_tprop(uri,myfile); // should also have title by now
						//if (gids.size()>0){
							sqlite3_bind_int(new_data,1,gids.back());
						//}
						sqlite3_bind_text(new_data,2,title.c_str(),-1,SQLITE_TRANSIENT); //title
						sqlite3_bind_text(new_data,3,uri.c_str(),-1,SQLITE_TRANSIENT); //uri
						//sqlite3_bind_text(new_data,4,mtime.c_str(),-1,SQLITE_TRANSIENT); //uri
						//sqlite3_bind_int(new_data,4,atoll(mtime.c_str())); //uri
						sqlite3_step(new_data);
						//cout << "t: " << title
						//	<< "\nu: " << uri << "\n-\n";
						sqlite3_reset(new_data);
					}
				} else if (ch=='c'){
					while ((myfile >> noskipws >> ch) && ch!='"'){
						t_str += ch;
					}
					if (t_str.compare("hildren")==0){
						sqlite3_bind_text(new_grp,1,title.c_str(),-1,SQLITE_TRANSIENT);
						sqlite3_step(new_grp);
						sqlite3_reset(new_grp);
						gids.push_back(sqlite3_last_insert_rowid(db));
						cout << "made group " << gids.back() << '\n';
						sqlite3_bind_int(rel1,1,gids.back());
						sqlite3_step(rel1);
						sqlite3_reset(rel1);

						if (gids.size()==1){ //this is the only group..
							sqlite3_bind_int(rel_null,1,gids.back());
							sqlite3_step(rel_null);
							sqlite3_reset(rel_null);
						} else { // has parent
							sqlite3_bind_int(rel_par,1,gids.back());
							sqlite3_bind_int(rel_par,2,gids[gids.size()-2]);
							int r=1;
							while (r==1){
								r = sqlite3_step(rel_par);
								cout << r << '\n';
								if (r==SQLITE_DONE){
									r = 0;
								} else {
									r = 1;
								}
							}
							sqlite3_reset(rel_par);
						}
					}
				} else if (ch=='a'){
					while ((myfile >> noskipws >> ch) && ch!='"'){
						t_str += ch;
					}
					if (t_str.compare("nnos")==0){
						while ((myfile >> noskipws >> ch) && ch!=']'){
							continue;
						}
					}
				//} else if (ch=='l'){
				//	while ((myfile >> noskipws >> ch) && ch!='"'){
				//		t_str += ch;
				//	}
				//	if (t_str.compare("astModified")==0){
				//		mtime = "";
				//		get_dprop(mtime,myfile);

				//	}
				}

				t_str = "";
				break;
			case ']':
				cout << "finished gid: " << gids.back() << '\n';
				gids.pop_back();
				break;

		}
	}
	/*
	sqlite3_stmt *read_data;
	sqlite3_prepare_v2(db,"SELECT gid,key,value FROM data;",-1,&read_data,0);
	while (sqlite3_step(read_data)==SQLITE_ROW){
		cout << sqlite3_column_int(read_data,0);
		cout << '|' << sqlite3_column_text(read_data,1);
		cout << '|' << sqlite3_column_text(read_data,2);
		cout << '\n';

	}
	sqlite3_finalize(read_data);
	*/

	sqlite3_finalize(new_grp);
	sqlite3_finalize(new_data);
	sqlite3_finalize(rel1);
	sqlite3_finalize(rel_null);
	sqlite3_finalize(rel_par);
	return 0;
}
