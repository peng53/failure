#
# Travese a weighted cell square matrix using only down or right direction with maximum possible weight.
# If equal, go right.
import random

def r_sqm(size,l,u):
	return [[random.randint(l,u) for _ in xrange(size)] for _ in xrange(size)]

def tsm(sqm):
	size = len(sqm)
	waypoints = [['']*size for _ in xrange(size)]
	values = [[0]*size for _ in xrange(size)]
	values[0][0]=sqm[0][0]
	for i in xrange(1,size):
		waypoints[0][i]='r'*i
		waypoints[i][0]='d'*i
		values[0][i] = values[0][i-1]+sqm[0][i]
		values[i][0] = values[i-1][0]+sqm[i][0]

	for y in xrange(1,size):
		for x in xrange(1,size):
			dir_d = values[y-1][x]
			dir_r = values[y][x-1]
			if dir_d>dir_r:
				values[y][x] = dir_d+sqm[y][x]
				waypoints[y][x] = waypoints[y-1][x]+'d'
			else:
				values[y][x] = dir_r+sqm[y][x]
				waypoints[y][x] = waypoints[y][x-1]+'r'
	print waypoints[size-1][size-1]

sqm = [[10,5,-9],[7,3,3],[1,2,7]]
tsm(sqm)

sqm = r_sqm(6,-20,10)
for e in sqm:
	print e
tsm(sqm)
