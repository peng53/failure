"""
Phase 1: draw points where x&y>=0
Phase 2: allow negative x&y
Phase 3: connect points like a line graph
Phase 4: allow parameters/options other than default
Phase 4.5: linear regression with numpy
	http://glowingpython.blogspot.com/2012/03/linear-regression-with-numpy.html
Phase 5: linear regression
Phase 6: rebuild(?!?) to use numpy arrays over lists
.
.
.
"""
import Tkinter as Tk
from collections import namedtuple
from numpy import array, arange
"""
Pt = namedtuple('pt','x y')
Rn = namedtuple('range','m M')

class XYpts:
	def __init__(self,XY,color,name=None):
		self.XY = sorted(XY)
		self.color = color
		self.name = name

class XYspread:
	def __init__(self,XYs,scale=1,x_range=None,y_range=None):
		if scale<0: raise ValueError
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
		xr,yr = Rn(G.min_x,G.max_x),Rn(G.min_y,G.max_y)
		Tk.Button(master, text="Close", command=quit).pack()
		H = 1+scale*(abs(yr.m)+abs(yr.M))
		W = 1+scale*(abs(xr.m)+abs(xr.M))
		self.w = Tk.Canvas(master, width=W, height=H, background="white")
		self.w.bind("<Button-1>", self.drawosd)
		self.w.pack()
		scalex = lambda x: scale*(x+abs(xr.m))
		scaley = lambda y: scale*(-y+abs(yr.M))
		if yr.m<=0 and yr.M>=0: self.w.create_line(0,scaley(0),W,scaley(0),fill="grey",dash=[2,1])
		if xr.m<=0 and xr.M>=0: self.w.create_line(scalex(0),0,scalex(0),H,fill="grey",dash=[2,1])

		for psets in G.XYs:
			A = np.array([[p.x for p in psets.XY],np.ones(len(psets.XY))])
			Y = np.array([p.y for p in psets.XY])
			W = np.linalg.lstsq(A.T,Y)[0]
			l0 = W[0]*xr.m + W[1]
			l1 = W[0]*xr.M + W[1]
			self.w.create_line(scalex(xr.m),scaley(l0),scalex(xr.M),scaley(l1),dash=[2,6,2],fill=psets.color)
			print W
			x0 = y0 = None
			lpad = rpad = 3
			for (x,y) in psets.XY:
				b_draw = False
				if x>=xr.m and x<=xr.M and y>=yr.m and y<=yr.M: b_draw = True
				elif x<0 and lpad>0:
					b_draw = True
					lpad -=1
				elif x>0 and rpad>0:
					b_draw = True
					rpad -=1
				#x1,y1 = scale*(x+abs(xr.m)),scale*(-y+abs(yr.M))
				x1,y1 = scalex(x),scaley(y)
				if b_draw is True:
					self.w.create_oval(x1,y1,x1+1,y1+1,outline=psets.color)
					if x0 is not None: self.w.create_line(x0,y0,x1,y1,fill=psets.color)
				x0,y0=x1,y1

	def drawosd(self,event):
		if self.osd is not None:
			for e in self.osd:
				self.w.delete(e)
		else: self.osd = []
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

def m_p_b(x,m,b):
	return x*m+b

def m_p_b_n(x,m,b,N):
	return x*m+b+random.uniform(-N,N)

def eval_xs(slope,yint,fromx,tox,changex,noise):
	xs = np.arange(fromx,tox,changex)
	ys = (xs*slope) + yint + (np.random.ranf(len(xs)) if noise else np.zeros(len(xs)))
	return np.array([xs,ys])
"""

R = namedtuple('domain_codomain','dm dM cm cM')
x_range = namedtuple('x_range','x0 x1 dx') # x_range should have x0<x1 & 0<dx ATM

class XYpts:
	def __init__(self,pts,color=None,name=None): #[(x,y)] form
		self.color,self.name = color,name
		self.x, self.y = array(sorted(pts)).T
		self.R = R(self.x[0],self.x[-1],self.y[0],self.y[-1])
	def xy_pts(self):
		for i,x in enumerate(self.x):
			yield x,self.y[i]

class XYpt_linear(XYpts):
	def __init__(self,xs,m,b,color=None,name=None):
		self.color,self.name = color,name
		self.x = arange(*xs) if isinstance(xs,x_range) else array(sorted(xs))
		self.y = self.x*m+b
		#mx = Mx = self.x[0]
		#for x in self.x[1:]:
			#if x<mx: mx = x
			#elif x>Mx: Mx = x
		#my = My = self.y[0]
		#for y in self.y[1:]:
			#if y<my: my = y
			#elif y>My: My = y
		# no need for above search as linear functions can only increase or decrease for increasing x
		self.R = R(self.x[0],self.x[-1],self.y[0],self.y[-1]) if m>0 else R(self.x[0],self.x[-1],self.y[-1],self.y[0])

class XYspread2:
	def __init__(self,XYs,scale=1,viewr=None):
		if scale<0: raise ValueError
		self.XYs = XYs
		self.scale = scale
		if viewr is None:
			mx,Mx,my,My = XYs[0].R
			for t in XYs[1:]:
				if t.R.dm < mx: mx = t.R.dm
				if t.R.dM > Mx: Mx = t.R.dM
				if t.R.cm < my: my = t.R.cm
				if t.R.cM > My: My = t.R.cM
			self.R = R(mx,Mx,my,My)
		else: self.R = viewr

class XYPlane2:
	def __init__(self,master,G):
		self.G = G
		self.osd = None
		R,scale = G.R,G.scale #xr,yr = Rn(G.min_x,G.max_x),Rn(G.min_y,G.max_y)
		H = 1+scale*(abs(R.cm)+abs(R.cM))
		W = 1+scale*(abs(R.dm)+abs(R.dM))
		Tk.Button(master, text="Close", command=quit).pack()
		self.w = Tk.Canvas(master, width=W, height=H, background="white")
		self.w.bind("<Button-1>", self.drawosd)
		self.w.pack()

		scalex = lambda x: scale*(x+abs(R.dm))
		scaley = lambda y: scale*(-y+abs(R.cM))

		# draw origin lines if (0,0) is visable
		if R.cm<=0 or R.cM>=0: self.w.create_line(0,scaley(0),W,scaley(0),fill="grey",dash=[2,1])
		if R.dm<=0 or R.dM>=0: self.w.create_line(scalex(0),0,scalex(0),H,fill="grey",dash=[2,1])

		for g in G.XYs:
			# lets focus on drawing first
			#A = np.array([[p.x for p in psets.XY],np.ones(len(psets.XY))])
			#Y = np.array([p.y for p in psets.XY])
			#W = np.linalg.lstsq(A.T,Y)[0]
			#l0 = W[0]*xr.m + W[1]
			#l1 = W[0]*xr.M + W[1]
			#self.w.create_line(scalex(xr.m),scaley(l0),scalex(xr.M),scaley(l1),dash=[2,6,2],fill=psets.color)
			#print W
			x0 = y0 = None
			lpad = rpad = 3
			for (x,y) in g.xy_pts():
				b_draw = False
				if x>=R.dm and x<=R.dM and y>=R.cm and y<=R.cM: b_draw = True
				elif x<0 and lpad>0: # outside but make an exception
					b_draw = True
					lpad -=1
				elif x>0 and rpad>0 or lpad>0: # outside but make an exception on the right
					b_draw = True
					rpad -=1
					lpad -=1 # if left-exceptions were not used, use them now
				x1,y1 = scalex(x),scaley(y)
				if b_draw is True:
					self.w.create_oval(x1,y1,x1+1,y1+1,outline=g.color)
					if x0 is not None: self.w.create_line(x0,y0,x1,y1,fill=g.color)
				x0,y0=x1,y1

	def drawosd(self,event):
		if self.osd is not None:
			for e in self.osd:
				self.w.delete(e)
		else: self.osd = []
		cx,cy = event.x,event.y
		for g in self.G.XYs:
			self.osd.append(self.w.create_rectangle(cx,cy,cx+16,cy+16,fill=g.color,width=0))
			self.osd.append(self.w.create_text(cx+48,cy+8,text=g.name))
			cy += 16



def draw_graph(pts):
	root = Tk.Tk()
	app = XYPlane(root,pts)
	root.mainloop()

def draw_graph2(G):
	root = Tk.Tk()
	app = XYPlane2(root,G)
	root.mainloop()


if __name__=="__main__":
	g = XYpt_linear(xs=x_range(-5,5,1),m=1,b=0,color="red",name="y=x")
	g2 = XYpt_linear(xs=x_range(-10,-5,1),m=-2,b=0,color="blue",name="y=-2x")
	G = XYspread2([g,g2],scale=25)
	draw_graph2(G)
	"""
	a,b,dx = -1.0, 1.0, 0.1
	x1 = XYpts([Pt(x,x) for x in frange(a,b,dx)],"black","y=x")
	x2 = XYpts([Pt(x,m_p_b_n(x,2,-2,0.05)) for x in frange(a,b,dx)], "red", "y=2x-2")
	x3 = XYpts([Pt(x,m_p_b_n(x,-0.5,0,0.01)) for x in frange(a,b,dx)], "blue", "y=-0.5x")
	x4 = XYpts([Pt(x,m_p_b_n(x,0.33,1,0.01)) for x in frange(a,b,dx)], "green", "y=0.33x-1")
	x5 = XYpts([Pt(x,m_p_b_n(x,0.0255,0.5,0.05)) for x in frange(a,b,dx)], "orange", "y=0.0255x-0.5")

	#x2 = XYpts([Pt(x,x**2) for x in frange(a,b,dx)],"red","y=x^2")
	#x3 = XYpts([Pt(x,x**3) for x in frange(a,b,dx)],"blue","y=x^3")
	#x4 = XYpts([Pt(x,x**4) for x in frange(a,b,dx)],"green","y=x^4")
	#x5 = XYpts([Pt(x,x**5) for x in frange(a,b,dx)],"orange","y=x^5")
	#x6 = XYpts([Pt(x,x**6) for x in frange(a,b,dx)],"pink","y=x^6")
	G = XYspread(\
		[\
		x1,\
		x2,\
		x3,\
		x4,\
		x5,\
		#x6\
		],200,(-2.0,2.0),(-2.0,2.0))
	draw_graph(G)
	"""
