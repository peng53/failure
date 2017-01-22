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


class XYPlane:
	def __init__(self,master,G):
		self.G = G
		self.max_dim = 800
		self.min_x, self.max_x = 0, 0
		self.min_y, self.max_y = 0, 0
		for e in G:
			for (x,y) in e.XY:
				self.min_x = min(self.min_x,x)
				self.max_x = max(self.max_x,x)
				self.min_y = min(self.min_y,y)
				self.max_y = max(self.max_y,y)
		#print self.min_x,self.max_x
		#print self.min_y,self.max_y
		Tk.Button(master, text="Close", command=quit).pack()
		self.scale = 300
		self.w = Tk.Canvas(master, width=self.scale*(abs(self.min_x)+self.max_x)+1, height=self.scale*(abs(self.min_y)+self.max_y)+1, background="white")
		self.w.bind("<Button-1>", self.drawosd)
		self.osd = None
		self.w.pack()
		self.w.create_line(0,self.scale*self.max_y,self.scale*(abs(self.min_x)+self.max_x),self.scale*self.max_y,fill="grey")
		self.w.create_line(self.scale*abs(self.min_x),0,self.scale*abs(self.min_x),self.scale*(abs(self.min_y)+self.max_y),fill="grey")
		for e in G:
			for (x,y) in e.XY:
				cy = self.scale*(-y+self.max_y)
				cx = self.scale*(x+abs(self.min_x))
				self.w.create_oval(cx,cy,cx+1,cy+1,outline=e.color)

		for e in G:
			#ATM assumes sorted by increasing x
			x0 = self.scale*(e.XY[0][0]+abs(self.min_x))
			y0 = self.scale*(-e.XY[0][0]+self.max_y)
			for (x,y) in e.XY:
				y1 = self.scale*(-y+self.max_y)
				x1 = self.scale*(x+abs(self.min_x))
				self.w.create_line(x0,y0,x1,y1,fill=e.color)
				x0,y0=x1,y1

	def drawosd(self,event):
		if self.osd is not None:
			for e in self.osd:
				self.w.delete(e)
		else:
			self.osd = []
		cx,cy = event.x,event.y
		for e in self.G:
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
	draw_graph([x1,x2,x3,x4,x5,x6])

