# dprod_view_cli.py

import os
import sqlite3

ORD = {
	'': "SELECT * FROM prod_records",
	' u': "SELECT * from prod_records ORDER by uid",
	' c': "SELECT * from prod_records ORDER by code",
	' s': "SELECT * from prod_records ORDER by start_time",
	' e': "SELECT * from prod_records ORDER by end_time",
	' d': "SELECT * from prod_records ORDER by desc",
}
EXT = {
	'u': "SELECT * from prod_records WHERE uid=?",
	'c': "SELECT * from prod_records WHERE code=?"
}
DTE = {
	'b': "SELECT * from prod_records WHERE date(end_time)<?",
	'a': "SELECT * from prod_records WHERE date(start_time)>?",
	'd': "SELECT * from prod_records WHERE date(start_time)=?"
}

def open_db():
	print "Opening DB for viewing.\nPlease enter file name.\n"
	s = raw_input("File name: ")
	if len(s)>0:
		if os.path.isfile(s):
			conn = sqlite3.connect(s)
			c = conn.cursor()
			cols = [col[1:3] for col in c.execute('pragma table_info(prod_records)')]
			if cols==[('uid','text'),('start_time','datetime'),('end_time','datetime'),('code','text'),('desc','text')]:
				print "'%s' loaded for viewing." %(s)
				return conn
			else:
				print "Database schema does not match intended."
				print "Closing file."
				conn.close()
	return None
def close_db(conn):
	conn.close()
def print_rows(rows,s,c):
	print '_'*82
	print "RN  /  UID   /   Start Time     /    End Time      / Code  /  Desc"
	print '-'*82
	for n,i in enumerate(xrange(s,min(c+s,len(rows)))):
		print "%2d | %6s | %16s | %16s | %5s | %-22s" %(n,rows[i][0],rows[i][1],rows[i][2],rows[i][3],rows[i][4])
	for _ in xrange(len(rows),s+c):
		print "   | %6s | %16s | %16s | %5s |" %('','','','')
	print '='*59, "%21s %d" %("Page",s//c)
def draw_screen(results,r_s,r_c,notes,n_s,n_c):
	print "Productivity Viewer"
	print "Results table from lookup"
	print_rows(results,r_s,r_c)
	print "Noted records"
	print_rows(notes,n_s,n_c)
	print "Command Bar"
	print "(L)oad (Q)uit (l)ookup (a)dd-to-notes (d)elete-from-notes"
	print "(S)ort (C)lear (1)st-page (n)ext-page (p)rev-page (s)ummarize"
def dlookup_d():
	print "Record lookup choices"
	print "()all"
	print "Exact / Limited"
	print "(u)id (c)ode (b)efore (d)ate (a)fter"
	print "By Order (prepend with space)"
	print "( u)id ( c)ode ( s)tart ( e)nd ( d)esc"
	s = raw_input("::")
	if len(s)==1:
		if s in DTE:
			print "Enter requirement." # for now just the easy ones, later allow prepended operator
			t = raw_input("::::")
			if len(t)==0:
				print "Requirement is required."
			elif '/' in t:
				print 1
				d = map(int,t.split('/',2))
				if len(d)==3:
					return (DTE[s],("%04d-%02d-%02d" %(d[2],d[0],d[1]),))
			elif '-' in t:
				print 2
				d = map(int,t.split('-',2))
				if len(d)==3:
					return (DTE[s],("%04d-%02d-%02d" %(d[0],d[1],d[2]),))
			elif len(t)==8:
				print 3
				return (DTE[s],('-'.join((t[4:8],t[0:2],t[2:4])),))
			else:
				print "Unrecognized date format."
		elif s in EXT:
			print "Enter requirement."
			t = raw_input("::::")
			if len(t)>0:
				return [EXT[s],(t,)]
	elif len(s)==0 or len(s)==2:
		if s in ORD:
			return [ORD[s]]
		else:
			print "Unknown option entered. 202"
	else:
		print "Unknown option entered. 101"
	return None
def drowchoose(length,offset,count):
	count = min(count,length)
	print "Row selection from 0 to %d." %(count-1)
	print "Seperate digits with '-' to indicate a range."
	print "Seperate digits with ',' to indicate specific rows."
	s = raw_input("::")
	try:
		if '-' in s:
			s = map(int,s.split('-',2))
			if 0<=s[0]<s[1]<=count:
				return xrange(s[1]+offset,s[0]+offset-1,-1)
			else:
				return "Input range is invalid."
		elif ',' in s:
			s = sorted(map(int,s.split(',')),reverse=True)
		else:
			s = [int(s)]
	except Exception as e:
		print e
		print "Input has invalid characters."
		return None
	if all(map(lambda n: 0<=n<count,s)):
		return [n+offset for n in s]
	else:
		print "Input is out of stated bounds."
	return None


def main():
	R,R_S = [],0
	N,N_S = [],0
	conn = c = None
	ic = 10
	draw_screen(R,R_S,ic,N,N_S,ic)
	delta = False
	while True:
		if delta:
			draw_screen(R,R_S,ic,N,N_S,ic)
			delta = 0
		s = raw_input(":")
		if len(s)==0:
			continue
		if s=='L':
			conn = open_db()
			if conn:
				c = conn.cursor()
		elif s=='Q':
			if conn:
				conn.close()
			return
		elif s=='l':
			if not conn:
				print "Please load a file first."
				continue
			q = dlookup_d()
			#print q
			#print len(q)
			if q:
				if len(q)==1:
					R = list(c.execute(q[0]))
				else:
					R = list(c.execute(q[0],q[1]))
				delta = 1
		elif s=='Cr':
			R,R_S = [], 0
			delta = 1
		elif s=='Cn':
			N,N_S = [], 0
			delta = 1
		elif s=='d':
			q = drowchoose(len(N),N_S,ic)
			if q:
				for n in q:
					N.pop(n)
				delta = 1
		elif s=='a':
			q = drowchoose(len(R),R_S,ic)
			if q:
				for n in q:
					N.append(R[n])
				delta = 1
		elif s=='nn':
			if N_S+ic<len(N):
				N_S+=ic
				delta = 1
		elif s=='nr':
			if R_S+ic<len(R):
				R_S+=ic
				delta=1
		elif s=='pn':
			if N_S!=0:
				N_S-=ic
				delta = 1
		elif s=='pr':
			if R_S!=0:
				R_S-=ic
				delta = 1
		elif s=='1n':
			N_S = 0
			delta = 1
		elif s=='1r':
			R_S = 0
			delta = 1

main()
