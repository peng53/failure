#
# Draw a graph using Tkinter

import Tkinter as Tk
import math
import itertools
import graph

class App:
	def __init__(self,master,sG):
		self.G = sG.adjm
		self.D = 800
		self.pointSize = 10
		self.R = self.D//2
		self.dA = 360/(len(self.G)-1)

		Tk.Button(root, text="Close", command=quit).pack()
		self.w = Tk.Canvas(root, width=self.D+20, height=self.D+20)
		self.w.pack()
		self.w.create_line(1,0,1,self.D+20)
		self.w.create_line(self.D+20,0,self.D+20,self.D+20)
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

def draw_simple_graph(sG):
	root = Tk.Tk()
	app = App(root,sG)
	root.mainloop()

def draw_simple_graph(sG):
	G = sG.adjm
	node_coords = []
	root = Tk.Tk()

	intersize = 800
	dia=4
	hk = intersize//2

	r = intersize//2
	angle_int = 360//(len(G)-1)
	w = Tk.Canvas(root, width=intersize+50, height=intersize+50)
	b = Tk.Button(root, text="Close", command=quit)
	b.pack()
	w.pack()

	for i,v in enumerate(G):
		x = r * math.cos(i*angle_int)+hk
		y = r * math.sin(i*angle_int)+hk
		node_coords.append((x,y))
		w.create_oval(x-dia,y-dia,x+dia,y+dia)
		w.create_text(x+2*dia,y+2*dia,text=str(i))

	for v,u in itertools.combinations(xrange(len(G)),2):
		vu = G[v][u] == 1
		uv = G[u][v] == 1
		x1,y1 = node_coords[v]
		x2,y2 = node_coords[u]
		if vu:
			if uv:
				w.create_line(x1,y1,x2,y2,arrow="both")
			else:
				w.create_line(x1,y1,x2,y2,arrow="last")
		elif not vu and uv:
			w.create_line(x1,y1,x2,y2,arrow="first")

	root.mainloop()

if __name__=="__main__":
	root = Tk.Tk()
	#app = App(root,graph.biclique(3,4))
	G = [\
	[0,0,0,1,0,0],\
	[0,0,0,0,1,0],\
	[0,0,0,0,0,1],\
	[0,0,1,0,0,0],\
	[1,0,0,0,0,0],\
	[0,1,0,0,0,0],\
	]
	app = App(root,graph.SimpleGraph(G))
	root.mainloop()
	#draw_simple_graph(graph.biclique(10,10))
