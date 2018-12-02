"""
imerge.py
Was I attempting to merge lists with a fixed sized stacks?
Regardless, it doesn't work and the variable names make it
hard to read.
"""

class FixedStack:
	def __init__(self,_maxsize):
		self.maxsize = _maxsize
		self.xs = []
		self.c = 0
	def pop(self):
		if self.xs:
			return self.xs.pop()
		else:
			raise Exception
	def push(self,x):
		if len(self.xs)<self.maxsize:
			self.xs.append(x)
		else:
			raise Exception
	def top(self):
		if self.xs:
			return self.xs[len(self.xs)]
		else:
			raise Exception

class MemoryBank:
	def __init__(self,size):
		self.bal = 0
		self.size = size
		self.xs = [None]*size
		self.i = size-1
		self.j = 0
	def put(self,x):
		self.bal += 1
		self.xs[self.j] = x
		self.j += 1
		if self.j == self.size:
			self.j = 0
		print "put ", x
	def get(self):
		self.bal -= 1
		self.i += 1
		if self.i == self.size:
			self.i = 0
		print "get", self.xs[self.i]
		return self.xs[self.i]
	def peek(self):
		if self.i+1 == self.size:
			return self.xs[0]
		else:
			return self.xs[self.i+1]
"""
def merge(A,l,c,r,fstk):
	towrite = l
	l_i = l
	r_i = c+1
	stk = 0
	while towrite<r+1:
		while l_i<=c and stk<fstk.maxsize:
			fstk.push(A[l_i])
			stk += 1
			l_i += 1
		while stk>0 and r_i<=r:
			if fstk.top() <= A[r_i]:
				A[towrite] = fstk.pop()
				stk -= 1
			else:
				A[towrite] = A[r_i]
				r_i += 1
			towrite +=1
		if l_i>=c:
			print A
			return

#for x in fstk.xs:
#		print x
"""
def merge(A,l,c,r,MB):
	w = l
	li = l
	ri = c+1
	s = 0
	while w<r+1:
		if s==0:
			while li<c+1 and s<MB.size:
				MB.put(A[li])
				li += 1
				s += 1
		while s>0 and ri<=r:
			if MB.peek() <= A[ri]:
				A[w] = MB.get()
				s -= 1
			else:
				A[w] = A[ri]
				ri += 1
			print A[w], " to ", w
			w += 1
		if li>c and s==0:
			print A
			print MB.bal
			return

def merger(A,l,c,r,MB):
	w = l
	li = l
	ri = c+1
	s = 0
	while li<c+1 and s<MB.size:
		MB.put(A[li])
		li += 1
		s += 1
	while s>0 and ri<=r:
		print MB.peek(), A[ri]
		if MB.peek() <= A[ri]:
			s -= 1
			x = MB.get()
			if li<=c:
				MB.put(A[li])
				li += 1
				s += 1
			A[w] = x
			print A[w]
		else:
			A[w] = A[ri]
			print A[w]
			ri += 1
		w += 1
		#print A
	if ri>r:
		print li
		print c+1
		while s>0:
			A[w] = MB.get()
			if li<=c:
				MB.put(A[li])
				li +=1
				s+=1
			w+=1
			s-=1
	print MB.xs

	return A

def main():
	m = MemoryBank(3)
	#L = [ 1,2,3,4,5,6]
	#R = [ 1,5,7,8,10]
	L = [ 66, 77, 88, 99 ]
	R = [11,22,33,44,55]
	A = L+R
	c = len(L)-1
	r = len(A)-1
	print A
	print merger(A,0,c,r,m)
	print c,r

main()
