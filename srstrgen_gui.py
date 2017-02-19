from Tkinter import *
from srstrgen import *

class SrApp:
	def __init__(self,MAS):
		self.M = MAS
		self.fINCS = Frame(MAS)
		self.fChks = Frame(self.fINCS)
		self.cks = [IntVar() for _ in xrange(6)]
		self.c = [Checkbutton(self.fINCS,text=k.upper(),variable=self.cks[i]) for i,k in enumerate(['lower','upper','digits','symbols1','symbols2','spaces'])]
		self.fINCS.grid()
		self.fChks.grid()
		Label(self.fINCS,text="Random Parts",font='bold').grid(sticky='w')
		Label(self.fINCS,text="Builtins").grid()
		for e in self.c:
			e.grid(sticky='w')#column=i)
		self.EE = {}
		for k,e in [('Excludes/Custom/Constant','ec'),('Length*Words','lw'),('Delimiter','dm')]:
			Label(self.fINCS,text=k).grid()
			self.EE[e]=Entry(self.fINCS)
			self.EE[e].grid()
		Button(self.fINCS,text="Add as random",command=self.get_c).grid()
		Button(self.fINCS,text="Add as constant",command=self.get_c2).grid()
		Label(self.fINCS,text="Count").grid()
		self.EE["ct"]=Entry(self.fINCS)
		self.EE["ct"].grid()
		Label(self.fINCS,text="Parts",font='bold').grid(sticky='w')
		self.fParts = Frame(MAS,bg='red')
		self.fParts.grid()
		self.sbParts = Scrollbar(self.fParts)
		self.lbParts = Listbox(self.fParts)
		self.lbParts.grid(sticky='nesw',column=0,row=1)
		self.sbParts.grid(column=1,row=1,sticky='nesw')
		Button(self.fParts,text="Delete Part",command=self.part_pop).grid(columnspan=2)
		Button(self.fParts,text="Commit",command=self.commit_parts).grid(columnspan=2)
		Button(self.fParts,text="Quit",command=quit).grid(columnspan=2)
		self.fOut = Frame(MAS,bg='blue')
		self.fOut.grid(row=0,column=1,sticky='nesw',rowspan=2)
		Label(self.fOut,text="Output",font='bold').grid(sticky='w')
		self.tOut = Text(self.fOut)
		self.tOut.grid(sticky='nesw')

		self.S = []

	def get_c(self):
		N = []
		I = b_inc(*[self.cks[i].get() for i in xrange(6)])
		t = str(self.EE['ec'].get())
		if any(I):
			N.append(b_inc_repr(I))
			E = t
			if len(E)!=0: N.append('not %s' %E)
		else:
			I,E = t[-2:].split(t[-1]) if len(t)>2 and t[-1]==t[-2] else t, ''
			N.append(I)
		lw = self.EE['lw'].get()
		if len(lw)==0: raise ValueError
		N.append(lw)
		if '*' in lw:
			try: L,W = map(int,lw.split('*'))
			except Exception as e: print e
			DM = self.EE['dm'].get()
			self.S.append((I,E,L,W,DM))
			if len(DM)!=0: N.append("by %s" %DM)
		else:
			try: self.S.append((I,E,int(lw)))
			except: return
		self.lbParts.insert('end',' '.join(N))
		print self.S[-1]
	def get_c2(self):
		w = 1
		t = self.EE['lw'].get()
		if len(t)!=0:
			try: t = int(t)
			except Exception as e: return
			if t>1: w = t
		t = self.EE['ec'].get()
		if len(t)==0: return
		t=self.EE['dm'].get().join([t]*w)
		self.S.append(t)
		self.lbParts.insert('end',t)
		print t

	def part_pop(self):
		if len(self.S)!=0:
			i = self.lbParts.index('active')
			self.S.pop(i)
			self.lbParts.delete(i)

	def commit_parts(self):
		out = moonPhase(self.S)
		self.tOut.insert('end','\n'.join(out))
		self.tOut.insert('end','\n')

def main():
	root = Tk()
	app = SrApp(root)
	root.mainloop()

if __name__=="__main__":
	main()

