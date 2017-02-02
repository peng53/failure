#
# Python module for changing polynomials to usable form
import re

#testterms = re.compile("(-?\d*\.?\d*x\d*)|(-?\d+\.?\d*)")
# strict: of A.axP form, all parameters required (except negative and .0 float)
strictterms = re.compile("-?\d+\.?\d*x\d+")
def rspol(s):
	for t in strictterms.finditer(s):
		coef,power = t.group(0).split('x')
		yield int(power),float(coef)
# because strict requires x, coef, and power, the split will always
# gives two values: the coef and power; which just needs to be changed
# in int/float and swapped before yielding. caret can be added to regex
# for more 'strictness'
term_single_power = re.compile("-?\d+\.?\d*x\d*")
def rsppol(s):
	for t in term_single_power.finditer(s):
		coef,power = t.group(0).split('x')
		yield (1 if power=='' else int(power)),float(coef)
		#yield 1,float(coef) if power=='' else int(power),float(coef)
# same as strictterms except you can omit the x's power if it is 1
term_zero_power = re.compile("-?\d+\.?\d*(x\d*)?")
def rzppol(s):
	for t in term_zero_power.finditer(s):
		N = t.group(0).split('x')
		if len(N)==2: yield (1 if N[1]=='' else int(N[1])),float(N[0])
		else: yield 0,float(N[0])
# same as term_single power except you can input constants without x**0
# uses form A.axP where x and P can be assumed (1x^0) => (1)
# peak-point cannot improve to assume 'invisible coef' without more check

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
