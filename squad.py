#
# Draw a graph using Tkinter

import Tkinter as Tk
import math
import itertools
import graph

class SimpleDraw:
	def __init__(self,master,):
		self.D = 800
		self.pointSize = 10
		self.R = self.D//2
		self.w = Tk.Canvas(master, width=self.D+20, height=self.D+20,bg = "white")
		self.w.grid(row=0,columnspan=6)
		self.tE = Tk.Entry(master)
		self.tE.grid(row=1,column=0)
		Tk.Button(master, text="Random/Biclique", command=self.random).grid(row=1,column=1)
		Tk.Button(master, text="Bipartite", command=self.random_bipartite).grid(row=1,column=2)
		Tk.Button(master, text="Complete", command=self.komplete).grid(row=1,column=3)
		Tk.Button(master, text="RM Edge", command=self.rmedge).grid(row=1,column=4)
		Tk.Button(master, text="Close", command=quit).grid(row=1,column=5)
		self.G,self.XY,self.E = None,None,None

	def draw(self,sG):
		self.w.delete(Tk.ALL)
		self.w.create_line(1,0,1,self.D+20)
		self.w.create_line(self.D+20,0,self.D+20,self.D+20)
		self.G = sG
		self.A = 0
		self.dA = 180
		self.i = 1
		self.XY = []
		for v in xrange(len(self.G.adjm)):
			x = self.R * math.cos(self.A)+self.R+5
			y = self.R * math.sin(self.A)+self.R+5
			print x,y
			self.XY.append((x,y))
			self.A += self.dA
			if self.i==360//180:
				self.dA = self.dA//2
				self.A = self.dA//2
			self.i += 1

		self.E = {}
		for v,u in itertools.combinations(xrange(len(self.G.adjm)),2):
			vu,uv = self.G.has_edge(v,u), self.G.has_edge(u,v)
			if not vu and not uv: continue
			x1,y1 = self.XY[v]
			x2,y2 = self.XY[u]
			EP = "none" if vu and uv else "last" if vu else "first"
			LC = "grey" if vu and uv else "pink" if vu else "cyan"
			self.E[(v,u)] = self.w.create_line(x1,y1,x2,y2,fill=LC,arrow=EP)
		for v,(x,y) in enumerate(self.XY):
			self.w.create_text(x,y,fill="black",text=str(v))

	def rmedge(self):
		s = self.tE.get()
		try:
			v,u = map(int,s.split(','))
			if u<v: v,u = u,v
			if (v,u) in self.E:
				self.w.delete(self.E[(v,u)])
				self.E.pop((v,u),None)
				self.G.remove_edge(v,u,True)
		except: return

	def komplete(self):
		try:
			G = graph.kgraph(int(self.tE.get()))
			self.draw(G)
		except: return

	def random(self):
		s = self.tE.get()
		try:
			if ',' in s:
				G = graph.biclique(*map(int,s.split(',')))
			else:
				G = graph.simplize(graph.random_graph(int(s)))
			self.draw(G)
		except: return

	def random_bipartite(self):
		s = self.tE.get()
		try:
			if ',' in s:
				G = graph.random_bipartite(*map(int,s.split(',')))
				self.draw(G)
		except: return

def draw_simple_graph(sG):
	root = Tk.Tk()
	app = SimpleDraw(root)
	app.draw(sG)
	root.mainloop()

if __name__=="__main__":
	G = graph.biclique(3,5)
	draw_simple_graph(G)
