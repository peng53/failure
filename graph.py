#
#
import random

def random_graph(vertices_count):
	G = [[False]*vertices_count for _ in xrange(vertices_count)]
	nodes = range(vertices_count)
	for v in xrange(vertices_count):
		random.shuffle(nodes)
		for neighbor in nodes[:random.randint(0,vertices_count)]:
			G[v][neighbor] = True
		G[v][v] = False
	return G

def simplize(adjm):
	t = [[v==1 for v in a] for a in adjm]
	return SimpleGraph(t)


class SimpleGraph:
	def __init__(self,adjm):
		#self.adjm = [[v==1 for v in a] for a in adjm]
		self.adjm = adjm
		self.mag = len(adjm)
		self.visited = [False]*self.mag
		for v in xrange(self.mag):
			self.adjm[v][v] = False

	def has_edge(self,v,u):
		"vu is not the same as uv"
		return not v==u and self.adjm[v][u]

	def are_neighbors(self,v,u):
		"v is a neighbor of u if vu and uv are edges"
		return not v==u and self.adjm[v][u] and self.adjm[u][v]

	def outdegree(self,v):
		"returns # of distinct vu edges"
		#"accounts for diagonal if present"
		#d = sum(self.adjm[v])
		#return d if not self.adjm[v][v] else d-1
		return sum(self.adjm[v])

	def indegree(self,v):
		"returns # of distinct uv edges"
		#d = sum((1 if u[v] else 0 for u in self.adjm))
		#d = sum((1 for u in self.adjm if u[v]))
		#d = 0
		#for u in self.adjm:
		#	if u[v]>0:
		#		d += 1
		#return d if self.adjm[v][v] else d-1
		return sum(1 for u in self.adjm if u[v])

	def remove_edge(self,u,v,both=False):
		if u==v or not self.adjm[u][v]:
			raise ValueError
		self.adjm[u][v]=False
		if both:
			self.remove_edge(v,u)

def biclique(m,n):
	adjm = [[0]*m+[1]*n for _ in xrange(m)]+[[1]*m+[0]*n for _ in xrange(n)]
	return SimpleGraph(adjm)

def kgraph(n):
	adjm = [[True]*n for _ in xrange(n)]
	return SimpleGraph(adjm)
