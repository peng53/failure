# cfill_al.py
import numpy as np
import Queue

def qboard(height,width,colors):
	return np.random.randint(colors,size=(height,width))

def expand_sl_left(qb,coord_y,coord_x,color_match):
	#while coord_x-1>=0 and qb[coord_y,coord_x-1]==color_match:
	#	coord_x -= 1
	#return coord_x
	for x in xrange(coord_x,-1,-1):
		if qb[coord_y,x]!=color_match:
			return x+1
	return 0

def expand_sl_right(qb,coord_y,coord_x,color_match):
	width = qb.shape[1]
	#while coord_x+1<width and qb[coord_y,coord_x+1]==color_match:
	#	coord_x += 1
	#return coord_x
	for x in xrange(coord_x,width):
		if qb[coord_y,x]!=color_match:
			return x-1
	return width-1

def slfill(qb,coord_y,coord_x,new_color,changelog=None):
	if qb[coord_y,coord_x]==new_color:
		return qb
	old_color = qb[coord_y,coord_x]
	return slfill2(qb,coord_y,coord_x,expand_sl_right(qb,coord_y,coord_x,old_color),new_color,old_color,changelog)

def slfill2(qb,coord_y,coord_x,coord_x2,new_color,old_color,changelog=None):
	max_y = qb.shape[0]-1
	q = Queue.Queue()
	q.put((coord_y,coord_x,coord_x2))
	upper_x1 = lower_x1 = upper_y = lower_y = -1
	while not q.empty():
		y,x0,x1 = q.get()
		if qb[y,x0]!=old_color:
			continue
		if changelog==None:
			print "got", y,x0,x1
		else:
			changelog.append((y,x0,x1))
		for x in xrange(x0,x1+1):
			qb[y,x] = new_color

			if y>0 and qb[y-1,x]==old_color:
				if upper_y!=-1 and upper_x1+1==x:
					upper_x1 += 1
				else:
					if upper_x1!=-1:
						q.put((upper_y,expand_sl_left(qb,upper_y,upper_x0,old_color),upper_x1))
					upper_y = y-1
					upper_x1 = upper_x0 = x

			if y<max_y and qb[y+1,x]==old_color:
				if lower_y!=-1 and lower_x1+1==x:
					lower_x1 += 1
				else:
					if lower_x1!=-1:
						q.put((lower_y,expand_sl_left(qb,lower_y,lower_x0,old_color),lower_x1))
					lower_y = y+1
					lower_x1 = lower_x0 = x

		if upper_y!=-1:
			q.put((upper_y,expand_sl_left(qb,upper_y,upper_x0,old_color),expand_sl_right(qb,upper_y,upper_x1,old_color)))
			upper_y = upper_x1 = -1

		if lower_y!=-1:
			q.put((lower_y,expand_sl_left(qb,lower_y,lower_x0,old_color),expand_sl_right(qb,lower_y,lower_x1,old_color)))
			lower_x1 = lower_y = -1

	return qb

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
q2 = np.array(
[[0,0,0,0,0,0,0],
 [0,0,0,0,0,0,0],
 [0,0,0,0,0,1,0],
 [0,0,0,0,0,1,0]]
)
q3 = np.array(
[[0,0,0,0,0,1],
 [0,1,1,1,0,1],
 [0,1,0,1,0,1],
 [1,1,0,1,0,1],
 [0,0,0,0,0,1]]
)
q4 = qboard(7,10,4)
q5 = qboard(1,25,3)
chlog = []
print q5
i = raw_input()
while i!='q':
	slfill(q5,0,0,int(i))
	print q5
	i = raw_input()
#qufill(q,0,0,2,chlog)
#slfill(q3,0,0,2,chlog)
#print q3
#print chlog
