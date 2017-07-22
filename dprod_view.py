#!/usr/bin/env python2
# dprod_view.py
from Tkinter import *
from tkFileDialog import askopenfilename, asksaveasfilename
from tkMessageBox import showerror, showwarning
from ttk import Treeview
import sqlite3

dq = {'':'SELECT * FROM prod_records'}
for k in ('uid','code'):
	dq['*'+k] = '%s ORDER BY %s' %(dq[''],k)
	dq['q'+k] = '%s WHERE %s=?' %(dq[''],k)
dq['*date'] = dq['*before'] = '%s ORDER BY start_time' %dq['']
dq['*after'] = '%s ORDER BY end_time' %dq['']

dq['qdate'] = '%s WHERE date(start_time)=? OR date(end_time)=?' %dq['']
dq['qbefore'] = '%s WHERE date(start_time)<? OR date(end_time)<?' %dq['']
dq['qafter'] = '%s WHERE date(start_time)>? OR date(end_time)>?' %dq['']

class SortByAttr:
	def __init__(self,root):
		self.sel = None
		self.root = root
		self.dialog = Toplevel(root,takefocus=True)
		self.dialog.title("Sort by..")
		self.dialog.grab_set()
		self.dialog.transient(root)
		self.dialog.grid()
		Label(self.dialog,text="Attribute to sort by").grid(row=0,columnspan=2)
		self.v=StringVar(value="UID")
		self.op = {"UID":1,"Start Time":2,"End Time":3,"Code":4,"Desc":5}
		self.om = OptionMenu(self.dialog,self.v,*self.op)
		self.om.grid(row=1,columnspan=2,sticky='ew')
		self.b=BooleanVar(value=False)
		Checkbutton(self.dialog,text='Reverse Order',variable=self.b).grid(row=2,columnspan=2)
		Button(self.dialog,text='Sort',command=self.choose).grid(row=3,column=0,sticky='ew')
		Button(self.dialog,text='Cancel',command=self.dialog.destroy).grid(row=3,column=1,sticky='ew')
	def choose(self):
		self.sel = (self.op[self.v.get()],self.b.get())
		self.dialog.destroy()
	def show_dialog(self):
		self.root.wait_window(self.dialog)
		return self.sel

class App:
	def __init__(self,root):

		self.root = root
		self.conn = None

		menubar = Menu(root)
		root.config(menu=menubar)
		m_db = Menu(menubar)

		m_db.add_command(label="Open Database",command=self.open_db)
		m_db.add_command(label="Close Database",command=self.close_db)
		menubar.add_cascade(label="Database",menu=m_db)
		m_lu = Menu(menubar)
		m_lu.add_command(label="Lookup Query",command=self.cmd_lookup)
		m_lu.add_command(label="Clear Query",command=self.clear_query)
		menubar.add_cascade(label="Lookup",menu=m_lu)
		m_rs = Menu(menubar)
		m_rs.add_command(label="Add to Notes",command=self.cmd_addtonotes)
		rs_sortby = lambda: self.tv_sortby(res=True)
		m_rs.add_command(label="Sort by..",command=rs_sortby)
		m_rs.add_command(label="Clear Results",command=self.clear_results)
		menubar.add_cascade(label="Results",menu=m_rs)
		m_nt = Menu(menubar)
		nt_sortby = lambda: self.tv_sortby(res=False)
		m_nt.add_command(label="Sort by..",command=nt_sortby)
		m_nt.add_command(label="Sum Time")
		m_nt.add_command(label="Clear Notes",command=self.clear_notes)
		m_nt.add_command(label="Delete Note(s)",command=self.del_noteS)
		m_nt.add_command(label="Move Up",command=self.note_up)
		m_nt.add_command(label="Move Down",command=self.note_dn)
		menubar.add_cascade(label="Notes",menu=m_nt)
		m_sy = Menu(menubar)
		m_sy.add_command(label="Save")
		m_sy.add_command(label="Print")
		m_sy.add_command(label="Clear",command=self.clear_notes)
		menubar.add_cascade(label="Summary",menu=m_sy)
		menubar.add_command(label="Quit",command=root.quit)

		Label(root,text="Production Record Viewer").grid()
		self.fque = Frame(root)
		self.fque.grid(sticky='ew')
		self.fque.grid_columnconfigure(0,weight=1)
		self.v = StringVar(value="UID")
		self.eque = Entry(self.fque)
		self.eque.grid(row=0,column=0,sticky='ew')
		l_enter_lookup = lambda x:self.cmd_lookup()
		self.eque.bind("<Return>",l_enter_lookup)
		self.eque.bind("<KP_Enter>",l_enter_lookup)
		self.om = OptionMenu(self.fque,self.v,"UID","Code","Date","Before","After")
		self.om.grid(row=0,column=1)
		Button(self.fque,text="Lookup",command=self.cmd_lookup).grid(row=0,column=2)

		Label(root,text="Results").grid(sticky='w')
		self.res_LR = Frame(root)
		self.res_LR.grid()
		self.res_L = Frame(self.res_LR)
		self.res_L.grid(row=0,column=0)
		self.res_R = Frame(self.res_LR)
		self.res_R.grid(row=0,column=1,sticky='n')

		self.res_sb = Scrollbar(self.res_L)
		self.res = Treeview(self.res_L,columns=('s','e','c','d'),yscrollcommand=self.res_sb.set,selectmode='extended')
		self.res.grid()
		self.res_sb.config(command=self.res.yview)
		self.res_sb.grid(row=0,column=1,sticky='ns')

		Button(self.res_R,text="Clear",command=self.clear_results).grid(sticky='ew')
		Button(self.res_R,text="Note",command=self.cmd_addtonotes).grid(sticky='ew')
		Button(self.res_R,text="Sort",command=rs_sortby).grid(sticky='ew')

		Label(root,text="Notes").grid(sticky='w')
		self.notes_LR = Frame(root)
		self.notes_LR.grid()
		self.notes_L = Frame(self.notes_LR,bg='blue')
		self.notes_L.grid(row=0,column=0,sticky='nesw')
		self.notes_R = Frame(self.notes_LR,bg='green')
		self.notes_R.grid(row=0,column=1,sticky='n')

		self.notes_sb = Scrollbar(self.notes_L)
		self.notes = Treeview(self.notes_L,columns=('s','e','c','d'),yscrollcommand=self.notes_sb.set,selectmode='extended')
		self.notes.bind("<Double-1>", self.on_double_click_notes)
		self.notes.grid()
		self.notes_sb.config(command=self.notes.yview)
		self.notes_sb.grid(row=0,column=1,sticky='ns')

		for x,t,w in [('#0','UID',76),('s','Start Time',120),('e','End Time',120),('c','Code',40),('d','Desc',300)]:
			self.res.heading(x,text=t)
			self.res.column(x,width=w)
			self.notes.heading(x,text=t)
			self.notes.column(x,width=w)

		Button(self.notes_R,text="Clear",command=self.clear_notes).grid(sticky='ew')
		Button(self.notes_R,text="Delete",command=self.del_noteS).grid(sticky='ew')
		Button(self.notes_R,text="Up",command=self.note_up).grid(sticky='ew')
		Button(self.notes_R,text="Down",command=self.note_dn).grid(sticky='ew')

		Button(self.notes_R,text="Sort",command=nt_sortby).grid(sticky='ew')
		Button(self.notes_R,text="Total").grid(sticky='ew')
		Button(self.notes_R,text='Save').grid(sticky='ew')

		Label(root,text="Summary").grid(sticky='w')
		self.fsum = Frame(root)
		self.fsum.grid(sticky='ew')
		self.fsum.grid_columnconfigure(0,weight=1)
		self.sum_sb = Scrollbar(self.fsum)
		self.sum_t = Text(self.fsum,height=10,yscrollcommand=self.sum_sb.set)
		self.sum_t.grid(row=0,column=0,sticky='ew')
		self.sum_sb.grid(row=0,column=1,sticky='ns')
		self.sum_sb.config(command=self.sum_t.yview)

	def open_db(self):
		s = askopenfilename(filetypes=[("Database file","*.db")])
		if len(s)>0:
			try:
				conn = sqlite3.connect(s)
				c = conn.cursor()
				c_cols = [col[1:3] for col in c.execute('pragma table_info(prod_records)')]
				if c_cols==[('uid','text'),('start_time','datetime'),('end_time','datetime'),('code','text'),('desc','text')]:
					self.conn = conn
					print self.conn
				else:
					showerror(title="Error",message="File selected does not match schema!")
			except:
				showerror(title="No file loaded",message="Please select valid database for reading.")
				self.conn = None
		else:
			showwarning(title="No file selected",message="Please select valid database for reading.")
	def close_db(self):
		if self.conn:
			self.conn.close()
		else:
			showwarning(title="No file open",message="No action taken.")
	def clear_query(self):
		self.eque.delete(0,'end')
	def clear_notes(self):
		self.notes.delete(*self.notes.get_children())
	def clear_results(self):
		self.res.delete(*self.res.get_children())
	def nice_date(self,s):
		try:
			if '/' in s:
				d = map(int,s.split('/',2))
				if len(d)==3:return "%04d-%02d-%02d" %(d[2],d[0],d[1])
				showerror(title="Invalid date",message="Date missing month, day, or year.")
			elif '-' in s:
				d = map(int,s.split('-',2))
				if len(d)==3:return "%04d-%02d-%02d" %(d[0],d[1],d[2])
				showerror(title="Invalid date",message="Date missing month, day, or year.")
			elif len(s)==8: return '-'.join((s[4:8],s[0:2],s[2:4]))
			else:
				showerror(title="Invalid date",message="Format un-recognized.")
		except Exception as e:
			showerror(title="Invalid date",message="Please enter a valid date in M/D/Y OR YYYY-MM-DD OR MMDDYYYY format.")
	def cmd_lookup(self):
		q = None
		self.clear_results()
		if self.conn:
			c = self.conn.cursor()
			s = self.eque.get()
			if len(s)==0: q = c.execute(dq[''])
			elif s=='*': q = c.execute(dq['*'+self.v.get().lower()])
			else:
				f = self.v.get().lower()
				if f in ('uid','code'):
					args = (s,)
				else:
					d = self.nice_date(s)
					if not d: return
					args = (d,d)
				q = c.execute(dq['q'+f],args)
			if q:
				for row in q:
					self.add_tv(row,pos='end',res=True)
		else:
			showerror(title="No file loaded",message="Please load a file for lookup.")
	def cmd_addtonotes(self):
		for I in self.res.selection():
			II = self.res.item(I)
			self.notes.insert(parent='',index='end',text=II['text'], values=II['values'])
	def del_noteS(self):
		for I in self.notes.selection():
			self.notes.delete(I)
	def note_up(self):
		for I in self.notes.selection():
			if self.notes.index(I)==0:
				return
			self.notes.move(I,'',self.notes.index(I)-1)
	def note_dn(self):
		for I in self.notes.selection():
			self.notes.move(I,'',self.notes.index(I)+1)
	def tv_sortby(self,res=True):
		t = SortByAttr(self.root)
		o = t.show_dialog()
		if not o: return
		print o
		tv = self.res if res else self.notes
		L = self.sorted_recs(tv,o[0]-1,o[1])
		#L = sorted(([tv.item(I)['text']]+tv.item(I)['values'] for I in tv.get_children()),key=lambda x:x[o[0]-1], reverse=o[1])
		if res:	self.clear_results()
		else: self.clear_notes()
		for e in L:
			self.add_tv(e,res=res)
	def sorted_recs(self,tv,col,rev):
		return sorted(([tv.item(I)['text']]+tv.item(I)['values'] for I in tv.get_children()),key=lambda x:x[col], reverse=rev)

	def add_tv(self,vs,pos='end',res=True):
		tv = self.res if res else self.notes
		tv.insert(parent='',index=pos,text=vs[0], values=vs[1:])
	def rem_tv(self,pos,res=True):
		# Pos is natural number
		tv = self.res if res else self.notes
		iid = tv.identify_row(pos)
		#print iid
		tv.delete(iid)
	def on_double_click_notes(self, event):
		region = self.notes.identify("region", event.x, event.y)
		if region=='heading':
			o = int(self.notes.identify_column(event.x)[1:])
			L = self.sorted_recs(self.notes,o,False)
			self.clear_notes()
			for e in L:
				self.notes.insert(parent='',index='end',text=e[0],values=e[1:])

root = Tk()
root.title("Production Record Viewer")
app = App(root)
root.mainloop()
