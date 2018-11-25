import json

f = open('../test.json','r')

s = """
{ "hello world" : "test",
  "some_num"    :  1.2,
	"mouth" : [ 13, 22, 33 ],
	"obj" : {
		"first":1,
		"second"  : 2
	}
}
"""
s = f.read()

def ex_str_value(s,i):
	s_len = len(s)
	j = i
	while j<s_len:
		if s[j]=='"':
			break
		elif s[j]=='\\' and j+1<s_len:
			j +=1
		j += 1
	# j = next(k for k in xrange(i,s_len) if s[k]=='"')
	if j>=s_len:
		return (None,j)
	r_str = s[i:j]
	return (r_str,j+1)

def ex_num_value(s,i):
	s_len = len(s)
	j = next(k for k in xrange(i,s_len) if not s[k].isdigit())
	if s[j]=='.':
		j = next(k for k in xrange(j+1,s_len) if not s[k].isdigit())
		r_num = float(s[i:j])
	else:
		r_num = int(s[i:j])
	return (r_num,j)

def skip_until(s,i,c):
	s_len = len(s)
	while i<s_len and s[i]!=c:
		i += 1
	if i>=s_len:
		raise Exception
	return i

def next_open_close(s,i):
	try:
		j, c = next((j,c) for (j,c) in enumerate(s[i:]) if c in '{["1234567890')
	except StopIteration:
		print "No open symbols were found"
	return (j,c)

def object_handler(my_stack, stack_types, s, i):
	s_len = len(s)
	while i<s_len and my_stack:
		if s[i]=='}':
			my_stack.pop()
			stack_types.pop()
			return i+1
		elif s[i]=='"':
			### getting property name ###
			prop_name, j = ex_str_value(s,i+1)
			if prop_name:
				i = j
				j = skip_until(s,i,':')
			try:
				#j, c = next((j,c) for (j,c) in enumerate(s[i:]) if c in '{["1234567890')
				j,c = next_open_close(s,i)
			except StopIteration:
				raise Exception
			### getting object type ###
			i += j+1
			if c=='"': ### it was a string
				str_value, j = ex_str_value(s,i)
				my_stack[-1][prop_name] = str_value
				i = j+1
			elif c=='{': ### it was a dictionary/ key-value pairs
				my_stack[-1][prop_name] = {}
				my_stack.append(my_stack[-1][prop_name])
				stack_types.append('d')
				return i
			elif c=='[': ### it was an array
				my_stack[-1][prop_name] = []
				my_stack.append(my_stack[-1][prop_name])
				stack_types.append('l')
				return i
			elif c.isdigit(): ### it was a number.
				j = i-1
				num_value, j = ex_num_value(s,j)
				my_stack[-1][prop_name] = num_value
				i = j
			else:
				raise Exception
		else:
			i += 1
	return i+1

def array_handler(my_stack, stack_types, s, i):
	s_len = len(s)
	while i<s_len and my_stack:
		if s[i]==']':
			my_stack.pop()
			stack_types.pop()
			return i+1
		elif s[i]=='"': ### it was a str value
			i += 1
			str_value, j = ex_str_value(s,i)
			my_stack[-1].append(str_value)
			i = j+1
		elif s[i]=='{': ### it was a dictionary/ key-value pairs
			my_stack[-1].append({})
			my_stack.append(my_stack[-1][-1])
			stack_types.append('d')
			return i+1
		elif s[i]=='[': ### it was an array
			my_stack[-1].append([])
			my_stack.append(my_stack[-1][-1])
			stack_types.append('l')
			return i+1
		elif s[i].isdigit(): ### it was a number
			num_value, j = ex_num_value(s,i)
			my_stack[-1].append(num_value)
			i = j
		else:
			i += 1
	return i+1
tree = {}
my_stack = [tree]
stack_types = ['d']
i = 1 ### this is the index after the first { or [
#print s
while my_stack and i<len(s):
	if stack_types[-1]=='d':
		i = object_handler(my_stack,stack_types,s,i)
	elif stack_types[-1]=='l':
		i = array_handler(my_stack,stack_types,s,i)
	else:
		raise ValueError
"""
print json.dumps(json.loads(s),indent=4)
"""
print "----------"
print "With stack"
print "----------"
labels = ['tree']
objects = [tree]
indents = [0]
while labels:
	l = labels.pop()
	o = objects.pop()
	i = indents.pop()
	if l:
		print '.'*i+l+':='
	if type(o) is dict:
		labels.append(None)
		objects.append('}')
		indents.append(i-1)
		print '.'*i+'{'
		for k in o:
			labels.append(k)
			objects.append(o[k])
			indents.append(i+1)
	elif type(o) is list:
		labels.append(None)
		objects.append(']')
		indents.append(i-1)
		print '.'*i+'['
		for v in o:
			labels.append(None)
			objects.append(v)
			indents.append(i+1)
	else:
		print '%s%s' %('.'*(i+1),o)
