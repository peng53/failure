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
from tkFileDialog import askopenfilename, asksaveasfilename
from tkColorChooser import askcolor
from numpy import array, arange, ones, linalg, zeros
from math import ceil
from rol_pol import *

class XYPlane:
	def __init__(self,master):
		self.G = XYspread([],opts={"px": 0, "py": 0})
		self.osd = None
		self.master = master
		self.f_main, self.f_eopt, self.f_graph, self.f_but, self.f_pst =\
			Frame(master), Frame(master), Frame(master), Frame(master), Frame(master)
		self.f_box2, self.f_box3 = Frame(self.f_eopt), Frame(self.f_eopt)
		self.w = Canvas(self.master, width=1, height=1, background="white")
		self.w.bind("<Button-1>", self.drawosd)

		self.f_main.grid(row=0)
		self.f_eopt.grid(row=1)
		self.f_box2.grid(column=0,row=1)
		self.f_box3.grid(column=1,row=1)
		self.f_graph.grid(row=2)
		self.f_but.grid(row=3)
		self.f_pst.grid(row=4)
		self.w.grid(column=1,row=0,rowspan=5)

		self.d = {\
			'exp':Entry(self.f_main,width=28), 'xs':Entry(self.f_main,width=28),\
			'name':Entry(self.f_box2,width=20), 'color':Entry(self.f_box2,width=20),\
			'view':Entry(self.f_graph,width=10), 'grid':Entry(self.f_graph,width=10),\
			'pad':Entry(self.f_graph,width=10), 'tick':Entry(self.f_graph,width=10),\
			'lines':BooleanVar(), 'ls':BooleanVar(), 'nls':BooleanVar(),\
		}
		self.d['exp'].insert('end',"-0.33x2+0.05x3")
		self.d['xs'].insert('end',"-5,9,0.1")
		self.d['color'].insert('end','red')
		self.d['grid'].insert('end','1,2')
		self.d['tick'].insert('end','2,4')
		self.c = {k:Checkbutton(self.f_box3,text=k.upper(),variable=self.d[k]) for k in ['lines','ls','nls']}
		for k in self.c: self.c[k].grid(sticky='w')

		Label(self.f_main,text="Polynomial").grid()
		Label(self.f_main,text="Expression").grid(sticky='w')
		self.d['exp'].grid()
		Label(self.f_main,text="x for Eval").grid(sticky='w')
		self.d['xs'].grid()

		Label(self.f_eopt,text="Extra Options").grid(row=0,columnspan=2)
		Label(self.f_box2,text="Name").grid(sticky='w')
		self.d['name'].grid()
		Label(self.f_box2,text="Color").grid(sticky='w')
		self.d['color'].grid()

		Label(self.f_graph,text="Graph Options").grid(row=0,column=0,columnspan=2)
		Label(self.f_graph,text="View").grid(row=1,column=0,sticky='w')
		self.d['view'].grid(row=2,column=0)
		Label(self.f_graph,text="Grid").grid(row=3,column=0,sticky='w')
		self.d['grid'].grid(row=4,column=0)
		Label(self.f_graph,text="Padding").grid(row=1,column=1,sticky='w')
		self.d['pad'].grid(row=2,column=1)
		Label(self.f_graph,text="Ticks").grid(row=3,column=1,sticky='w')
		self.d['tick'].grid(row=4,column=1)

		Button(self.f_but,text="Add",command=self.poly_get).grid(row=0,column=0,sticky='ew')
		Button(self.f_but,text="Pop",command=self.poly_pop).grid(row=0,column=1,sticky='ew')
		Button(self.f_but,text="Color",command=self.cus_color).grid(row=1,column=0,sticky='ew')
		Button(self.f_but,text="File",command=self.openf).grid(row=1,column=1,sticky='ew')
		Button(self.f_but,text="Draw",command=self.make_canvas).grid(row=2,column=0,sticky='ew')
		Button(self.f_but,text="Quit",command=quit).grid(row=2,column=1,sticky='ew')
		Button(self.f_but,text="Print",command=self.printw).grid(row=2,column=2,sticky='ew')

		Label(self.f_pst,text="Data Sets").grid()
		self.pst_sb = Scrollbar(self.f_pst)
		self.pst = Listbox(self.f_pst,yscrollcommand=self.pst_sb.set)
		self.pst.grid(sticky='nesw',column=0,row=1)
		self.pst_sb.config(command=self.pst.yview)
		self.pst_sb.grid(column=1,row=1,sticky='nesw')

	def openf(self):
		fn = askopenfilename()
		with open(fn,'r') as f: l = [map(float,line.strip().split(',')) for line in f]
		o = self.p_opts()
		if 'name' not in o: o['name']=fn
		self.d['name'].delete(0,'end')
		self.d['color'].delete(0,'end')
		x,y = zip(*l)
		try: P = d_pts(x,y)
		except: return
		P.add_opt(o)
		P.genR()
		self.G.add_set(P)
		self.pst.insert('end',o['name'])

	def entry_get(self,e):
		s = self.d[e].get()
		return None if len(s)==0 else s

	def p_opts(self):
		o = {"color":"black"}
		for k in ['name','color']:
			s = self.entry_get(k)
			if s: o[k]=s
		for k in self.c:
			b = self.d[k].get()
			if b: o[k]=True
		return o

	def cus_color(self):
		c = askcolor()
		if c!=(None, None):
			self.d['color'].delete(0,'end')
			self.d['color'].insert(0,c[1])

	def poly_get(self):
		try:
			e = self.entry_get('exp')
			x = self.entry_get('xs')
			if e is None or x is None: raise Exception
			P = str_pts(x,e)
			o = self.p_opts()
			P.add_opt(o)
			P.genR()
			self.G.add_set(P)
			self.pst.insert('end', o['name'] if 'name' in o else e)
			self.d['exp'].delete(0,'end')
			self.d['name'].delete(0,'end')
			self.d['color'].delete(0,'end')
			#for g in self.G.XYs: print id(g.d)
		except Exception as e: print(e)

	def poly_pop(self):
		if len(self.G.XYs)!=0:
			i = self.pst.index('active')
			self.G.XYs.pop(i)
			self.pst.delete(i)

	def LINE(self,x,y,x1,y1,c='black',d=()):
		self.w.create_line(x,y,x1,y1,fill=c,dash=d)

	def nicecst(self,e,N):
		v = self.entry_get(e)
		if v is not None:
			if ',' in v:
				v = v.split(',')
				if len(v) in N: return map(float,v)
			if 1 in N:
				return float(v)
		return None

	def gopts(self):
		o = {}
		v = self.nicecst('view',[2,4])
		if v: o['view'] = R(v[0],v[1],v[0],v[1]) if len(v)==2 else R(*v)
		v = self.nicecst('grid',[1,2])
		if v: o['grid'] = [v,v] if isinstance(v,float) else v
		v = self.nicecst('tick',[1,2])
		if v: o['tick'] = [v,v] if isinstance(v,float) else v
		v = self.nicecst('pad',[1,2])
		if v:
			if isinstance(v,float): o['px'] = o['py'] = v
			else: o['px'], o['py'] = v
		self.G.set_opts(o)
		#try:
			#v = map(float,self.entry_get('view').split(','))
			#if len(v)==4: o['view'] = R(*v)
			#elif len(v)==2: o['view'] = R(v[0],v[1],v[0],v[1])
		#except Exception as e: pass
		#try:
			#v = map(float,self.entry_get('grid').split(','))
			#if len(v)==1: o['grid'] = v*2
			#elif len(v)==2: o['grid'] = v
		#except Exception as e: pass
		#try:
			#v = map(float,self.entry_get('pad').split(','))
			#if len(v)==1: o['px'] = o['py'] = v[0]
			#elif len(v)==2: o['px'], o['py'] = v
		#except Exception as e: pass
		#try:
			#v = map(float,self.entry_get('tick').split(','))
			#if len(v)==1: o['tick'] = v*2
			#elif len(v)==2: o['tick'] = v
		#except Exception as e: pass
		#self.G.set_opts(o)

	def make_canvas(self):
		self.gopts()
		R = self.G.d['view'] if 'view' in self.G.d else self.G.recalc_view()
		self.scale = scale = round(700./max(R.dM-R.dm,R.cM-R.cm))
		W, H = 1+int(ceil(scale * abs(R.dM - R.dm))), 2+int(ceil(scale * abs(R.cM - R.cm)))
		self.w.delete('all')
		self.w.config(width=W,height=H)
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
			if 'ls' in g.d: # Least squares
				lr = g.lsq()
				self.LINE(scx(lr[0]),scy(lr[1]),scx(lr[2]),scy(lr[3]),g.d['color'],(2,6,2))
			if 'nls' in g.d: # Naive
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
		if self.osd:
			for e in self.osd:
				self.w.delete(e)
		else: self.osd = []
		cx,cy = event.x,event.y
		self.osd.append(self.w.create_text(cx,cy-32,text="%sx scale" %self.scale))
		for g in self.G.XYs:
			if 'name' in g.d:
				self.osd.append(self.w.create_rectangle(cx-16,cy+5,cx+16,cy+7,fill=g.d['color'],width=0))
				self.osd.append(self.w.create_text(cx,cy,text=g.d['name'],fill="black"))
				cy += 16
	def printw(self):
		fn = asksaveasfilename()
		with open(fn,'w') as f:
			f.write(self.w.postscript())
def draw_graph():
	root = Tk()
	app = XYPlane(root)
	root.mainloop()

if __name__=="__main__":
	draw_graph()
