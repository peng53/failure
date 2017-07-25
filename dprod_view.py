#!/usr/bin/env python2
# dprod_view.py
from Tkinter import *
from tkFileDialog import askopenfilename, asksaveasfilename
from tkMessageBox import showerror, showwarning
from ttk import Treeview
import sqlite3
import time

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

class RecordView(Treeview):
	def bind_col(self):
		self.bind("<Double-1>", self.b_dbck_col)
		self.bind("<End>", self.b_end)
		self.bind("<Home>", self.b_home)
	def clear(self):
		self.delete(*self.get_children())
	def append(self,vs):
		# Add item vs to end
		self.insert(parent='',index='end',text=vs[0],values=vs[1:])
	def remove(self):
		for I in self.selection():
			self.delete(I)
	def moveUp(self):
		for I in self.selection():
			if self.index(I)==0: return
			self.move(I,'',self.index(I)-1)
	def moveDn(self):
		for I in self.selection():
			self.move(I,'',self.index(I)+1)
	def sort(self,col,rev):
		L = sorted(([self.item(I)['text']]+self.item(I)['values'] for I in self.get_children()), key=lambda v:v[col], reverse=rev)
		self.clear()
		for r in L:
			self.append(r)
	def sort_by(self):
		t = SortByAttr(self.master)
		o = t.show_dialog()
		if o:
			self.sort(o[0]-1,o[1])
	def b_dbck_col(self,event):
		if self.identify('region',event.x,event.y)=='heading':
			self.sort(int(self.identify_column(event.x)[1:]),False)
	def b_end(self,event):
		self.yview_moveto(1)
	def b_home(self,event):
		self.yview_moveto(0)

class App:
	def __init__(self,root):
		self.root = root
		self.conn = None
		self.lastq = (None,None)
		self.fque,self.res_LR,self.notes_LR,self.fsum = Frame(root),Frame(root),Frame(root),Frame(root)

		Label(root,text="Production Record Viewer").grid()
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
		self.res_LR.grid()
		self.res_L = Frame(self.res_LR)
		self.res_L.grid(row=0,column=0)
		self.res_R = Frame(self.res_LR)
		self.res_R.grid(row=0,column=1,sticky='n')
		self.res_sb = Scrollbar(self.res_L)
		self.res = RecordView(self.res_L,columns=('s','e','c','d'),yscrollcommand=self.res_sb.set,selectmode='extended')
		self.res.bind_col()
		self.res.grid()
		self.res_sb.config(command=self.res.yview)
		self.res_sb.grid(row=0,column=1,sticky='ns')
		Button(self.res_R,text="Clear",command=self.res.clear).grid(sticky='ew')
		Button(self.res_R,text="Note",command=self.cmd_addtonotes).grid(sticky='ew')
		Button(self.res_R,text="Sort",command=self.res.sort_by).grid(sticky='ew')
		Button(self.res_R,text="Sort?",command=self.sort_last_q).grid(sticky='ew')

		Label(root,text="Notes").grid(sticky='w')
		self.notes_LR.grid()
		self.notes_L = Frame(self.notes_LR)
		self.notes_L.grid(row=0,column=0,sticky='nesw')
		self.notes_R = Frame(self.notes_LR)
		self.notes_R.grid(row=0,column=1,sticky='n')
		self.notes_sb = Scrollbar(self.notes_L)
		self.notes = RecordView(self.notes_L,columns=('s','e','c','d'),yscrollcommand=self.notes_sb.set,selectmode='extended')
		self.notes.bind_col()
		self.notes.grid()
		self.notes_sb.config(command=self.notes.yview)
		self.notes_sb.grid(row=0,column=1,sticky='ns')
		Button(self.notes_R,text="Clear",command=self.notes.clear).grid(sticky='ew')
		Button(self.notes_R,text="Delete",command=self.notes.remove).grid(sticky='ew')
		Button(self.notes_R,text="Up",command=self.notes.moveUp).grid(sticky='ew')
		Button(self.notes_R,text="Down",command=self.notes.moveDn).grid(sticky='ew')
		Button(self.notes_R,text="Sort",command=self.notes.sort_by).grid(sticky='ew')
		Button(self.notes_R,text="Total",command=self.totalt).grid(sticky='ew')
		Button(self.notes_R,text='Save',command=self.res.clear).grid(sticky='ew')

		for x,t,w in [('#0','UID',76),('s','Start Time',120),('e','End Time',120),('c','Code',40),('d','Desc',300)]:
			self.res.heading(x,text=t)
			self.res.column(x,width=w)
			self.notes.heading(x,text=t)
			self.notes.column(x,width=w)

		Label(root,text="Summary").grid(sticky='w')
		self.fsum.grid(sticky='ew')
		self.fsum.grid_columnconfigure(0,weight=1)
		self.sum_sb = Scrollbar(self.fsum)
		self.sum_t = Text(self.fsum,height=10,yscrollcommand=self.sum_sb.set)
		self.sum_t.grid(row=0,column=0,sticky='ew')
		self.sum_sb.grid(row=0,column=1,sticky='ns')
		self.sum_sb.config(command=self.sum_t.yview)

		menubar = Menu(root)
		root.config(menu=menubar)
		m_db,m_lu,m_rs,m_nt,m_sy = Menu(menubar),Menu(menubar),Menu(menubar),Menu(menubar),Menu(menubar)
		lemenu = {
			m_db: [('Open Database',self.open_db),('Close Database',self.close_db)],
			m_lu: [('Lookup Query',self.cmd_lookup),('Clear Query',self.clear_query)],
			m_rs: [("Add to Notes",self.cmd_addtonotes),("Sort by..",self.res.sort_by),("Clear Results",self.res.clear)],
			m_nt: [("Sort by..",self.notes.sort_by),("Sum Time",self.totalt),("Clear Notes",self.notes.clear),
				("Delete Note(s)",self.notes.remove),("Move Up",self.notes.moveUp),("Move Down",self.notes.moveDn)],
			m_sy: [("Save",None),("Print",None),("Clear",self.clear_sum_t)]
		}
		for k in lemenu:
			for l,c in lemenu[k]:
				k.add_command(label=l,command=c)
		menubar.add_cascade(label="Database",menu=m_db)
		menubar.add_cascade(label="Lookup",menu=m_lu)
		menubar.add_cascade(label="Results",menu=m_rs)
		menubar.add_cascade(label="Notes",menu=m_nt)
		menubar.add_cascade(label="Summary",menu=m_sy)
		menubar.add_command(label="Quit",command=root.quit)

	def open_db(self):
		s = askopenfilename(filetypes=[("Database file","*.db")])
		if len(s)>0:
			try:
				conn = sqlite3.connect(s)
				c = conn.cursor()
				c_cols = [col[1:3] for col in c.execute('pragma table_info(prod_records)')]
				if c_cols==[('uid','text'),('start_time','datetime'),('end_time','datetime'),('code','text'),('desc','text')]:
					self.conn = conn
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
		self.res.clear()
		if self.conn:
			q = None
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
					self.res.append(row)
				self.lastq = (s, self.v.get().lower())
		else:
			showerror(title="No file loaded",message="Please load a file for lookup.")
	def cmd_addtonotes(self):
		for I in self.res.selection():
			self.notes.append([self.res.item(I)['text']]+self.res.item(I)['values'])
	def sort_last_q(self):
		if self.lastq == (None, None):
			return
		t = SortByAttr(self.root)
		o = t.show_dialog()
		if o:
			c = self.conn.cursor()
			if self.lastq[0]=='*':
				self.lastq = ('*',[None,"uid","start_time","end_time","code","desc"][o[0]])
				args = self.lastq
				l = 'SELECT * from prod_records ORDER BY '+self.lastq[1]
				if o[1]:
					l += ' DESC'
				q = c.execute(l)
			else:
				pass

			self.res.clear()
			for row in q:
				self.res.append(row)
	def clear_sum_t(self):
		self.sum_t.delete('0.0','end')
	def totalt(self):
		# by uid & total in notes
		t = {}
		for row in self.notes.get_children():
			u = self.notes.item(row)['text']
			if u not in t:
				t[u] = {}
			vs = self.notes.item(row)['values']
			if vs[2] not in t[u]:
				t[u][vs[2]] = 0
			t0 = time.mktime(time.strptime(vs[0], '%Y-%m-%d %H:%M'))
			t1 = time.mktime(time.strptime(vs[1], '%Y-%m-%d %H:%M'))
			t[u][vs[2]] += t1-t0
		self.sum_t.delete('0.0','end')
		self.sum_t.insert('0.0','Total time of notes by UID and Code\n')
		for u in t:
			self.sum_t.insert('end',u)
			self.sum_t.insert('end','\n')
			s = 0
			for c in t[u]:
				self.sum_t.insert('end','%10s : %4d mins\n' %(c,t[u][c]//60))
				s += t[u][c]
			self.sum_t.insert('end','Total %5d mins\n' %(s//60))

root = Tk()
root.title("Production Record Viewer")
app = App(root)
root.mainloop()
