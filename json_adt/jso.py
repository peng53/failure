
s = """
{ "hello world" : "test",
	"mouth" : [ 1, 2, 3 ],
	"obj" : {
		"first": 1,
		"second": 2
	}
}
"""

class Jso:
	def __init__(self):
		self.special = None
		self.data = None
	def set(self,v):
		self.data = v
	def add_key_value(self,k,v):
		if not self.special:
			self.special = 1
			self.data = {}
		if self.special==1:
			self.data[k] = v
	def add_value(self,v):
		if not self.special:
			self.special = 2
			self.data = []
		if self.special==2:
			self.data.append(v)
	def get_value(self,k):
		try:
			return self.data[k]
		except:
			return None
	def __str__(self):
		s = []
		if self.special==1:
			s.append('{')
			for k in self.data:
				s.append(k)
				s.append(':')
				s.append(repr(self.data[k]))
			s.append('}')
		elif self.special==2:
			s.append('[')
			for v in self.data:
				s.append(v)
				s.append(',')
			s.append(']')
		return ' '.join(s)
p = Jso()
p.add_key_value("horse","butter")
p.add_key_value("obj",Jso())
j = p.get_value("obj")
j.add_key_value("mice",24)
p.add_key_value("arr",Jso())
j = p.get_value("arr")
print p
r = {}
r['horse'] = 'butter'
r['obj'] = {}
j = r['obj']
j['mice'] = 24
r['arr'] = []
print r

def object_handler(my_stack, stack_types, s, i):
	s_len = len(s)
	while my_stack:
		while i<s_len:
			if s[i]=='}':
				if len(my_stack)==1:
					return (my_stack.pop(),i+1)
				else:
					my_stack.pop()
					stack_types.pop()
					return (None,i+1)
			elif s[i]=='"':
				### getting property name ###
				j = i+1
				while j<s_len and s[j]!='"':
					j += 1
				if j>=s_len:
					raise Exception
				## Success ###
				else:
					prop_name = s[i:j]
					i = j+1
					while i<s_len and s[i]!=':':
						i += 1
					if i>=s_len:
						raise Exception
				try:
					j, c = next((j,c) for (j,c) in enumerate(s[i:]) if c in '{["1234567890')
				except StopIteration:
					raise Exception
				### getting object type ###
				i += j+1
				if c=='"': ### it was a string
					j = i
					while j<s_len and s[j]!='"':
						j += 1
					my_stack[-1][prop_name] = s[i:j]
					i = j+1
				elif c=='{': ### it was a dictionary/ key-value pairs
					my_stack[-1][prop_name] = {}
					my_stack.append(my_stack[-1][prop_name])
					stack_types.append('d')
					return (None,i)
				elif c=='[': ### it was an array
					my_stack[-1][prop_name] = []
					my_stack.append(my_stack[-1][prop_name])
					stack_types.append('l')
					return (None,i)
				elif c.isdigit(): ### it was a number.
					j = i
					while j<s_len and (s[j].isdigit()):
						j += 1
					if (j>=s_len):
						raise Exception
					if s[j]=='.':
						j += 1
						while j<s_len and (s[j].isdigit()):
							j += 1						
					my_stack[-1][prop_name] = float(s[i-1:j-1])
				else:
					raise Exception
			else:
				i += 1
my_stack = [{}]
stack_types = ['d']
print object_handler(my_stack,stack_types,s,1)
print my_stack
#print j.data
#print p.get_value("obj")
