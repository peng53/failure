#
# Draw a graph using Tkinter

import Tkinter as Tk
import math
import random
G = [\
	[1,1,1,1,0],\
	[0,1,1,0,0],\
	[0,0,1,0,0],\
	[0,0,1,1,0],\
	[0,0,0,0,1]\
]

node_coords = []
root = Tk.Tk()

intersize = 800
dia=2

w = Tk.Canvas(root, width=intersize+50, height=intersize+50)
w.pack()

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
