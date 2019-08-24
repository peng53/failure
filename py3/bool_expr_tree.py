# bool_expr_tree.py
from queue import Queue

class BoolNode:
	def eval(self, data):
		''' Where data is something used by 'exp' '''
		pass
	def teval(self, data):
		print(data)
		return self.eval(data)

	def pivot(self):
		return None

	def sym(self):
		return 0

class ExprNode (BoolNode):
	def __init__(self, exp):
		'''Where exp is a lambda expression that takes 'data' '''
		self.exp = exp

	def eval(self, data):
		''' Returns whether exp evaluates true given data '''
		return self.exp(data)

class OrNode (BoolNode):
	def __init__(self, left, right):
		''' Where left & right are the two sides of the operation '''
		self.l = left
		self.r = right

	def eval(self, data):
		''' Returns whether left OR right is true, short-circuits
		 Should use the iterative eval when completed '''
		return self.l.eval(data) or self.r.eval(data)

	def pivot(self):
		return (self.l, self.r)

	def sym(self):
		return 1

class AndNode (BoolNode):
	def __init__(self, left, right):
		''' Where left & right are the two sides of the operation '''
		self.l = left
		self.r = right

	def eval(self, data):
		''' Returns whether left AND right is true, short-circuits
		 Should use the iterative eval when completed '''
		return self.l.eval(data) and self.r.eval(data)

	def pivot(self):
		return (self.l, self.r)

	def sym(self):
		return 2

class ChainNode:
	def __init__(self, exp, deferfail = False):
		''' Where node is to be evaluated '''
		self.exp = exp
		self.leftl = None
		self.rightl = None
		self.deferfail = deferfail
		# deferfail means if this node returns false, and deferfail is true
		# return eval of rightl node

	def replace(self, a, b = None):
		''' Replace myself with a->b '''
		if not b:
			b = a
			# Case of 1 len chain
		b.deferfail = self.deferfail
		if self.rightl:
			self.rightl.leftl = b
			b.rightl = self.rightl
		# relink my right to b
		if self.leftl:
			self.leftl.rightl = a
			a.leftl = self.leftl
		# relink my left to be a

	def insert(self, other):
		''' Inserts a Node after myself '''
		self.rightl.leftl = other
		other.rightl = self.rightl
		self.rightl = other
		other.leftl = self

	def __str__(self):
		return str(self.exp)

def breathlink(node):
	''' Returns this node's left and right child as chain if possible '''
	c = node.exp.pivot()
	if not c:
		return None
	'''
	a = ChainNode(c[0],True if node.exp.sym()==1 else False)
	b = ChainNode(c[1])
	a.rightl = b
	b.leftl = a


	node.leftl.rightl = a
	a.rightl = node.leftl.rightl
	node.replace(a,b)
	return (a,b)
	'''
	# instead of replacing node, why not 'upgrade' node to the left child
	# while inserting the right child after it
	# make the right child first
	right = ChainNode(c[1])
	right.rightl = node.rightl
	right.deferfail = node.deferfail
	# begin change of node
	node.deferfail = True if node.exp.sym()==1 else False
	node.exp = c[0]
	node.rightl = right
	right.leftl = node
	return (node,right)

def tree2chain(tree):
	''' Changes a expr tree to a chain '''

	q = Queue()
	C = ChainNode(tree, False)
	q.put(C)
	while not q.empty():
		n = q.get()
		print('Got {}'.format(n))
		nc = breathlink(n)
		if nc:
			print('Put {}'.format(nc[0]))
			print('Put {}'.format(nc[1]))
			q.put(nc[0])
			q.put(nc[1])
		else:
			# if breathlink returns false, than the node is already in final state
			print('final: {}'.format(n.exp.exp))

	return C
'''
   a    B AND C
  /^\
 b   c  D OR E
/|\
d e
'''
#d = ExprNode(lambda x: x%2==0) # is divisible by 2
#e = ExprNode(lambda x: x%5==0) # is divisible by 5
#c = ExprNode(lambda x: x > 0)  # is greater than 0
d = ExprNode('lambda x: x%2==0') # is divisible by 2
e = ExprNode('lambda x: x%5==0') # is divisible by 5
c = ExprNode('lambda x: x > 0')  # is greater than 0


b = OrNode(d,e)
a = AndNode(b,c)

CHAIN = tree2chain(a)
#print(CHAIN)
n = CHAIN
#print(CHAIN.exp.exp)
while n:
	print('{} CAN FAIL: {}'.format(n.exp.exp,n.deferfail))
	#print(n)
	n = n.rightl

'''
c = ExprNode(lambda x: x>2)
print('c: {}'.format(c.teval(3)))

mike = {'name':'Mike','age': 24}
ray = {'name':'Ray','age': 57}

d = ExprNode(lambda d: d['name'] == 'Charles')
print('d: {}'.format(d.teval(mike)))
print()
e = ExprNode(lambda d: d['name'] == 'Ray')
f = OrNode(d,e)
print('f(d||e): {}'.format(f.teval(ray)))
g = AndNode(d,e)
print('g(d&&e): {}'.format(g.teval(ray)))
print()
h = OrNode(f,g)
print('h(f||g): {}'.format(h.teval(ray)))

i = OrNode(g,f)
print('i(g||f): {}'.format(i.teval(ray)))
'''
