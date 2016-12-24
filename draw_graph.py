#
# Draw a graph using Tkinter

import Tkinter as Tk
import math
import random
import itertools
import graph
"""
G = [\
	[1,1,1,0,0],\
	[0,1,1,0,0],\
	[1,0,1,0,0],\
	[0,0,0,1,1],\
	[0,0,1,1,1]\
]
"""
G = graph.biclique(5,3).adjm

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

"""

for v,v_adj in enumerate(G):
	for n,is_neighbor in enumerate(v_adj):
		if v!=n and is_neighbor==1 and not visited[n]:
			x1,y1 = node_coords[v]
			x2,y2 = node_coords[n]
			if G[n][v]==1:
				w.create_line(x1,y1,x2,y2,arrow="both")
			else:
				w.create_line(x1,y1,x2,y2,arrow="last")
		visited[v]=True
"""
"""
perline = math.ceil(math.sqrt(len(G)))
linepart = intersize // (perline)
varry = 50
x = linepart
y = linepart
for i,v in enumerate(G):
	vpush = random.randint(10,varry)
	w.create_oval(x+vpush,y+vpush,x+dia+vpush,y+dia+vpush)
	w.create_text(x+vpush,y+vpush-dia,text=str(i))
	node_coords.append((x+dia//2+vpush,y+dia//2+vpush))
	x+=linepart
	if x>intersize:
		x=linepart
		y+=linepart
"""
"""
for v,v_adj in enumerate(G):
	for n,is_neighbor in enumerate(v_adj):
		if v!=n and is_neighbor==1:
			x1,y1 = node_coords[v]
			x2,y2 = node_coords[n]
			""
			if v<n:
				x1 -= dia
				x2 -= dia
				y1 -= dia
				y2 -= dia
			else:
				x1 += dia
				x2 += dia
				y1 += dia
				y2 += dia
			""
			w.create_line(x1,y1,x2,y2,arrow="last")
"""


root.mainloop()
