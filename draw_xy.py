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
from numpy import array, arange, ones, linalg, zeros
from random import normalvariate
from math import ceil

R = namedtuple('domain_codomain','dm dM cm cM')
x_range = namedtuple('x_range','x0 x1 dx') # x_range should have x0<x1 & 0<dx ATM

#def nstrpoly(s):
	#expect_coef = 1
	#coef_a = coef_b = 0
	#pow_a = pow_b = 0
	#one_power = 0
	#for i,c in enumerate(s):
		#if expect_coef:
			#if c.isdigit():
				#coef_b += 1
			#elif c=='-' and coef_a==coef_b: #unary minus
				#coef_b += 1
			#elif c in ('-','+'): #not unary minus and no power: end coef, output,
				#yield (0,float(s[coef_a:coef_b]))
				#coef_a = coef_b = i+1
			#elif c=='x':
				#expect_coef = 0
				#one_power = 1
				#pow_a = pow_b = i+1
		#elif c.isdigit():
			#pow_b += 1
		#elif c in ('+','-'):
			#coef = 1 if coef_a==coef_b else -1 if coef_b-coef_a==1 and s[coef_a]=='-' else float(s[coef_a:coef_b])
			#powi = 1 if one_power and pow_a==pow_b else 0 if pow_a==pow_b else int(s[pow_a:pow_b])
			#yield(powi,coef)
			#expect_coef = 1
			#one_power = 0
			#if c=='-': coef_a = coef_b = i
			#else: coef_a = coef_b = i+1
			#pow_a = pow_b = 0
	#coef = 1 if coef_a==coef_b else -1 if coef_b-coef_a==1 and s[coef_a]=='-' else float(s[coef_a:coef_b])
	#powi = 1 if one_power and pow_a==pow_b else 0 if pow_a==pow_b else int(s[pow_a:pow_b])
	#yield(powi,coef)

def strpoly(s):
	c, p = [], []
	i = 0
	while i<len(s):
		#eat unary minus
		if s[i]=='-':
			c.append('-')
			i+=1
		#eat coef
		while i<len(s) and (s[i].isdigit() or s[i]=='.'): #may be broken to multiple periods
			c.append(s[i])
			i+=1
		#end of coef, do I see an x?
		if i<len(s) and s[i]=='x': # then we may have a power
			i+=1
			# unary minus is not allowed for powers
			while i<len(s) and s[i].isdigit(): #no floating pt powers allowed!
				p.append(s[i])
				i+=1
			if len(p)==0: #power is 'invisible 1'
				p.append('1')
		#either end of poly or an operation (only +/- allowed)
		#push current term
		if len(p)==0: #a constant
			yield (0,float(''.join(c))) #currently defaulting to floats
		elif len(c)==0: #coef is 'invisible 1'
			yield (int(''.join(p)),1)
		else: #we have both a power and a coef
			if c==['-']: #invisible negative 1
				yield (int(''.join(p)),-1.)
			else:
				yield (int(''.join(p)),float(''.join(c)))
		c, p = [], []
		if i<len(s) and s[i]=='-': #minus as operation rather than unary
			c.append('-')
		i+=1

# strpoly usage, where s is expression like so: -2x4+4x-3. the integer after a x is a power.
# for p,c in strpoly('-2x4-4x-3'): print p,c # output should be sorted for use


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

def x_poly(P):
	s = sorted(P,reverse=True)
	deg, coef = s[0][0], 0
	if deg < 0: raise ValueError
	for d2, c2 in s:
		if d2 < 0: raise ValueError
		if d2 == deg: coef += c2
		else:
			yield coef
			for d3 in xrange(deg-1,d2,-1): yield 0
			deg, coef = d2, c2
	yield coef
	for d3 in xrange(deg-1,-1,-1): yield 0

class XYpts_poly(XYpts):
	def __init__(self,xs,p,opts={}):
		self.d={"color":"black","name":''}
		for o in opts: self.d[o] = opts[o]
		self.x = arange(*xs) if isinstance(xs,x_range) else array(sorted(xs))
		self.y = zeros(len(self.x))
		for c in x_poly(p): self.y = self.y * self.x + c
		self.genR()

class XYspread:
	def __init__(self,XYs,opts={}):
		self.d = {"%":1, "view": None, "px": 0, "py": 0}
		for o in opts: self.d[o] = opts[o]
		if self.d['%']<0: raise ValueError
		self.XYs = XYs
		self.R = R(min(t.R.dm for t in XYs)-self.d['px'],max(t.R.dM for t in XYs)+self.d['px'],\
			min(t.R.cm for t in XYs)-self.d['py'],max(t.R.cM for t in XYs)+self.d['py'])\
			if self.d['view'] is None else self.d['view']

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
	def __init__(self,master,G):
		self.master,self.G = master,G
		Tk.Button(master, text="Close", command=quit).pack()
		self.make_canvas()

	def LINE(self,x,y,x1,y1,c='black',d=()):
		self.w.create_line(x,y,x1,y1,fill=c,dash=d)

	def make_canvas(self):
		self.osd,R,scale = None, self.G.R, self.G.d['%']
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
		self.w.pack()
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

def draw_graph(G):
	root = Tk.Tk()
	app = XYPlane(root,G)
	root.mainloop()

if __name__=="__main__":
	gs = []
	#gs.append(XYpts_linear(xs=x_range(-5,5,1),m=1,b=0,opts={"color":"red","name":"x","lines":0}))
	#gs.append(XYpts(pts=lfox(-4.0,4.0,0.1,lambda x:x*x-2),opts={"color":"green","name":"x^2-2","lines":0}))
	#gs.append(XYpts_poly(xs=x_range(-4,4,0.1),p=[(2,1),(0,-2)],opts={"color":"green","name":"x^2-2","lines":0,"nls":0,"ls":0}))
	gs.append(XYpts_poly(xs=x_range(-1,1,0.01),p=[(3,1)],opts={'color':'pink','name':'x^3','lines':None,'ls':None}))
	#gs.append(XYpts(pts=lfox(-2.0,2.0,0.1,lambda x:(x+0.3)**4),opts={"color":"cyan","name":"(x+0.3)^4"}))
	viewr=None
	padx=0
	pady=0
	#viewr=R(-4.5,4.5,-4.5,4.5)
	#viewr=R(-1.25,1,-1.25,1.25)
	G = XYspread(gs,opts={'%':1000,'view':viewr,'px':padx,'py':pady,'grid':[0.25,0.25],'tick':[0.5,0.5]})
	draw_graph(G)
