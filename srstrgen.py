#srstrgen.py
"""
Random string generator of a certain format.

Just an idea ATM, put on hold until work on other stuff has ceased.

Not a ground breaking idea, just basic stuff using the google-heavy
''.join(a for a in string.x)

Not meant to beat any password generator.

After this is complete, I'll try for an easy GUI
Might collide with inc_test.py/rdata_gen.cpp later on
"""
from random import randint
from array import array

C = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',\
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',\
	'0','1','2','3','4','5','6','7','8','9',\
	'`','~','!','@','#','$','%','^','&','*','(',')','_','+','-','=',\
	',','.','/',';',"'",'[',']','<','>','?',':','"','{','}','|','\\',\
	' ']
C_L = [26,26,10,16,16,1]
#C_R = [(0,25),(26,51),(52,61),(62,77),(78,93),(94,94)]
C_R2= [0,26,52,62,78,94]
#r between C_R2[i] and C_R2[i]+C_L[i]

def rr(incs,length):
	if not any(incs) or len(incs)>6: raise ValueError
	o = []
	r_l = sum(l for i,l in enumerate(C_L) if incs[i])
	for _ in xrange(length):
		r = randint(0,r_l-1)
		for i,is_inc in enumerate(incs):
			if is_inc:
				if C_R2[i]<=r<=C_R2[i]+C_L[i]: o.append(C[r])
			else: r+=C_L[i]
	return ''.join(o)

def rr_a(incs,length):
	if not any(incs) or len(incs)>6: raise ValueError
	o = array('c')
	r_l = sum(l for i,l in enumerate(C_L) if incs[i])
	for _ in xrange(length):
		r = randint(0,r_l-1)
		for i,is_inc in enumerate(incs):
			if is_inc:
				if C_R2[i]<=r<=C_R2[i]+C_L[i]: o.append(C[r])
			else: r+=C_L[i]
	return o.tostring()

def rr_t(incs):
	if not any(incs) or len(incs)>6: raise ValueError
	r_l = sum(l for i,l in enumerate(C_L) if incs[i])
	p_b = []
	for r in xrange(0,(r_l-1)+1): #xrange cutoffs last int unlike randint
		for i,is_inc in enumerate(incs):
			if is_inc:
				if C_R2[i]<=r<=C_R2[i]+C_L[i]:
					p_b.append(C[r])
					break
			else: r+=C_L[i]
	print p_b
