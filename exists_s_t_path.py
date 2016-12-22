#
# Check if s and t are connected in a simple graph
# Graph is in form of adjacency matrix

import Queue
import random

def exists_s_t_path_breadth(G,s,t):
	if s==t: return True
	visited = [False for _ in xrange(len(G))]
	visited[s] = True
	q = Queue.Queue()
	for neighbor,is_neighbor in enumerate(G[s]):
		if neighbor!=s and is_neighbor==1:
			q.put(neighbor)
	while not q.empty():
		n = q.get()
		if n==t: return True
		visited[n] = True
		for neighbor,is_neighbor in enumerate(G[n]):
			if neighbor!=n and is_neighbor==1 and not visited[neighbor]:
				q.put(neighbor)
	return False

def exists_s_t_path_depth(G,s,t):
	if s==t: return True
	visited = [False for _ in xrange(len(G))]
	visited[s] = True
	stk = [neighbor for neighbor,is_neighbor in enumerate(G[s]) if (neighbor!=s and is_neighbor==1)]
	while len(stk)!=0:
		n = stk.pop()
		if n==t: return True
		visited[n] = True
		for neighbor,is_neighbor in enumerate(G[n]):
			if neighbor!=n and is_neighbor==1 and not visited[neighbor]:
				stk.append(neighbor)
	return False

def random_graph(vertices_count):
	G = []
	nodes = range(vertices_count)
	for v in xrange(vertices_count):
		G.append([0]*vertices_count)
		random.shuffle(nodes)
		G[v][v] = 1
		for neighbor in nodes[:random.randint(0,vertices_count)]:
			G[v][neighbor] = 1
	return G


G = [\
	[1,1,1,0,0],\
	[1,1,0,1,0],\
	[1,0,1,0,1],\
	[0,1,0,1,0],\
	[0,0,1,0,1],\
] # graph is connected

print exists_s_t_path_breadth(G,2,3)

H = [\
	[1,1,1,0,0,0],\
	[1,1,0,1,0,0],\
	[1,0,1,0,1,0],\
	[0,1,0,1,0,0],\
	[0,0,1,0,1,0],\
	[0,0,0,0,0,1]
] # last vertex is disconnected

print exists_s_t_path_breadth(H,2,3)
print exists_s_t_path_breadth(H,2,5)

I = [\
	[1,1,1,0,0,0,0],\
	[1,1,0,1,0,0,0],\
	[1,0,1,0,1,0,0],\
	[0,1,0,1,0,0,0],\
	[0,0,1,0,1,0,0],\
	[0,0,0,0,0,1,1],\
	[0,0,0,0,0,1,1]\
] # last two vertices are in seperate component

print exists_s_t_path_breadth(I,1,5)
print exists_s_t_path_breadth(I,6,5)
print exists_s_t_path_breadth(I,6,4)

J = [\
	[1,1,0,1,0],\
	[0,1,1,0,0],\
	[0,0,1,0,0],\
	[0,0,0,1,0],\
	[1,0,0,0,1]\
] # directed graph; G[x] is list of x's neighbors

print exists_s_t_path_depth(J,0,2)
print exists_s_t_path_depth(J,2,0)
