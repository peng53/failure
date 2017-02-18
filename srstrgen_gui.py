from Tkinter import *
from srstrgen import *

class SrApp:
	def __init__(self,MAS):
		self.M = MAS
		self.fINCS = Frame(MAS)
		self.fChks = Frame(self.fINCS)
		self.cks = [BooleanVar() for _ in xrange(6)]
		self.c = [Checkbutton(self.fINCS,text=k.upper(),variable=self.cks[i]) for i,k in enumerate(['lower','upper','digits','symbols1','symbols2','spaces'])]
		self.fINCS.grid()
		self.fChks.grid()
		Label(self.fINCS,text="Random Parts",font='bold').grid(sticky='w')
		Label(self.fINCS,text="Builtins").grid()
		for i,e in enumerate(self.c):
			e.grid(sticky='w')#column=i)
		Label(self.fINCS,text="Custom").grid()
		self.eCus = Entry(self.fINCS)
		self.eCus.grid()
		Label(self.fINCS,text="Excludes").grid()
		self.eExc = Entry(self.fINCS)
		self.eExc.grid()
		Label(self.fINCS,text="Length*Words").grid()
		self.eLW = Entry(self.fINCS)
		self.eLW.grid()
		Label(self.fINCS,text="Delimiter").grid()
		self.eDlm = Entry(self.fINCS)
		self.eDlm.grid()
		Button(self.fINCS,text="Add RP").grid()
		Label(self.fINCS,text="Constant String",font='bold').grid(sticky='w')
		self.eBs = Entry(self.fINCS)
		self.eBs.grid()
		Button(self.fINCS,text="Add CS").grid()
		Label(self.fINCS,text="Parts",font='bold').grid(sticky='w')
		self.fParts = Frame(MAS)
		self.fParts.grid()
		self.sbParts = Scrollbar(self.fParts)
		self.lbParts = Listbox(self.fParts)
		self.lbParts.grid(sticky='nesw',column=0,row=1)
		self.sbParts.grid(column=1,row=1,sticky='nesw')
		Button(self.fParts,text="Delete Part").grid(columnspan=2)
		Button(self.fParts,text="Commit").grid(columnspan=2)
		Button(self.fParts,text="Quit",command=quit).grid(columnspan=2)

def main():
	root = Tk()
	app = SrApp(root)
	root.mainloop()

if __name__=="__main__":
	main()

