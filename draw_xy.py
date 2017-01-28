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
from numpy import array, arange, ones, linalg
from random import normalvariate
"""
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

def fox(x0,x1,dx,F):
	x = x0
	while x<x1:
		yield x,F(x)
		x += dx
lfox = lambda x0,x1,dx,F: [(x,y) for (x,y) in fox(x0,x1,dx,F)]


class XYpts:
	def __init__(self,pts,opts={}): #[(x,y)] form
		self.d={"color":"black","name":''}
		for o in opts:
			self.d[o] = opts[o]
		self.x, self.y = array(sorted(pts)).T
		my = My = self.y[0]
		for y in self.y[1:]:
			if y<my: my = y
			elif y>My: My = y
		self.R = R(self.x[0],self.x[-1],my,My)

	def xy_pts(self):
		for i,x in enumerate(self.x):
			yield x,self.y[i]
	def lsq(self):
		A = array([self.x,ones(self.x.size)])
		W = linalg.lstsq(A.T,self.y)[0]
		return [(self.x[0],self.x[0]*W[0]+W[1]),(self.x[-1],self.x[-1]*W[0]+W[1])]
	def nslq(self,parts):
		"naive segmented least squares"
		W = []
		#if 2*parts<self.x.size: return None
		cut = self.x.size//parts
		for i in xrange(0,self.x.size,cut):
			x = self.x[i:i+cut]
			y = self.y[i:i+cut]
			A = array([x,ones(x.size)])
			w = linalg.lstsq(A.T,y)[0]
			#W.append((x[0],x[-1],w[0],w[1]))
			W.append((x[0],w[0]*x[0]+w[1]))
			W.append((x[-1],w[0]*x[-1]+w[1]))

		return W

class XYpts_linear(XYpts):
	def __init__(self,xs,m,b,opts={}):
		self.d={"color":"black","name":''}
		for o in opts:
			self.d[o] = opts[o]
		self.x = arange(*xs) if isinstance(xs,x_range) else array(sorted(xs))
		self.y = self.x*m+b
		self.R = R(self.x[0],self.x[-1],self.y[0],self.y[-1]) if m>0 else R(self.x[0],self.x[-1],self.y[-1],self.y[0])

class XYspread:
	def __init__(self,XYs,scale=1,viewr=None,padx=0,pady=0):
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
			self.R = R(mx-padx,Mx+padx,my-pady,My+pady)
		else: self.R = viewr

class XYPlane:
	def __init__(self,master,G):
		self.G,self.osd,R,scale = G,None,G.R,G.scale
		W = 1+scale*(abs(R.dm)+abs(R.dM))
		H = 1+scale*(abs(R.cm)+abs(R.cM))
		scalex = lambda x: scale*(x+abs(R.dm))
		scaley = lambda y: scale*(-y+abs(R.cM))
		self.w = Tk.Canvas(master, width=W, height=H, background="white")
		self.w.bind("<Button-1>", self.drawosd)
		# draw origin lines if (0,0) is visible
		if R.cm<=0 or R.cM>=0: self.w.create_line(0,scaley(0),W,scaley(0),fill="grey",dash=[4,4])
		if R.dm<=0 or R.dM>=0: self.w.create_line(scalex(0),0,scalex(0),H,fill="grey",dash=[4,4])
		for g in G.XYs:
			if 'ls' in g.d:
				# Least squares
				lr =  g.lsq()
				self.w.create_line(scalex(lr[0][0]),scaley(lr[0][1]),scalex(lr[0][1]),scaley(lr[1][1]),dash=[2,6,2],fill=g.d['color'])
			if 'nls' in g.d:
				# Naive
				W = g.nslq(5)
				x0 = y0 = None
				for (x,y) in W:
					x1, y1 = scalex(x),scaley(y)
					if x0 is not None:
						self.w.create_line(x0,y0,x1,y1,dash=[4,3,4],fill="purple")
					x0, y0 = x1,y1
			x0 = y0 = None
			lpad = rpad = 3
			for (x,y) in g.xy_pts():
				b_draw = False
				if x>=R.dm and x<=R.dM and y>=R.cm and y<=R.cM: b_draw = True
				elif x<0 and lpad>0: # outside but make an exception
					b_draw = True
					lpad -=1
				elif x>0 and (rpad>0 or lpad>0): # outside but make an exception on the right
					b_draw = True
					rpad -=1
					lpad -=1 # if left-exceptions were not used, use them now
				x1,y1 = scalex(x),scaley(y)
				if b_draw is True:
					self.w.create_oval(x1,y1,x1+1,y1+1,outline=g.d['color'])
					if x0 is not None and 'lines' in g.d: self.w.create_line(x0,y0,x1,y1,fill=g.d['color'])
				x0,y0=x1,y1
		Tk.Button(master, text="Close", command=quit).pack()
		self.w.pack()
	def drawosd(self,event):
		if self.osd is not None:
			for e in self.osd:
				self.w.delete(e)
		else: self.osd = []
		cx,cy = event.x,event.y
		for g in self.G.XYs:
			self.osd.append(self.w.create_rectangle(cx-16,cy+5,cx+16,cy+7,fill=g.d['color'],width=0))
			#self.osd.append(self.w.create_rectangle(cx,cy,cx+16,cy+16,fill=g.color,width=0))
			self.osd.append(self.w.create_text(cx,cy,text=g.d['name'],fill="black"))
			#self.osd.append(self.w.create_text(cx+48,cy+8,text=g.name,fill=g.color))
			cy += 16

def draw_graph(G):
	root = Tk.Tk()
	app = XYPlane(root,G)
	root.mainloop()

if __name__=="__main__":
	normal = [(normalvariate(0,1),normalvariate(0,1)) for _ in xrange(500)]
	gs = []
	gs.append(XYpts_linear(xs=x_range(-5,5,1),m=1,b=0,opts={"color":"red","name":"x","lines":0}))
	#gs.append(XYpts_linear(xs=x_range(-10,-5,1),m=-2,b=0,opts={"color":"blue","name":"-2x"}))
	gs.append(XYpts(pts=lfox(-4.0,4.0,0.1,lambda x:x*x-2),opts={"color":"green","name":"x^2-2","lines":0}))
	#gs.append(XYpts_linear(xs=x_range(-1.0,1.0,0.1),m=2,b=-2,opts={"color":"pink","name":"2x-2"}))
	gs.append(XYpts(pts=lfox(-2.0,2.0,0.1,lambda x:(x+1)**3-3),opts={"color":"orange","name":"(x+1)^3-3",'ls':0}))
	#gs.append(XYpts(pts=lfox(-2.0,2.0,0.1,lambda x:(x+0.3)**4),opts={"color":"cyan","name":"(x+0.3)^4"}))
	gs.append(XYpts(pts=lfox(-2.0,2.0,0.1,lambda x:x**5+0.1),opts={"color":"grey","name":"x^5+0.1","lines":0,'nls':0}))
	gs.append(XYpts(pts=lfox(-2.0,2.0,0.1,lambda x:x**6-1),opts={"color":"brown","name":"x^6-1","lines":0}))
	#gs.append(XYpts(pts=normal,opts={'name':'normal','color':'blue'}))
	viewr=None
	padx=0.2
	pady=0.2
	viewr=R(-4.5,4.5,-4.5,4.5)
	G = XYspread(gs,scale=50,viewr=viewr,padx=padx,pady=pady)
	draw_graph(G)
