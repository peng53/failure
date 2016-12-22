#
# Draw a graph using Tkinter

import Tkinter as Tk
import math

G = [\
	[1,1,0,1,0],\
	[0,1,1,0,0],\
	[0,0,1,0,0],\
	[0,0,0,1,0],\
	[1,0,0,0,1]\
]

node_coords = []
root = Tk.Tk()

intersize = 800
dia=10

w = Tk.Canvas(root, width=intersize+2*dia, height=intersize+2*dia)
w.pack()

perline = math.ceil(math.sqrt(len(G)))
linepart = intersize // (perline)

x = linepart
y = linepart
for i,v in enumerate(G):
	w.create_oval(x,y,x+dia,y+dia)
	w.create_text(x,y-dia,text=str(i))
	node_coords.append((x+dia//2,y+dia//2))
	x+=linepart
	if x>intersize:
		x=linepart
		y+=linepart

for v,v_adj in enumerate(G):
	for n,is_neighbor in enumerate(v_adj):
		if v!=n and is_neighbor==1:
			x1,y1 = node_coords[v]
			x2,y2 = node_coords[n]
			if v<n:
				x1 -= dia>>1
				x2 -= dia>>1
				y1 -= dia>>1
				y2 -= dia>>1
			else:
				x1 += dia>>1
				x2 += dia>>1
				y1 += dia>>1
				y2 += dia>>1
			w.create_line(x1,y1,x2,y2,arrow="last")

#for i,v in enumerate(L):
#	print (i+1)*linepart
#	w.create_oval((i+1)*linepart,10,(i+2)*linepart,20)



#w.create_line(0, 0, 200, 100)
#w.create_line(0, 100, 200, 0, fill="red", dash=(4, 4))

#w.create_rectangle(50, 25, 150, 75, fill="blue")


root.mainloop()
