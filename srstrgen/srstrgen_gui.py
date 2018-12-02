from Tkinter import *
from srstrgen import *

class SrApp:
	def __init__(self,MAS):
		self.M = MAS
		self.fChks, self.fINCS, self.fButs, self.fParts =\
			Frame(MAS), Frame(MAS), Frame(MAS), Frame(MAS)
		self.sbOut = Scrollbar(MAS)
		self.tOut = Text(MAS,height=34,yscrollcommand=self.sbOut.set)
		self.sbOut.config(command=self.tOut.yview)
		self.fChks.grid()
		self.fINCS.grid()
		self.fButs.grid()
		self.fParts.grid()
		self.tOut.grid(column=1,row=0,rowspan=4,sticky='n')
		self.sbOut.grid(column=2,row=0,rowspan=4,sticky='ns')
		self.cks = [IntVar() for _ in xrange(6)]
		self.c = [Checkbutton(self.fChks,text=k.upper(),variable=self.cks[i]) for i,k in\
			enumerate(['lower','upper','digits','symbols1','symbols2','spaces'])]
		for i in xrange(3):
			self.c[i].grid(row=i,column=0,sticky='w')
			self.c[i+3].grid(row=i,column=1,sticky='w')
		self.EE = {}
		for k,e in [('Excludes/Custom/Constant','ec'),('Length*Words','lw'),('Delimiter','dm'),('Count','ct')]:
			Label(self.fINCS,text=k).grid()
			self.EE[e]=Entry(self.fINCS)
			self.EE[e].grid()

		Button(self.fButs,text="Random",command=self.get_c).grid(row=0,column=0,sticky='ew')
		Button(self.fButs,text="Constant",command=self.get_c2).grid(row=0,column=1,sticky='ew')
		Button(self.fButs,text="Pop",command=self.part_pop).grid(row=1,column=0,sticky='ew')
		Button(self.fButs,text="Commit",command=self.commit_parts).grid(row=1,column=1,sticky='ew')
		Button(self.fButs,text="Clear",command=self.clear_out).grid(row=2,column=0,sticky='ew')
		Button(self.fButs,text="New",command=self.new_part).grid(row=2,column=1,sticky='ew')
		Button(self.fButs,text="Save",command=quit).grid(row=3,column=0,sticky='ew')
		Button(self.fButs,text="Quit",command=quit).grid(row=3,column=1,sticky='ew')

		self.sbParts = Scrollbar(self.fParts)
		self.lbParts = Listbox(self.fParts,yscrollcommand=self.sbParts.set)
		self.lbParts.grid(sticky='nesw',column=0,row=0)
		self.sbParts.grid(column=1,row=0,sticky='nesw')
		self.sbParts.config(command=self.lbParts.yview)
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
			except Exception as e: raise ValueError
			DM = self.EE['dm'].get()
			self.S.append((I,E,L,W,DM))
			if len(DM)!=0: N.append("by %s" %DM)
		else:
			try: self.S.append((I,E,int(lw)))
			except: raise ValueError
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
		if len(self.S)==0: raise ValueError
		cnt = self.EE['ct'].get()
		try: cnt = int(cnt)
		except: cnt = 1
		out = moonPhase(self.S,cnt)
		self.tOut.insert('end','\n'.join(out))
		self.tOut.insert('end','\n')
	def clear_out(self):
		self.tOut.delete(1.0,'end')
	def new_part(self):
		for e in self.EE:
			self.EE[e].delete(0,'end')
		for c in self.c:
			c.deselect()
def main():
	root = Tk()
	root.wm_title("Specialized Random String Generator")
	app = SrApp(root)
	root.mainloop()

if __name__=="__main__":
	main()

