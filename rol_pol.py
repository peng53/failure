#
# Python module for changing polynomials to usable form
import re
import heapq
from collections import namedtuple
from numpy import array, arange, ones, linalg, zeros

ione_term = re.compile("(-?x\d*)|(-?\d+(\.\d+)?(x\d*)?)")
def riopol(s):
	"""
	supports: invisible coefficient &| power, floats, constants, and negatives
	uses two primary cases: x with invis 1 and constant with optional x
	pretty much should cover all cases in implied form
	to use x^p rather than xp, replace x with x\^ in regex and in function below (maybe?)
	"""
	for r_t in ione_term.finditer(s):
		t = r_t.group(0)
		if 'x' in t:
			c,p = t.split('x',1)
			#       invis 1*       unary minus
			c = 1 if len(c)==0 else -1 if c=='-' else float(c)
			#       invis ^1
			p = 1 if len(p)==0 else int(p)
			yield p,c
		else: yield 0,float(t) # constant

def conLinPol(s):
	"""
	For checking degree of s.
	Only checks for 0, 1, or 1+
	Hmm if first term yielded by s_poly has degree 0|1, I can assume s is constant|linear
	and apply specilized functions..
	"""
	d = 0
	for r_t in ione_term.finditer(s):
		t = r_t.group(0)
		if 'x' in t:
			c,p = t.split('x',1)
			if len(p)==0 and d==0: d = 1
			else: d = max(int(p),d)
	return d

def strpoly(s):
	"""strpoly usage, where s is expression like so: -2x4+4x-3. the integer after a x is a power.
	   for p,c in strpoly('-2x4-4x-3'): print p,c # output should be sorted for use"""
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
			if len(p)==0: p.append('1') #power is 'invisible 1'
		#either end of poly or an operation (only +/- allowed)
		#push current term
		if len(p)==0: yield (0,float(''.join(c)))  #a constant, defaulting to floats
		elif len(c)==0: yield (int(''.join(p)),1) #coef is 'invisible 1'
		else: #we have both a power and a coef
			if c==['-']: yield (int(''.join(p)),-1.) #invisible negative 1
			else: yield (int(''.join(p)),float(''.join(c)))
		c, p = [], []
		if i<len(s) and s[i]=='-': c.append('-')  #minus as operation rather than unary
		i+=1

def s_poly(S):
	"""
	Takes a poly string of implied form and spits out coef in decreasing order
	while filling zeros inbetween powers. Uses a heap to sort incoming values
	from string parser. Due to being a min heap, I had to invert the powers
	but otherwise acts like a max heap. A generator that uses a generator!
	Ideal for my use since I don't need the values stored (as I use them
	right away via numpy)
	"""
	h = []
	for p,c in riopol(S):
		if p<0: raise ValueError
		heapq.heappush(h,(-p,c))
	np,c = heapq.heappop(h)
	while len(h)!=0:
		np2,c2 = heapq.heappop(h)
		if np2 == np: c+=c2
		else:
			yield c
			for d3 in xrange(np+1,np2,1): yield 0
			np,c = np2,c2
	yield c
	for d3 in range(np+1,1,1): yield 0

def x_poly(P):
	"""
	generator for input poly from greatest to least power
	input: list-like object containing degree,coefficient pairs (tuple or list)
	output: sorted degree,coefficient pairs where degree is decreasing by 1 until 0
	does not accept negative degrees and will raise a ValueError.
	ideal for use in Horner's method
	"""
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

# Intended equalities for like input Es
# strpoly, riopol
# s_poly, x_poly

def test_eq(g1,g2,E,E2=None):
	"""
	Used to test whether two generators given same input (or like input)
	will return same output. Could have tested equality on the fly (w/o
	lists) but storing allows quick lookthrough
	The last parameter (E2) is for when the input type doesn't match
	"""
	l1 = list(g1(E))
	l2 = list(g2(E if E2==None else E2))
	if len(l1)!=len(l2):
		print "Different yield sizes. %d - %d\n%s\n%s" %(len(l1),len(l2),', '.join(l1),', '.join(l2))
		return False
	for i in xrange(len(l1)):
		if l1[i]!=l2[i]:
			print "Non matching items: %f ? %f" %(l1[i],l2[i])
			return False
	print "Same total yield."
	return True

def poly_pts(xs,E):
	# xs is a numpy array
	ys = zeros(len(xs))
	try:
		g = s_poly if isinstance(E,str) else x_poly
		for c in g(E):
			ys = ys*xs + c
		return ys
	except ValueError:
		raise ValueError
		return

R = namedtuple('domain_codomain','dm dM cm cM')
x_range = namedtuple('x_range','x0 x1 dx') # x_range should have x0<x1 & 0<dx ATM

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
	# Allows to get x,f(x) points on with range and lambda function
	# Useful for functions like (x-2)^3+4 since implied form doesn't allow
	# for parenthesis (and negative powers)
	x = x0
	if dx>0:
		while x<x1:
			yield x,F(x)
			x+=dx
	else:
		 while x>x1:
			yield x,F(x)
			x+=dx

class XYpts:
	def __init__(self,xs,ys,d=None): #[(x,y)] form
		self.x, self.y = xs, ys
		self.d = {} if d is None else d
	def __str__(self):
		return '\n'.join('(%f, %f)' %(x,self.y[i]) for i,x in enumerate(self.x))
	def __repr__(self):
		return '\n'.join('(%f, %f)' %(x,self.y[i]) for i,x in enumerate(self.x))
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
			yield (x[0],w[0]*x[0]+w[1])
			yield (x[-1],w[0]*x[-1]+w[1])
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
	def add_opt(self,d):
		for o in d:
			self.d[o]=d[o]

class XYpts_ex(XYpts):
	def __init__(self,xs,ex,d=None):
		# where xs is raw or a range and ex is str expression
		self.x = arange(*xs) if len(xs)==3 else array(sorted(xs))
		self.y = poly_pts(self.x,ex)
		self.d = {} if d is None else d

class XYpts_raw(XYpts):
	def __init__(self,xs,ys,d=None):
		xy = array([xs,ys]).T
		gxy = xy[xy[:,0].argsort()]
		self.x, self.y = gxy[:,0], gxy[:,1]
		self.d = {} if d is None else d

class XYpts_fx(XYpts):
	def __init__(self,xr,F,d=None):
		if isinstance(xr,x_range):
			self.x, self.y = [], []
			for x,y in fox(*xr,F=F):
				self.x.append(x)
				self.y.append(y)
		else:
			self.x = xr
			self.y = [F(x) for x in xr]
		self.d = {} if d is None else d

def d_pts(lx,ly):
	# Where lx is an x_range or iterable of floats
	# and ly is list of terms or iterable of floats
	# *have not solved kinks with certain inputs
	if len(lx)==len(ly): return XYpts_raw(array(lx),array(ly))
	else: return XYpts_ex(array(lx),ly)

def str_pts(qx,qy):
	try:
		for c in qy:
			if c==',':
				x, y = array(map(float,qx.split(','))), array(map(float,qy.split(',')))
				if len(x)!=len(y): raise Exception
				return XYpts_raw(x,y)
			if c=='x' or c=='+':
				x = map(float,qx.split(','))
				return XYpts_ex(x,qy)
	except Exception as e:
		print e
		raise ValueError

class XYspread:
	def __init__(self,XYs=None,opts=None):
		self.d = {'px':0, 'py':0}
		if opts is not None:
			for k in opts:
				self.d[k] = opts[k]
		self.XYs = [] if XYs is None else XYs

	def add_set(self,XY):
		self.XYs.append(XY)

	def recalc_view(self):
		self.R = R(min(t.R.dm for t in self.XYs)-self.d['px'],max(t.R.dM for t in self.XYs)+self.d['px'],\
			min(t.R.cm for t in self.XYs)-self.d['py'],max(t.R.cM for t in self.XYs)+self.d['py'])
		return self.R
	def set_opts(self,opts):
		self.d = {'px':0, 'py':0}
		for o in opts:
			self.d[o]=opts[o]

def main():
	e = "10x+3x3-100"
	x = "-30,-5,5"
	x2 = "-30,-25,-20,-15,-10"
	y2 = "3,2,4,5,1"
	x3 = [-30,-25,-20,-15,-10]
	y3 = [3,2,4,5,1]
	e3 = [(1,10),(3,3),(0,-100)]
	xr = x_range(-30,-5,5)
	l = lambda x: 10*x + 3*x**3 - 100
	print "Init XYpts object with"
	print "-\nExpression and x_range string:"
	print "%s and %s" %(e,x)
	XY = str_pts(x,e)
	print XY
	print "-\nExpression and x points in string:"
	print "%s and %s" %(e,x2)
	XY = str_pts(x2,e)
	print XY
	print "-\nx points and y points in seperate strings:"
	print "%s and %s" %(x2,y2)
	XY = str_pts(x2,y2)
	print XY
	print "-\nx points and y points in seperate lists:"
	print "%s and %s" %(x3,y3)
	XY = d_pts(x3,y3)
	print XY
	print "-\nx points and expression as lists:"
	print "%s and %s" %(x3,e3)
	print "*cavaet: expression is deg,coef, not coef deg as expected"
	XY = d_pts(x3,e3)
	print XY
	print "-\nx range and lambda function:"
	print "%s and %s (as x range and lambda)" %(x,e)
	XY = XYpts_fx(xr,l)
	print XY
	print "-\nx list and lambda function:"
	print "%s and %s (as x list and lambda)" %(x3,e)
	XY = XYpts_fx(x3,l)
	print XY

if __name__=="__main__":
	main()
