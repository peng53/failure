#!/usr/bin/env python2
# dprod_view.py
from Tkinter import *
from ttk import Treeview
import sqlite3

class App:
	def __init__(self,root):

		self.root = root
		self.fque = Frame(root)
		self.v = StringVar()
		self.tk = {'eque':Entry(self.fque,width=10),
			'keys':OptionMenu(self.fque,self.v,"Hi","B;i"),
			'res':Treeview(root,columns=(0,1),displaycolumns=(0,1))}
		self.tk['eque'].grid(row=0,column=0)
		self.tk['keys'].grid(row=0,column=1)
		Button(self.fque,text="Lookup").grid(row=0,column=2)
		Label(root,text="Production Record Viewer").grid()
		self.fque.grid()
		Label(root,text="Results").grid()
		self.tk['res'].grid()
		Label(root,text="Notes").grid()
		Label(root,text="Summary").grid()
		self.tk['res'].insert(parent="",index=0,text="TOUSE",values=("1st","2nd"))


		#self.master = master
		#self.query_box = Tk.Frame(master)
		#self.query_box.grid(sticky='ew')
		#Tk.Label(self.query_box,text="Production Records Viewer").grid(row=0,columnspan=3)
		#self.query_box_entry = Tk.Entry(self.query_box,width=86)
		#self.query_box_entry.grid(row=1,column=0,sticky='NESW')
		#self.v = Tk.StringVar(master)
		#self.query_box_opmenu = Tk.OptionMenu(self.query_box,self.v,"UID","Code")
		#self.query_box_opmenu.grid(row=1,column=1)
		#self.query_box_but = Tk.Button(self.query_box,text="Look-up")
		#self.query_box_but.grid(row=1,column=2)

		#self.result_box = Tk.Frame(master)
		#self.result_box_but_box = Tk.Frame(self.result_box)
		#self.result_box_but_sel = Tk.Button(self.result_box_but_box,text="Select")
		#self.result_box_but_cls = Tk.Button(self.result_box_but_box,text="Clear")
		#self.result_box_lb = Tk.Listbox(self.result_box,width=100)
		#self.result_box.grid(sticky='nesw')
		#Tk.Label(self.result_box,text="Search Results",width=100).grid(row=0,columnspan=2,column=0)
		#self.result_box_lb.grid(row=1,column=0,sticky='ew')

		#self.result_box_but_box.grid(row=1,column=1)
		#self.result_box_but_sel.grid(row=0,column=0)
		#self.result_box_but_cls.grid(row=1,column=0)

		#self.notes = Tk.Frame(master)
		#self.notes.grid()
		#self.notes_but = Tk.Frame(self.notes)
		#self.notes_but.grid(row=1,column=1)
		#Tk.Label(self.notes,text="Selected").grid(row=0,columnspan=2)
		#self.notes_lb = Tk.Listbox(self.notes,width=100)
		#self.notes_lb.grid(row=1,column=0,sticky='NESW')
		#self.notes_but_sort = Tk.Button(self.notes_but,text="Sort")
		#self.notes_but_cls = Tk.Button(self.notes_but,text="Clear")
		#self.notes_but_sum = Tk.Button(self.notes_but,text="Sum")
		#self.notes_but_del = Tk.Button(self.notes_but,text="Delete")
		#self.notes_but_sort.grid(row=0,column=0)
		#self.notes_but_cls.grid(row=1,column=0)
		#self.notes_but_sum.grid(row=2,column=0)
		#self.notes_but_del.grid(row=3,column=0)



root = Tk()
app = App(root)
root.mainloop()
