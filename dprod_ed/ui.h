#ifndef D_UI
#define D_UI
#include <vector>
#include "db.h"

namespace RecordEditor {
	void init();
	void freem();
	void reset();
	void hide();
	Record exportr();
	Record exportr(Record&);
	void import(const Record&);
	void show(const unsigned Y,const unsigned X);
	int edit(const unsigned Y,const unsigned X);
	time_t get_time(const bool start1_end0);
}
namespace FilterForm {
	void init();
	void freem();
	void reset();
	void hide();
	void show(const unsigned Y,const unsigned X);
	int edit(const unsigned Y,const unsigned X,SQLi&);
}
static void database_mnip(const unsigned Y,const unsigned X,const size_t l,SQLi&);
int init_viewr();
void nloop();
void cleanup();
#endif
