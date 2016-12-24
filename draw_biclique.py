#
# Draw a graph using Tkinter

import Tkinter as Tk
import graph
import itertools

root = Tk.Tk()
W=200
H=200
w = Tk.Canvas(root, width=W, height=H)
w.pack()
dia=5

w.create_line(1,1,1,H)
w.create_line(W-1,1,W-1,H)

m = 3
n = 2
G = graph.biclique(m,n).adjm
node_coords = []

x,dy = W//4,H//m
for i,y in enumerate(xrange(dy//2,dy*m,dy)):
	node_coords.append((x,y))
	w.create_oval(x,y,x+dia,y+dia)
	w.create_text(x-30,y+dia,text=str(i))
x,dy = x*3, H//n
for i,y in enumerate(xrange(dy//2,dy*n,dy)):
	node_coords.append((x,y))
	w.create_oval(x,y,x+dia,y+dia)
	w.create_text(x+30,y+dia,text=str(i))

for v,u in itertools.combinations(xrange(len(G)),2):
	if G[v][u] == 1:
		x1,y1 = node_coords[v]
		x2,y2 = node_coords[u]
		w.create_line(x1,y1,x2,y2)

root.mainloop()
