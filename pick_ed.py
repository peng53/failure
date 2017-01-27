import Tkinter as Tk

class PickEd:
	def __init__(self,master,V):
		self.v = V
		self.KV = not hasattr(V,'index')
		self.fr1 = Tk.Frame()
		SB = Tk.Scrollbar(master,command=self.yview, orient='vertical')
		self.lb1 = Tk.Listbox(self.fr1,height=25,width=40 if self.KV else 80, yscrollcommand=SB.set)
		for item in self.v:
			self.lb1.insert('end', item)

		self.EN1 = Tk.Entry()
		self.fr1.grid(column=0,row=0,columnspan=3 if self.KV else 6)

		if self.KV:
			self.fr2 = Tk.Frame()
			self.lb2 = Tk.Listbox(self.fr2,height=25,width=40, yscrollcommand=SB.set)
			for item in self.v:
				self.lb2.insert('end', str(self.v[item]))
			self.EN2 = Tk.Entry()

			self.fr2.grid(column=3,row=0,columnspan=3)
			Tk.Label(self.fr2,text="Values").grid()
			Tk.Label(self.fr1,text="Keys").grid()
			self.lb2.grid()
			self.EN2.grid(row=1,column=3,columnspan=3,sticky='nsew')

		else:
			Tk.Label(self.fr1,text="Values").grid()
		self.lb1.grid()
		SB.grid(row=0,rowspan=3,column=6,sticky='ns')

		self.EN1.grid(row=1,column=0,columnspan=3,sticky='nsew')


		Tk.Button(master,text="Get", command=self.getb).grid(row=2,column=0, sticky='ew')
		Tk.Button(master,text="Set", command=self.setb).grid(row=2,column=1, sticky='ew')
		Tk.Button(master,text="Add", command=self.getb).grid(row=2,column=2, sticky='ew')
		Tk.Button(master,text="Del", command=self.getb).grid(row=2,column=3, sticky='ew')
		Tk.Button(master,text="Clr", command=self.getb).grid(row=2,column=4, sticky='ew')
		Tk.Button(master,text="Exit", command=master.quit).grid(row=2,column=5, sticky='ew')

	def yview(self, *args):
		apply(self.lb1.yview, args)
		if self.KV:
			apply(self.lb2.yview, args)

	def getb(self):
		I = self.lb1.index('active')
		if self.KV:
			self.lb2.see(I)
			v,v2 = self.lb1.get(I), self.lb2.get(I)
			self.EN2.delete(0,'end')
			self.EN2.insert(0,v2)
		else:
			v = self.v[I]
		self.EN1.delete(0,'end')
		self.EN1.insert(0,v)

	def setb(self):
		I = self.lb1.index('active')
		if self.KV:
			pass

def run_pick_ed():
	root = Tk.Tk()
	v = {"cookie":"cake","banana":"bread"}
	for x in xrange(40):
		v[x+4] = x*30
	#v = range(10,50,2)
	app = PickEd(root,v)
	root.mainloop()

run_pick_ed()
