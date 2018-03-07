# cfill_al.py
import numpy as np
import Queue

def qboard(height,width,colors):
	return np.random.randint(colors,size=(height,width))

def slfill(qb,coord_y,coord_x,new_color):
	if qb[coord_y,coord_x]==new_color:
		return

def qufill(qb,coord_y,coord_x,new_color,changelog=None):
	if qb[coord_y,coord_x]==new_color:
		return qb
	old_color = qb[coord_y,coord_x]
	q = Queue.Queue()
	q.put((coord_y,coord_x))
	height, width = qb.shape
	while not q.empty():
		y,w = q.get()
		e = w
		while w-1>=0 and qb[y,w-1]==old_color:
			w -= 1
		while e+1<width and qb[y,e+1]==old_color:
			e += 1
		if changelog!=None:
			changelog.append((w,e))
		while e>=w:
			qb[y,w]=new_color
			if y>0 and qb[y-1,w]==old_color:
				q.put((y-1,w))
			if y+1<height and qb[y+1,w]==old_color:
				q.put((y+1,w))
			w += 1
	return qb

q = np.array(
[[0, 0, 1, 1],
 [1, 0, 1, 1],
 [0, 0, 0, 0],
 [1, 0, 1, 1],
 [1, 0, 0, 0]]
)
chlog = []
print q
qufill(q,0,0,2,chlog)
print q
print chlog
