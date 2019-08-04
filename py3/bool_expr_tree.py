# bool_expr_tree.py

class BoolNode:
	def eval(self, data):
		''' Where data is something used by 'exp' '''
		pass

class ExprNode (BoolNode):
	def __init__(self, exp):
		'''Where exp is a lambda expression that takes 'data' '''
		self.exp = exp

	def eval(self, data):
		''' Returns whether exp evaluates true given data '''
		return self.exp(data)

c = ExprNode(lambda x: x>2)
print(c.eval(3))

d = ExprNode(lambda d: d['name'] == 'Charles')
print(d.eval({'name':'Mike','age': 24}))
