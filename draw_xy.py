"""
Phase 1: draw points where x&y>=0
Phase 2: allow negative x&y
Phase 3: connect points like a line graph
Phase 4: allow parameters/options other than default
Phase 5: linear regression
.
.
.
"""
import Tkinter as Tk

class XYpts:
	def __init__(self,XY,color,name=None):
		self.XY = sorted(XY)
		self.color = color
		self.name = name

class XYspread:
	def __init__(self,XYs,scale=1,x_range=None,y_range=None):
		self.XYs = XYs
		self.scale= scale
		if x_range is None:
			self.min_x = self.max_x = self.min_y = self.max_y = 0
			for pts in XYs:
				for (x,y) in pts.XY:
					if x<self.min_x: self.min_x = x
					elif x>self.max_x: self.max_x = x
					if y<self.min_y: self.min_y = y
					elif y>self.max_y: self.max_y = y
		else:
			self.min_x, self.max_x = x_range
			self.min_y, self.max_y = y_range

class XYPlane:
	def __init__(self,master,G):
		self.osd = None
		self.G = G
		scale = G.scale
		xM,yM = (G.min_x,G.max_x),(G.min_y,G.max_y)
		Tk.Button(master, text="Close", command=quit).pack()
		self.w = Tk.Canvas(master, width=1+scale*(abs(xM[0])+abs(xM[1])), height=1+scale*(abs(yM[0])+abs(yM[1])), background="white")
		self.w.bind("<Button-1>", self.drawosd)
		self.w.pack()

		if yM[0]<=0 and yM[1]>=0: self.w.create_line(0,scale*abs(yM[1]),scale*(abs(xM[0])+abs(xM[1])),scale*abs(yM[1]),fill="grey")
		if xM[0]<=0 and xM[1]>=0: self.w.create_line(scale*abs(xM[0]),0,scale*abs(xM[0]),scale*(abs(yM[0])+abs(yM[1])),fill="grey")
		#for e in G.XYs:
			#for (x,y) in e.XY:
				#cy = scale*(-y+yM[1])
				#cx = scale*(x+abs(xM[0]))
				#self.w.create_oval(cx,cy,cx+1,cy+1,outline=e.color)
		#for e in G.XYs:
			#x0 = scale*(e.XY[0][0]+abs(xM[0]))
			#y0 = scale*(-e.XY[0][0]+yM[1])
			#for (x,y) in e.XY:
				#y1 = scale*(-y+yM[1])
				#x1 = scale*(x+abs(xM[0]))
				#self.w.create_line(x0,y0,x1,y1,fill=e.color)
				#x0,y0=x1,y1

		for psets in G.XYs:
			x0 = y0 = None
			lpad = rpad = 3
			for (x,y) in psets.XY:
				b_draw = False
				x1,y1 = scale*(x+abs(xM[0])),scale*(-y+abs(yM[1]))
				if x>=xM[0] and x<=xM[1] and y>=yM[0] and y<=yM[1]: b_draw = True
				elif x<0 and lpad>0:
					b_draw = True
					lpad -=1
				elif x>0 and rpad>0:
					b_draw = True
					rpad -=1
				if b_draw is True:
					self.w.create_oval(x1,y1,x1+1,y1+1,outline=psets.color)
					if x0 is not None: self.w.create_line(x0,y0,x1,y1,fill=psets.color)
				x0,y0=x1,y1

	def drawosd(self,event):
		if self.osd is not None:
			for e in self.osd:
				self.w.delete(e)
		else:
			self.osd = []
		cx,cy = event.x,event.y
		for e in self.G.XYs:
			self.osd.append(self.w.create_rectangle(cx,cy,cx+16,cy+16,fill=e.color,width=0))
			self.osd.append(self.w.create_text(cx+48,cy+8,text=e.name))
			cy += 16


def frange(a,b,d):
	x = a
	while x<b:
		yield x
		x += d

def draw_graph(pts):
	root = Tk.Tk()
	app = XYPlane(root,pts)
	root.mainloop()


if __name__=="__main__":
	a,b,dx = -1.0, 1.0, 0.1
	x1 = XYpts([(x,x) for x in frange(a,b,dx)],"black","y=x")
	x2 = XYpts([(x,x**2) for x in frange(a,b,dx)],"red","y=x^2")
	x3 = XYpts([(x,x**3) for x in frange(a,b,dx)],"blue","y=x^3")
	x4 = XYpts([(x,x**4) for x in frange(a,b,dx)],"green","y=x^4")
	x5 = XYpts([(x,x**5) for x in frange(a,b,dx)],"orange","y=x^5")
	x6 = XYpts([(x,x**6) for x in frange(a,b,dx)],"pink","y=x^6")
	G = XYspread(\
		[\
		#x1,\
		x2,\
		x3,\
		x4,\
		x5,\
		x6\
		],75,(-2.0,2.0),(-2.0,2.0))
	draw_graph(G)

