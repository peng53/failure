def lambdasplit(xs, conds, yieldDefault=False):
	"""
	Matches values in xs to a condition, yielding
	which condition and the value.
	If no condition matches and yieldDefault is not False,
	yield it and the non matching value.
	"""
	for x in xs:
		yielded = False
		for i,cond in enumerate(conds):
			if cond(x):
				yield(i,x)
				yielded = True
				break
		if not yielded and yieldDefault:
			yield(yieldDefault,x)

def condseperate(xs, conds, default=False):
	"""
	Seperates xs by conds, placing the values to buckets
	indexed by the condition #. If default is True, non
	matching values are sent to bucket after last condition #.
	"""
	buckets = [[] for _ in range(len(conds)+bool(default))]
	for bn, x in lambdasplit(xs, conds, len(conds) if default else False):
		buckets[bn].append(x)
	return buckets


l = range(1,16)
div3 = lambda n: n%3==0
div5 = lambda n: n%5==0
div3n5 = lambda n: div3(n) and div5(n)
# ndivn = lambda n: True


for case,x in lambdasplit(l,[div3n5, div3, div5], 10):
	if case==0:
		print("fuzz bizz {}".format(x))
	elif case==1:
		print("fuzz {}".format(x))
	elif case==2:
		print("bizz {}".format(x))
	elif case==10:
		print("{}".format(x))

k = condseperate(l, [div3n5, div3, div5], False)
print(k)

m = condseperate(
	[
		"fire2",
		"water",
		"water3",
		"holy_light"
	],
	[
		lambda s: any(c.isdigit() for c in s),
		lambda s: '_' in s,
	],
	True
)
print(m)
