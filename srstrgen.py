#srstrgen.py
"""
Random string generator of a certain format.

Just an idea ATM, put on hold until work on other stuff has ceased.

Not a ground breaking idea, just basic stuff using the google-heavy
''.join(a for a in string.x)

Not meant to beat any password generator.

After this is complete, I'll try for an easy GUI
Might collide with inc_test.py/rdata_gen.cpp later on

A default string 'C' contains includes for lower and upper case letters;
digits; number row and other symbols; and spaces.
C_L defines include lengths
C_R2 defines start points for the C string
C_tr and C_os defines number row symbols and other symbols respectively.
b_inc is an object that state which groups of 'C' is included (see 'C' for order)

inc_exc function take characters (defined as an iterable or bitset list) and characters from an
 exclude list and return an iterable of the included minus the excluded. Ideally they should
 allow for excludes that aren't even included. Basically returning set(incs)-set(exc).
"""
from collections import namedtuple
from random import randint, choice
import string

C=("abcdefghijklmnopqrstuvwxyz"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"0123456789"
	"!#$%&()*+-=@^_`~"
	"\"',./:;<>?[\\]{|}"
	" ")
C_L = [26,26,10,16,16,1]
C_R2= [0,26,52,62,78,94]
C_tr = '!#$%&()*+-=@^_`~'
C_os = '"\',./:;<>?[\\]{|}'

b_inc = namedtuple('included','lower upper digits symbols1 symbols2 space')
def b_inc_repr(incs):
	return ''.join(s for i,s in enumerate(['az','AZ','09','!@','<>','sP']) if incs[i])

def include_chars(incs):
	if len(incs)==0: raise ValueError
	if not isinstance(incs,b_inc): return incs
	if not any(incs): raise ValueError
	return ''.join(s for i,s in enumerate([string.lowercase,string.uppercase,string.digits,C_tr,C_os,' ']) if incs[i])

#inc_exc_s = lambda I,E=[]:tuple(set(include_chars(I))-set(E))

def inc_exc_s(I,E=[],D={}):
	if (I,E) not in D: D[(I,E)] = tuple(set(include_chars(I))-set(E))
	return D[(I,E)]
def inc_exc(incs,exc):
	C = sorted(include_chars(incs))
	o = []
	for e in sorted(exc,reverse=True):
		while len(C)!=0:
			if C[-1]<e: break #e isn't included to begin with
			c = C.pop()
			if c==e: break
			o.append(c)
	return o+C

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

def rPhase(C,L,W=1,DM=' '):
	"""Given iterable of allowed symbols, return a string with W words of length L
	delimited by DM. L only denotes the number of symbols in each word and not the
	actual word length. Default arguments assume a single word.
	Use: want: 3 groups of 4 digits seperated by space
	C='0123456789'; L=4; W=3; DM=' '
	C can be generated with inc_exc_s(b_inc(0,0,1,0,0,0))
	.. or using string.digits in this case
	"""
	return DM.join(''.join(choice(C) for _ in xrange(L)) for _w in xrange(W))

def moonPhase(todo,count=1):
	S = [''.join(s if isinstance(s,str) else rPhase(inc_exc_s(s[0],s[1]),*s[2:]) for s in todo) for _ in xrange(count)]
	return S

if __name__=="__main__":
	print "Want 2 groups of 3 lower case letters seperated by - ending with - and 4 lower alphanumeric"
	C1 = string.lowercase
	C2 = inc_exc_s(b_inc(1,0,1,0,0,0),'')
	print "C1 = %s\nC2 = %s" %(C1,''.join(C2))
	s = rPhase(C1,3,2,'-')
	s += '-'+rPhase(C2,4)
	print "s = %s" %(s)
	print "Want: 3 strings of format\n\t3 groups of 4 chars from 'abc' seperated by -;"
	print "\t'-cookie-';"
	print "\t1 group of 10 alphanumerics;"
	print "\t'-';"
	print "\tand 1 group of 4 chars from 'abc';"
	s = moonPhase([("abc","",4,3,'-'),'-cookie-',(b_inc(1,1,1,0,0,0),"345abc",10),'-',('abc','',3,1)],3)
	print "s = %s" %(s)
