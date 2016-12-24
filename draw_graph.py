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
		self.w = Tk.Canvas(master, width=self.D+20, height=self.D+20)
		self.w.pack()
		self.tE = Tk.Entry(master)
		self.tE.pack()
		Tk.Button(master, text="Random/Biclique", command=self.random).pack()
		Tk.Button(master, text="Close", command=quit).pack()
		self.G,self.XY = None,None

	def draw(self,sG):
		self.w.delete(Tk.ALL)
		self.w.create_line(1,0,1,self.D+20)
		self.w.create_line(self.D+20,0,self.D+20,self.D+20)
		self.G = sG.adjm
		self.dA = 360/(len(self.G)-1)
		self.XY = [(self.R * math.cos(v*self.dA)+self.R+5,self.R * math.sin(v*self.dA)+self.R+5) for v in xrange(len(self.G))]
		for v,u in itertools.combinations(xrange(len(self.G)),2):
			vu,uv = self.G[v][u] == 1,self.G[u][v] == 1
			if not vu and not uv: continue
			x1,y1 = self.XY[v]
			x2,y2 = self.XY[u]
			EP = "both" if vu and uv else "last" if vu else "first"
			LC = "black" if vu and uv else "red" if vu else "blue"
			self.w.create_line(x1,y1,x2,y2,fill=LC,arrow=EP)
		for v,(x,y) in enumerate(self.XY):
			self.w.create_oval(x-self.pointSize,y-self.pointSize,x+self.pointSize,y+self.pointSize)
			self.w.create_text(x,y+2*self.pointSize,text=str(v))

	def random(self):
		s = self.tE.get()
		try:
			if ',' in s:
				G = graph.biclique(*map(int,s.split(',')))
			else:
				G = graph.simplize(graph.random_graph(int(s)))
			self.draw(G)
		except:
			return


def draw_simple_graph(sG):
	root = Tk.Tk()
	app = SimpleDraw(root)
	app.draw(G)
	root.mainloop()

if __name__=="__main__":
	"""
	G = [\
	[0,0,0,1,0,0],\
	[0,0,0,0,1,0],\
	[0,0,0,0,0,1],\
	[0,0,1,0,0,0],\
	[1,0,0,0,0,0],\
	[0,1,0,0,0,0],\
	]
	draw_simple_graph(graph.SimpleGraph(G))
	"""
	G = graph.biclique(10,3)
	draw_simple_graph(G)
