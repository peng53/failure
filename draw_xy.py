"""
Phase 1: draw points where x&y>=0
Phase 2: allow negative x&y
Phase 3: connect points like a line graph
Phase 4: allow parameters/options other than default
Phase 4.5: linear regression with numpy
	http://glowingpython.blogspot.com/2012/03/linear-regression-with-numpy.html
Phase 5: linear regression
Phase 6: rebuild(?!?) to use numpy arrays over lists
Phase 7: polynomial string parsing
	so that point sets can be drawn via GUI and not by editing the source!
.
.
.
"""
import Tkinter as Tk
from collections import namedtuple
from numpy import array, arange, ones, linalg, zeros
from math import ceil
from rol_pol import x_poly, s_poly

R = namedtuple('domain_codomain','dm dM cm cM')
x_range = namedtuple('x_range','x0 x1 dx') # x_range should have x0<x1 & 0<dx ATM

class XYpts:
	def __init__(self,pts,opts={}): #[(x,y)] form
		self.d={"color":"black","name":''}
		for o in opts: self.d[o] = opts[o]
		self.x, self.y = array(sorted(pts)).T
		self.genR()
	def xy_pts(self):
		for i,x in enumerate(self.x): yield x,self.y[i]
	def lsq(self):
		A = array([self.x,ones(self.x.size)])
		W = linalg.lstsq(A.T,self.y)[0]
		return (self.x[0],self.x[0]*W[0]+W[1],self.x[-1],self.x[-1]*W[0]+W[1])
	def nslq(self,parts):
		"naive segmented least squares"
		if self.x.size>>1<parts: return
		cut = self.x.size//parts
		for i in xrange(0,self.x.size,cut):
			x, y = self.x[i:i+cut], self.y[i:i+cut]
			A = array([x,ones(x.size)])
			w = linalg.lstsq(A.T,y)[0]
			#W.append((x[0],w[0]*x[0]+w[1])) #W.append((x[-1],w[0]*x[-1]+w[1]))
			yield (x[0],w[0]*x[0]+w[1])
			yield (x[-1],w[0]*x[-1]+w[1])
		#return W
	def genR(self):
		# X is assumed to be sorted.
		my = My = self.y[0]
		for y in self.y:
			if y < my: my = y
			elif y > My: My = y
		self.R = R(self.x[0],self.x[-1],my,My)
	def pixel_pts(self,scale,minx,maxy):
		SX = scale*(self.x-minx)
		SY = scale*(maxy-self.y)
		for i,x in enumerate(SX): yield x,SY[i]

class XYpts_linear(XYpts):
	def __init__(self,xs,m,b,opts={}):
		self.d={"color":"black","name":''}
		for o in opts: self.d[o] = opts[o]
		self.x = arange(*xs) if isinstance(xs,x_range) else array(sorted(xs))
		self.y = self.x*m+b
		self.R = R(self.x[0],self.x[-1],self.y[0],self.y[-1]) if m>0 else R(self.x[0],self.x[-1],self.y[-1],self.y[0])

class XYpts_poly(XYpts):
	def __init__(self,xs,p,opts={}):
		self.d={"color":"black","name":''}
		for o in opts: self.d[o] = opts[o]
		self.x = arange(*xs) if isinstance(xs,x_range) else array(sorted(xs))
		self.y = zeros(len(self.x))
		g = s_poly if isinstance(p,str) else x_poly
		for c in g(p): self.y = self.y * self.x + c
		self.genR()

class XYspread:
	def __init__(self,XYs=[],opts={}):
		self.d = {"%":1, "view": None, "px": 0, "py": 0}
		for o in opts: self.d[o] = opts[o]
		if self.d['%']<0: raise ValueError
		self.XYs = XYs

	def add_set(self,XY):
		self.XYs.append(XY)

	def recalc_view(self):
		self.R = R(min(t.R.dm for t in self.XYs)-self.d['px'],max(t.R.dM for t in self.XYs)+self.d['px'],\
			min(t.R.cm for t in self.XYs)-self.d['py'],max(t.R.cM for t in self.XYs)+self.d['py'])\
			if self.d['view'] is None else self.d['view']
		return self.R

def frange(x0,x1,dx):
	x = x0
	if dx>0:
		while x<x1:
			yield x
			x+=dx
	else:
		 while x>x1:
			yield x
			x+=dx
def fox(x0,x1,dx,F):
	x = x0
	if dx>0:
		while x<x1:
			yield x,F(x)
			x+=dx
	else:
		 while x>x1:
			yield x,F(x)
			x+=dx

class XYPlane:
	def __init__(self,master):
		self.master = master
		self.G = XYspread()
		Tk.Button(master, text="Close", command=quit).grid(row=0,column=3)
		Tk.Label(master,text="Poly Expression").grid(row=1,column=0)
		self.p_ent = Tk.Entry(master)
		self.p_ent.grid(row=1,column=1,columnspan=5,sticky='ew')
		Tk.Label(master,text="x Bounds").grid(row=2,column=0)
		self.mx_ent = Tk.Entry(master)
		self.mx_ent.grid(row=2,column=1,sticky='ew')
		self.Mx_ent = Tk.Entry(master)
		self.Mx_ent.grid(row=2,column=2,sticky='ew')
		Tk.Label(master,text="Interval").grid(row=2,column=3)
		self.int_ent = Tk.Entry(master)
		self.int_ent.grid(row=2,column=4,sticky='ew')
		Tk.Button(master, text="Include Poly", command=self.inc_poly).grid(row=0,column=1)
		Tk.Button(master, text="Build Canvas", command=self.make_canvas).grid(row=0,column=2)
		Tk.Label(master,text="Name").grid(row=3,column=0)
		self.name_ent = Tk.Entry(master)
		self.name_ent.grid(row=3,column=1)
		Tk.Label(master,text="Color").grid(row=3,column=2)
		self.col_ent = Tk.Entry(master)
		self.col_ent.grid(row=3,column=3)




		#self.make_canvas()

	def inc_poly(self):
		try:
			s = self.p_ent.get()
			self.p_ent.delete(0,'end')
			mx = float(self.mx_ent.get())
			Mx = float(self.Mx_ent.get())
			dx = abs(float(self.int_ent.get()))
			c = self.col_ent.get()
			if c=='': c = 'black'
			self.col_ent.delete(0,'end')
			N = self.name_ent.get()
			if N=='': N = s
			self.name_ent.delete(0,'end')
			xs = x_range(mx,Mx,dx)
			xy = XYpts_poly(xs,s,{'name':N,'color':c})
			self.G.add_set(xy)
		except Exception as e:
			print e


	def LINE(self,x,y,x1,y1,c='black',d=()):
		self.w.create_line(x,y,x1,y1,fill=c,dash=d)

	def make_canvas(self):
		self.osd,scale = None, self.G.d['%']
		R = self.G.recalc_view()
		W, H = 1+int(ceil(scale * abs(R.dM - R.dm))), 2+int(ceil(scale * abs(R.cM - R.cm)))
		print W,H
		# if graph is of size [200,1000]: leave alone
		# else: set scale so it is
		# work {
		if W<200 or H<200 or W>1000 or H>1000:
			scale = round(700./max(R.dM-R.dm,R.cM-R.cm))
			print "Graph using scale:", scale
			W, H = 1+int(ceil(scale * abs(R.dM - R.dm))), 2+int(ceil(scale * abs(R.cM - R.cm)))
			print W,H
		# } end work
		"""
		if W<199 or H<199:
			print 'm2'
			scale = (min(W,H)//256+1)*100
			print "Graph too small! using scale:",scale
			W, H = 1+int(ceil(scale * abs(R.dM - R.dm))), 2+int(ceil(scale * abs(R.cM - R.cm)))
		elif W>1000 or H>1000:
			print 'm1'
			scale = (256//max(W,H)+1)*100
			print "Graph too large! using scale:",scale
			W, H = 1+int(ceil(scale * abs(R.dM - R.dm))), 2+int(ceil(scale * abs(R.cM - R.cm)))
			print W,H
		"""

		self.w = Tk.Canvas(self.master, width=W, height=H, background="white")
		self.w.grid(row=4,columnspan=6)
		self.w.bind("<Button-1>", self.drawosd)
		scx = lambda x: scale*(x-R.dm)
		scy = lambda y: scale*(R.cM-y)
		scp = lambda x,y: (scale*(x-R.dm), scale*(R.cM-y))
		OX,OY = scp(0,0)
		if R.cm<=0 and R.dm<=0: self.w.create_text(OX,OY,text='0')
		if R.cm<=0: self.LINE(0,OY,W,OY,'#333',(1,2))
		if R.dm<=0: self.LINE(OX,0,OX,H,"#333",[1,2])
		if 'grid' in self.G.d:
			tx,ty = self.G.d['grid']
			for x in arange(scx(tx),W,scale*tx): self.LINE(x,0,x,H,"#ccc",4)
			for x in arange(scx(-tx),0,-scale*tx): self.LINE(x,0,x,H,"#ccc",4)
			for y in arange(scy(ty),H,scale*ty): self.LINE(0,y,W,y,"#ccc",4)
			for y in arange(scy(-ty),0,-scale*ty): self.LINE(0,y,W,y,"#ccc",4)
		if 'tick' in self.G.d:
			tx,ty = self.G.d['tick']
			for t,x in fox(tx,R.dM,tx,scx): self.w.create_text(x,OY,text=str(t))
			for t,x in fox(-tx,R.dm,-tx,scx): self.w.create_text(x,OY,text=str(t))
			for t,y in fox(ty,R.cM,ty,scy): self.w.create_text(OX,y,text=str(t))
			for t,y in fox(-ty,R.cm,-ty,scy): self.w.create_text(OX,y,text=str(t))

		for g in self.G.XYs:
			if 'ls' in g.d:
				# Least squares
				lr = g.lsq()
				self.LINE(scx(lr[0]),scy(lr[1]),scx(lr[2]),scy(lr[3]),g.d['color'],(2,6,2))
			if 'nls' in g.d:
				# Naive
				x0 = y0 = None
				for x,y in g.nslq(3):
					x1, y1 = scp(x,y)
					if x0 is not None: self.LINE(x0,y0,x1,y1,"purple",[4,3,4])
					x0, y0 = x1, y1

			x0 = y0 = None
			lpad = rpad = 3
			for x,y in g.pixel_pts(scale,R.dm,R.cM):
				b_draw = False
				if 0<=x<=W and 0<=y<=H: b_draw = True
				elif x<W>>1 and lpad>0: # outside but make an exception
					b_draw = True
					lpad -=1
				elif x>W>>1 and (rpad>0 or lpad>0): # outside but make an exception on the right
					b_draw = True
					rpad -=1
					lpad -=1 # if left-exceptions were not used, use them now
				if b_draw is True:
					self.w.create_oval(x,y,x+1,y+1,outline=g.d['color'])
					if x0 is not None and 'lines' in g.d: self.LINE(x0,y0,x,y,g.d['color'])
				x0,y0=x,y
	def drawosd(self,event):
		if self.osd is not None:
			for e in self.osd:
				self.w.delete(e)
		else: self.osd = []
		cx,cy = event.x,event.y
		for g in self.G.XYs:
			self.osd.append(self.w.create_rectangle(cx-16,cy+5,cx+16,cy+7,fill=g.d['color'],width=0))
			self.osd.append(self.w.create_text(cx,cy,text=g.d['name'],fill="black"))
			cy += 16

def draw_graph():
	root = Tk.Tk()
	app = XYPlane(root)
	root.mainloop()

if __name__=="__main__":
	#gs = []
	#gs.append(XYpts_linear(xs=x_range(-5,5,1),m=1,b=0,opts={"color":"red","name":"x","lines":0}))
	#gs.append(XYpts(pts=lfox(-4.0,4.0,0.1,lambda x:x*x-2),opts={"color":"green","name":"x^2-2","lines":0}))
	#gs.append(XYpts_poly(xs=x_range(-4,4,0.1),p=[(2,1),(0,-2)],opts={"color":"green","name":"x^2-2","lines":0,"nls":0,"ls":0}))
	#gs.append(XYpts_poly(xs=x_range(-1,1,0.01),p=[(3,1)],opts={'color':'pink','name':'x^3','lines':None,'ls':None}))
	#gs.append(XYpts(pts=lfox(-2.0,2.0,0.1,lambda x:(x+0.3)**4),opts={"color":"cyan","name":"(x+0.3)^4"}))
	#viewr=None
	#padx=0
	#pady=0
	#viewr=R(-4.5,4.5,-4.5,4.5)
	#viewr=R(-1.25,1,-1.25,1.25)
	#G = XYspread(gs,opts={'%':1000,'view':viewr,'px':padx,'py':pady,'grid':[0.25,0.25],'tick':[0.5,0.5]})
	draw_graph()
