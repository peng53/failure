#
# Python module for changing polynomials to usable form
import re
import heapq

strictterms = re.compile("-?\d+\.?\d*x\d+")
def rspol(s):
	"""
	strict: of A.axP form, all parameters required (except negative and .0 float)
	because strict requires x, coef, and power, the split will always
	gives two values: the coef and power; which just needs to be changed
	in int/float and swapped before yielding. caret can be added to regex
	for more 'strictness'
	"""
	for t in strictterms.finditer(s):
		coef,power = t.group(0).split('x')
		yield int(power),float(coef)

term_single_power = re.compile("-?\d+\.?\d*x\d*")
def rsppol(s):
	"same as strictterms except you can omit the x's power if it is 1"
	for t in term_single_power.finditer(s):
		coef,power = t.group(0).split('x')
		yield (1 if power=='' else int(power)),float(coef)

term_zero_power = re.compile("-?\d+(.\d+)?(x\d*)?")
def rzppol(s):
	"""
	same as term_single power except you can input constants without x**0
	uses form A.axP where x and P can be assumed (1x^0) => (1)
	peak-point cannot improve to assume 'invisible coef' without more check
	"""
	for t in term_zero_power.finditer(s):
		N = t.group(0).split('x')
		if len(N)==2: yield (1 if N[1]=='' else int(N[1])),float(N[0])
		else: yield 0,float(N[0])

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
	for p,c in riopol(S): heapq.heappush(h,(-p,c))
	np,c = heapq.heappop(h)
	while len(h)!=0:
		np2,c2 = heapq.heappop(h)
		if np2 == np: c+=c2
		else:
			yield c
			for d3 in xrange(np+1,np2,1): yield 0
			np,c = np2,c2
	yield c
	for d3 in xrange(np+1,1,1): yield 0

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
