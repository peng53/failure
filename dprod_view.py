#!/usr/bin/env python2
# dprod_view.py
from Tkinter import *
from ttk import Treeview
import sqlite3

class App:
	def __init__(self,root):

		self.root = root

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

		self.res = Treeview(self.res_L,columns=('s','e','c','d'))
		self.res.grid()
		self.res_sb = Scrollbar(self.res_L)
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

		self.notes = Treeview(self.notes_L,columns=('s','e','c','d'))
		self.notes.grid()
		self.notes_sb = Scrollbar(self.notes_L)
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
		self.sum_t = Text(self.fsum)
		self.sum_t.grid(row=0,column=0,sticky='ew')
		self.sum_sb = Scrollbar(self.fsum)
		self.sum_sb.grid(row=0,column=1,sticky='ns')
		self.add_tv(['z2484','05/27/2017 12:55','05/27/2017 14:24','342','Desc here'],res=False)
		self.add_tv(['z2484','05/27/2017 14:24','05/27/2017 17:55','521','Desc here'],res=False)
		self.add_tv(['z2484','05/27/2017 12:52','05/27/2017 12:55','103','Desc here'],pos=0,res=False)

		self.add_tv(['x1454','05/27/2017 12:52','05/27/2017 12:55','103','Desc here'])

	def add_tv(self,vs,pos='end',res=True):
		tv = self.res if res else self.notes
		tv.insert(parent='',index=pos,text=vs[0], values=vs[1:])



root = Tk()
app = App(root)
root.mainloop()
