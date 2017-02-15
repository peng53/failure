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
from random import randint, choice
from array import array
import string

#Character includes are the in following order:
C=("abcdefghijklmnopqrstuvwxyz" #lowercase alpha
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ" #uppercase alpha
	"0123456789" #digits
	"`~!@#$%^&*()_+-=" #number row symbols
	",./;'[]<>?:\"{}|\\" #other symbols
	" ") #space
# C_L are include lengths
C_L = [26,26,10,16,16,1]
# C_R2 are include start points (which technically can be calculated..)
C_R2= [0,26,52,62,78,94]
#r between C_R2[i] and C_R2[i]+C_L[i]
# C_tr and C_os are number row symbols and other symbols in the same order as above includes
C_tr = r'`~!@#$%^&*()_+-='
C_os = r',./;\'[]<>?:"{}|\\'

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

def rr_p(incs,length):
	if not any(incs) or len(incs)>6: raise ValueError
	c = ''.join(s for i,s in enumerate([string.lowercase,string.uppercase,string.digits,C_tr,C_os,' ']) if incs[i])
	return ''.join(choice(c) for _ in xrange(length))

def rr_test(incs):
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
