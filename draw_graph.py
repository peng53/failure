#
# Draw a graph using Tkinter

import Tkinter as Tk
import math
import itertools
import graph

def draw_simple_graph(sG):
	G = sG.adjm
	node_coords = []
	root = Tk.Tk()

	intersize = 800
	dia=10
	hk = intersize//2
	print "hk",hk
	r = intersize//2
	angle_int = 360//len(G)
	print "angle_int",angle_int

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
	draw_simple_graph(graph.biclique(5,3))
