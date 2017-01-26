import Tkinter as Tk

class PickEd:
	def __init__(self,master):
		self.kframe = Tk.Frame(background="white")
		self.vframe = Tk.Frame(bg="grey")
		self.kframe.grid(column=0,row=0)
		self.vframe.grid(column=1,row=0)
		Tk.Label(self.kframe,text="Kframe").grid()
		Tk.Label(self.vframe,text="Vframe").grid()

		self.dd =  {}
		scrollbar = Tk.Scrollbar(master,command=self.yview, orient=Tk.VERTICAL)
		scrollbar.grid(column=3,row=0,sticky="NS")
		for e in xrange(50):
			self.dd[e] = e+2
		self.listbox = Tk.Listbox(self.kframe,height=25,width=40, yscrollcommand=scrollbar.set)
		self.listbox.grid()
		for item in self.dd:
			self.listbox.insert(Tk.END, item)
		self.listbox2 = Tk.Listbox(self.vframe,height=25,width=40, yscrollcommand=scrollbar.set)
		self.listbox2.grid()
		for item in self.dd:
			self.listbox2.insert(Tk.END, str(self.dd[item]))

		Tk.Button(master,text="Get", command=self.getb).grid()

		#self.menubar = Tk.Menu(master)
		#self.menubar.add_command(label="Quit", command=master.quit)
		#master.config(menu=self.menubar)
		#self.fname = Tk.Entry(master)
		#self.fname.pack()

	def yview(self, *args):
		apply(self.listbox.yview, args)
		apply(self.listbox2.yview, args)

	def getb(self):
		print self.listbox.index(Tk.ACTIVE)
		print self.dd[self.listbox.index(Tk.ACTIVE)]
def run_pick_ed():
	root = Tk.Tk()
	app = PickEd(root)
	root.mainloop()

run_pick_ed()
