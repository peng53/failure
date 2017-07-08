#!/usr/bin/env python2
# dprod_view.py
import Tkinter as Tk
import sqlite3

class App:
	def __init__(self,master):
		self.master = master
		self.l = Tk.Label(master, text="test")
		self.l.pack()
		self.e = Tk.Entry(master)
		self.e.pack()
		self.attrib = []
		self.v = Tk.StringVar(master)
		self.sb = Tk.OptionMenu(master,self.v,"UID","Code")
		self.sb.pack()
root = Tk.Tk()
app = App(root)
root.mainloop()
