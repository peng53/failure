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

dq['qdate'] = '%s WHERE start_time BETWEEN ? AND ? OR end_time BETWEEN ? AND ?' %dq['']
dq['qbefore'] = '%s WHERE start_time<=? OR end_time <=?' %dq['']
dq['qafter'] = '%s WHERE start_time>=? OR end_time >=?' %dq['']

def intStrf(i):
	return time.strftime('%m/%d/%Y %H:%M',time.localtime(i))

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
		m_rs.add_command(label="Add to Notes")
		m_rs.add_command(label="Sort by..")
		m_rs.add_command(label="Clear Results",command=self.clear_results)
		menubar.add_cascade(label="Results",menu=m_rs)
		m_nt = Menu(menubar)
		m_nt.add_command(label="Sort by..")
		m_nt.add_command(label="Sum Time")
		m_nt.add_command(label="Clear Notes",command=self.clear_notes)
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
		self.res = Treeview(self.res_L,columns=('s','e','c','d','si','ei'),displaycolumns=('s','e','c','d'),yscrollcommand=self.res_sb.set)
		self.res.grid()
		self.res_sb.config(command=self.res.yview)
		self.res_sb.grid(row=0,column=1,sticky='ns')

		Button(self.res_R,text="Clear",command=self.clear_results).grid(sticky='ew')
		Button(self.res_R,text="Sort").grid(sticky='ew')

		Label(root,text="Notes").grid(sticky='w')
		self.notes_LR = Frame(root)
		self.notes_LR.grid()
		self.notes_L = Frame(self.notes_LR,bg='blue')
		self.notes_L.grid(row=0,column=0,sticky='nesw')
		self.notes_R = Frame(self.notes_LR,bg='green')
		self.notes_R.grid(row=0,column=1,sticky='n')

		self.notes_sb = Scrollbar(self.notes_L)
		self.notes = Treeview(self.notes_L,columns=('s','e','c','d','si','ei'),displaycolumns=('s','e','c','d'),yscrollcommand=self.notes_sb.set)
		self.notes.grid()
		self.notes_sb.config(command=self.notes.yview)
		self.notes_sb.grid(row=0,column=1,sticky='ns')

		for x,t,w in [('#0','UID',76),('s','Start Time',120),('e','End Time',120),('c','Code',40),('d','Desc',300)]:
			self.res.heading(x,text=t)
			self.res.column(x,width=w)
			self.notes.heading(x,text=t)
			self.notes.column(x,width=w)

		Button(self.notes_R,text="Clear",command=self.clear_notes).grid(sticky='ew')
		Button(self.notes_R,text="Sort").grid(sticky='ew')
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
				if c_cols==[('uid','text'),('start_time','integer'),('end_time','integer'),('code','text'),('desc','text')]:
					self.conn = conn
					print self.conn
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
			d = map(int,s.split('/',2))
			if len(d)==3:return time.mktime(time.struct_time([d[2],d[0],d[1],0,0,0,0,0,-1]))
			showerror(title="Invalid date",message="Date missing month, day, or year.")
		except Exception as e:
			showerror(title="Invalid date",message="Please enter a valid date in M/D/Y format.")
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
					args = (s)
				else:
					d = self.nice_date(s)
					print d
					if not d: return
					args = (d,d+86400,d,d+86400) if f=='date' else (d,d)
				q = c.execute(dq['q'+f],args)
			if q:
				for row in q:
					r = [row[0],intStrf(row[1]),intStrf(row[2]),row[3],row[4],row[1],row[2]]
					self.add_tv(r,pos='end',res=True)
		else:
			showerror(title="No file loaded",message="Please load a file for lookup.")
	def add_tv(self,vs,pos='end',res=True):
		tv = self.res if res else self.notes
		tv.insert(parent='',index=pos,text=vs[0], values=vs[1:])
	def rem_tv(self,pos,res=True):
		# Pos is natural number
		tv = self.res if res else self.notes
		iid = tv.identify_row(pos)
		#print iid
		tv.delete(iid)

root = Tk()
app = App(root)
root.mainloop()
