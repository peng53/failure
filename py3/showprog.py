import tkinter as tk
from tkinter import ttk
from tkinter import filedialog
import sqlite3
import os

def getShowsFromSqlite(filename):
	con = sqlite3.connect(filename)
	cur = con.cursor()
	shows = []
	colsToGet = ['title','last_watched']
	for row in cur.execute(f'select {",".join(colsToGet)} from shows'):
		shows.append({colsToGet[i]:row[i] for i in range(len(colsToGet))})
	con.close()
	return shows

def testGenNewSqlite(filename):
	if os.path.exists(filename):
		return
	con = sqlite3.connect(filename)
	cur = con.cursor()
	cur.execute("create table shows (title text, last_watched text)")
	cur.execute("insert into shows (title,last_watched) values ('Bleak','e10')")
	cur.execute("insert into shows (title,last_watched) values ('Bland','e23')")
	con.commit()
	con.close()

def loadExistingFile():
	filename = filedialog.askopenfilename()
	f = getShowsFromSqlite(filename)
	# open file as sqlite db
	# verify contents..
	# return connection
	return f

def newDatabaseFile():
	filename = filedialog.asksaveasfilename()
	print(filename)
	testGenNewSqlite(filename)

class Application(tk.Frame):
	def __init__(self, master=None):
		super().__init__(master)
		self.master = master
		self.master.grid()
		self.menubar = tk.Menu(master)
		self.master.config(menu=self.menubar)

		dumb = lambda: 1
		filemenu = tk.Menu(master)

		self.showlist = ttk.Treeview(master, columns=('#1','#2'), show='headings')
		self.showlist.heading('#1',text='Title')
		self.showlist.heading('#2',text='Last Watched')
		self.showlist.grid(row=0, column=0, sticky='nsew')
		
		self.showlistScroll = ttk.Scrollbar(master, orient=tk.VERTICAL, command=self.showlist.yview)
		self.showlist.configure(yscroll=self.showlistScroll.set)
		self.showlistScroll.grid(row=0, column=1, sticky='ns')

		self.showlist.insert(parent='', index='end', values=('1','2'))

		def loadNIns():
			filename = filedialog.askopenfilename()
			shows = getShowsFromSqlite(filename)
			for show in shows:
				self.showlist.insert(parent='', index='end', values=(show['title'],show['last_watched']))
	
		filemenu.add_command(label='New', command=newDatabaseFile)
		filemenu.add_command(label='Open', command=loadNIns)
		filemenu.add_command(label='Close', command=dumb)
		filemenu.add_separator()
		filemenu.add_command(label='Exit', command=root.destroy)
		self.menubar.add_cascade(label='File',menu=filemenu)

		trackF = lambda: self.tracknew()
		editmenu = tk.Menu(master)
		editmenu.add_command(label='Track New', command=trackF)
		self.menubar.add_cascade(label='Edit',menu=editmenu)
		

	def tracknew(self):
		print('tracknew')

root = tk.Tk()
root.columnconfigure(0, weight=1)
root.rowconfigure(0, weight=1)
app = Application(master=root)
root.mainloop()