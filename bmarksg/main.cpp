#include "bmarksg.h"

int main(){
	DB_Connection db;
	int A = db.create_group("alpha");
	int B = db.create_group("beta");
	int C = db.create_group("charlie",A);
	db.create_group("delta",C);
	db.print_groups();
	db.change_parent(C,B);
	db.print_groups();
	return 0;
}
