#!/usr/bin/env python2
# dprod_view.py
from Tkinter import *
from ttk import Treeview
import sqlite3

class App:
	def __init__(self,root):
		self.root = root

		menubar = Menu(root)
		root.config(menu=menubar)
		m_db = Menu(menubar)
		m_db.add_command(label="Open Database")
		m_db.add_command(label="Close Database")
		menubar.add_cascade(label="Database",menu=m_db)
		m_lu = Menu(menubar)
		m_lu.add_command(label="Lookup Query")
		m_lu.add_command(label="Clear Query")
		menubar.add_cascade(label="Lookup",menu=m_lu)
		m_rs = Menu(menubar)
		m_rs.add_command(label="Add to Notes")
		m_rs.add_command(label="Sort by..")
		m_rs.add_command(label="Clear Results")
		menubar.add_cascade(label="Results",menu=m_rs)
		m_nt = Menu(menubar)
		m_nt.add_command(label="Sort by..")
		m_nt.add_command(label="Sum Time")
		menubar.add_cascade(label="Notes",menu=m_nt)
		m_sy = Menu(menubar)
		m_sy.add_command(label="Save")
		m_sy.add_command(label="Print")
		m_sy.add_command(label="Clear")
		menubar.add_cascade(label="Summary",menu=m_sy)
		menubar.add_command(label="Quit",command=root.quit)

		Label(root,text="Production Record Viewer").grid()
		self.fque = Frame(root)
		self.fque.grid(sticky='ew')
		self.fque.grid_columnconfigure(0,weight=1)
		self.v = StringVar(value="UID")
		self.eque = Entry(self.fque)
		self.eque.grid(row=0,column=0,sticky='ew')
		self.om = OptionMenu(self.fque,self.v,"UID","Code")
		self.om.grid(row=0,column=1)
		Button(self.fque,text="Lookup").grid(row=0,column=2)

		Label(root,text="Results").grid(sticky='w')
		self.res_LR = Frame(root)
		self.res_LR.grid()
		self.res_L = Frame(self.res_LR)
		self.res_L.grid(row=0,column=0)
		self.res_R = Frame(self.res_LR)
		self.res_R.grid(row=0,column=1,sticky='n')

		self.res_sb = Scrollbar(self.res_L)
		self.res = Treeview(self.res_L,columns=('s','e','c','d'),yscrollcommand=self.res_sb.set)
		self.res.grid()
		self.res_sb.config(command=self.res.yview)
		self.res_sb.grid(row=0,column=1,sticky='ns')

		Button(self.res_R,text="Clear").grid(sticky='ew')
		Button(self.res_R,text="Sort").grid(sticky='ew')

		Label(root,text="Notes").grid(sticky='w')
		self.notes_LR = Frame(root)
		self.notes_LR.grid()
		self.notes_L = Frame(self.notes_LR,bg='blue')
		self.notes_L.grid(row=0,column=0,sticky='nesw')
		self.notes_R = Frame(self.notes_LR,bg='green')
		self.notes_R.grid(row=0,column=1,sticky='n')

		self.notes_sb = Scrollbar(self.notes_L)
		self.notes = Treeview(self.notes_L,columns=('s','e','c','d'),yscrollcommand=self.notes_sb.set)
		self.notes.grid()
		self.notes_sb.config(command=self.notes.yview)
		self.notes_sb.grid(row=0,column=1,sticky='ns')

		for x,t,w in [('#0','UID',76),('s','Start Time',120),('e','End Time',120),('c','Code',40),('d','Desc',300)]:
			self.res.heading(x,text=t)
			self.res.column(x,width=w)
			self.notes.heading(x,text=t)
			self.notes.column(x,width=w)

		Button(self.notes_R,text="Clear").grid(sticky='ew')
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

		self.add_tv(['z2484','05/27/2017 12:55','05/27/2017 14:24','342','First item added, but 2nd'],res=False)
		self.add_tv(['z2484','05/27/2017 14:24','05/27/2017 17:55','521','Second item added, but 3rd'],res=False)
		self.add_tv(['z2484','05/27/2017 12:52','05/27/2017 12:55','103','Third item added, but 1st'],pos=0,res=False)
		self.add_tv(['x1454','05/27/2017 12:52','05/27/2017 12:55','103','Desc here'])
		for x in xrange(30):
			self.add_tv(['x1454','05/27/2017 12:52','05/27/2017 12:55','103','Desc here'],res=True)
		self.rem_tv(1,False)

	def add_tv(self,vs,pos='end',res=True):
		tv = self.res if res else self.notes
		print tv.insert(parent='',index=pos,text=vs[0], values=vs[1:])
	def rem_tv(self,pos,res=True):
		# Pos is natural number
		tv = self.res if res else self.notes
		iid = tv.identify_row(pos)
		print iid
		tv.delete(iid)



root = Tk()
app = App(root)
root.mainloop()
