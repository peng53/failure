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
from Tkinter import *
from numpy import array, arange, ones, linalg, zeros
from math import ceil
from rol_pol import *


class XYspread:
	def __init__(self,XYs=[],opts={}):
		self.d = {"%":1, "view": None, "px": 0, "py": 0}
		for o in opts: self.d[o] = opts[o]
		if self.d['%']<0: raise ValueError
		#self.d = opts
		self.XYs = XYs

	def add_set(self,XY):
		self.XYs.append(XY)

	def recalc_view(self):
		self.R = R(min(t.R.dm for t in self.XYs)-self.d['px'],max(t.R.dM for t in self.XYs)+self.d['px'],\
			min(t.R.cm for t in self.XYs)-self.d['py'],max(t.R.cM for t in self.XYs)+self.d['py'])\
			if self.d['view'] is None else self.d['view']
		return self.R
	def set_opts(self,opts):
		for o in opts:
			self.d[o]=opts[o]

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
		self.G = XYspread()
		self.master = master
		self.f_main, self.f_box2, self.f_box3, self.f_graph, self.f_but =\
			Frame(master), Frame(master), Frame(master), Frame(master), Frame(master)
		self.f_main.grid(column=0,row=0)
		self.f_box2.grid(column=1,row=0)
		self.f_box3.grid(column=2,row=0)
		self.f_graph.grid(column=0,row=1)
		self.f_but.grid(column=1,row=1,columnspan=2)

		self.d = {\
			'exp':Entry(self.f_main,width=25),\
			'xs':Entry(self.f_main,width=25),\
			'name':Entry(self.f_box2,width=10),\
			'color':Entry(self.f_box2,width=10),\
			'view':Entry(self.f_graph,width=10),\
			'grid':Entry(self.f_graph,width=10),\
			'pad':Entry(self.f_graph,width=10),\
			'tick':Entry(self.f_graph,width=10),\
			'lines':BoolVar(),\
			'ls':BoolVar(),\
			'nls':BoolVar(),\
		}
		self.c = {\
			'lines':Checkbutton(self.f_box3,text="Lines",variable=self.d['lines']),\
			'ls':Checkbutton(self.f_box3,text="LS",variable=self.d['ls']),\
			'nls':Checkbutton(self.f_box3,text="NLS",variable=self.d['nls']),\
		}

		Label(self.f_main,text="Polynomial").grid()
		Label(self.f_main,text="Expression").grid(sticky='w')
		self.d['exp'].grid()
		Label(self.f_main,text="x for Eval").grid(sticky='w')
		self.d['xs'].grid()

		Label(self.f_box2,text="Extra Options").grid(row=0)
		Label(self.f_box2,text="Name").grid(row=1,sticky='w')
		self.d['name'].grid(row=2)
		Label(self.f_box2,text="Color").grid(row=3,sticky='w')
		self.d['color'].grid(row=4)

		self.c['lines'].grid(row=1,column=0,sticky='w')
		self.c['ls'].grid(row=2,column=0,sticky='w')
		self.c['nls'].grid(row=3,column=0,sticky='w')

		Label(self.f_graph,text="Graph").grid(row=0,column=0,columnspan=2)
		Label(self.f_graph,text="View").grid(row=1,column=0,sticky='w')
		self.d['view'].grid(row=2,column=0)
		Label(self.f_graph,text="Grid").grid(row=3,column=0,sticky='w')
		self.d['grid'].grid(row=4,column=0)
		Label(self.f_graph,text="Padding").grid(row=1,column=1,sticky='w')
		self.d['pad'].grid(row=2,column=1)
		Label(self.f_graph,text="Ticks").grid(row=3,column=1,sticky='w')
		self.d['tick'].grid(row=4,column=1)

		Button(self.f_but,text="Add",command=self.poly_get).grid(row=0,column=0,sticky='ew')
		Button(self.f_but,text="Pop").grid(row=1,column=0,sticky='ew')
		Button(self.f_but,text="Draw",command=self.make_canvas).grid(row=0,column=1,sticky='ew')
		Button(self.f_but,text="Quit",command=quit).grid(row=1,column=1,sticky='ew')

	def entry_get(self,e):
		if e in self.d:
			s = self.d[e].get()
			return None if len(s)==0 else s
		else: raise KeyError

	def poly_get(self):
		try:
			e = self.entry_get('exp')
			x = map(float,self.entry_get('xs').split(','))
			if e is None or x is None: raise Exception
			x = arange(*x) if len(x)==3 else array(sorted(x))
			y = poly_pts(x,e)
			o={"color":"black","name":''}
			for k in ['name','color']:
				s = self.entry_get(k)
				if s is not None: o[k]=s
			for k in self.c:
				b = self.d[k].get()
				if b: o[k]=True
			#print(e,x,o)
			P = XYpts(x,y,o)
			print(P)
			print(P.R)
			self.G.add_set(P)

		except Exception as e:
			print(e)

	def LINE(self,x,y,x1,y1,c='black',d=()):
		self.w.create_line(x,y,x1,y1,fill=c,dash=d)

	def gopts(self):
		o = {}
		try:
			v = map(float,self.entry_get('view').split(','))
			if len(v)==4: o['view'] = R(*v)
		except Exception as e: print e
		try:
			v = map(float,self.entry_get('grid').split(','))
			if len(v)==1: o['grid'] = v*2
			elif len(v)==2: o['grid'] = v
		except Exception as e: print e
		try:
			v = map(float,self.entry_get('pad').split(','))
			if len(v)==1: o['px'] = o['py'] = v[0]
			elif len(v)==2: o['px'], o['py'] = v
		except Exception as e: print e
		try:
			v = map(float,self.entry_get('tick').split(','))
			if len(v)==1: o['tick'] = v*2
			elif len(v)==2: o['tick'] = v
		except Exception as e: print e
		print(o)
		self.G.set_opts(o)


	def make_canvas(self):
		self.osd,scale = None, self.G.d['%']
		self.gopts()
		R = self.G.recalc_view()
		W, H = 1+int(ceil(scale * abs(R.dM - R.dm))), 2+int(ceil(scale * abs(R.cM - R.cm)))
		if W<200 or H<200 or W>1000 or H>1000:
			scale = round(700./max(R.dM-R.dm,R.cM-R.cm))
			print "Graph using scale:", scale
			W, H = 1+int(ceil(scale * abs(R.dM - R.dm))), 2+int(ceil(scale * abs(R.cM - R.cm)))
			print W,H

		self.w = Canvas(self.master, width=W, height=H, background="white")
		self.w.grid(row=2,columnspan=3)
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
	root = Tk()
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
