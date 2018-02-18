#ifndef D_UI
#define D_UI
#include "db.h"

namespace RecordEditor {
	/**
	 * An editor of a Record. Contains fields for UID, code, start time,
	 * end time, and description. An user can edit said fields and save/
	 * discard input by pressing enter/escape at the 'save button'.
	 */
	void init(); //< Should be used if RecordEditor is desired
	void freem(); //< Should be used when RecordEditor is no longer used.
	void reset(); //< Reset form fields to default values
	void hide(); //< Hides RecordEditor from view
	Record exportr(); //< Returns a Record obj from data in RecordEditor
	Record exportr(Record&); //< Modifies a Record obj so it matches RecordEditor
	void import(const Record&); //< Copies fields from a Record to RecordEditor
	void show(const unsigned Y,const unsigned X); //< Displays RecordEditor
	int edit(const unsigned Y,const unsigned X); //< Displays RecordEditor and begins user interaction
	time_t get_time(const bool start1_end0); //< Return the time from either 'start' or 'date' date
}
namespace FilterForm {
	/**
	 * A 'picker' for filtering a database. Allows users to filter
	 * only entries matching: UID, code, start time, or end time.
	 * The filter is applied when user presses 'enter' on the last field
	 * or 'escape-enter' on any field. To close without applying,
	 * 'escape-escape' or 'backslash' can be used.
	 */
	void init(); //< Should be used if FilterForm is desired
	void freem(); //< Should be used when FilterForm is no longer used.
	void reset(); //< Reset form fields to default values
	void hide(); //< Hides FilterForm from view
	void show(const unsigned Y,const unsigned X); //< Displays Filterform
	int edit(const unsigned Y,const unsigned X,SQLi&); //< Displays FilterForm and begins user interaction.
}
static void database_mnip(const unsigned Y,const unsigned X,const size_t l,SQLi&); //< Displays database and allows modification with interface
int init_viewr(); //< Sets and runs inital functions. E.g. FilterForm::init
void nloop(); //< The 'main' portion of ui. Use after init_viewr
void cleanup(); //< The 'ending' portion of ui. Use to cleanup after init_viewr
#endif
