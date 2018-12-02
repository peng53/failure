#
# Seems to be a version of shunting yards in python
#


import re
valid_tok = re.compile(r"(\d+(\.\d+)?)|([()+*/^x-])")
def fixate(s):
	# Splits terms & operators in a list
	o = []
	for t in valid_tok.finditer(s):
		o.append(t.group(0))
	return o

def ob(T):
	# Takes list of terms & numbers and returns a
	# RPN binary tree?
	p = {'+':2,'-':2,'*':3,'/':3,'^':4}
	sstk, out = [], []
	c = None
	for t in T:
		if t=='x' or t.lstrip('-').isdigit(): out.append('x' if t=='x' else float(t))
		elif t in p:
			if len(sstk)==0 or sstk[-1]=='(' or t==sstk[-1]=='^' or p[t]>p[sstk[-1]]:
				sstk.append(t)
			else:
				out.append(sstk.pop())
				sstk.append(t)
		elif t=='(': sstk.append('(')
		elif t==')':
			while sstk[-1]!='(':
				out.append(sstk.pop())
			sstk.pop()
	while len(sstk)!=0:
		c = sstk.pop()
		if c!='(': out.append(c)
	return out

s = " 10 + (2/1)  + 7*  3"
f = fixate(s)
g = ob(f)

print s
print f
print g
