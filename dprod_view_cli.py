# dprod_view_cli.py

import os
import sqlite3
import time

SELS = {
	'': "SELECT * FROM prod_records",
	' u': "SELECT * from prod_records ORDER by uid",
	' c': "SELECT * from prod_records ORDER by code",
	' s': "SELECT * from prod_records ORDER by start_time",
	' e': "SELECT * from prod_records ORDER by end_time",
	' d': "SELECT * from prod_records ORDER by desc",
	'u': "SELECT * from prod_records WHERE uid=?",
	'c': "SELECT * from prod_records WHERE code=?",
	'b': "SELECT * from prod_records WHERE date(end_time)<?",
	'a': "SELECT * from prod_records WHERE date(start_time)>?",
	'd': "SELECT * from prod_records WHERE date(start_time)=?"
}
ORD = ('',' u',' c',' s',' e',' d')
EXT = ('u','c')
DTE = ('b','a','d')

def sqlsel(cur,t,args=None):
	if t in SELS:
		Q = cur.execute(SELS[t],args)
		for r in Q:
			yield r
def sqlmat(cur):
	cols = [col[1:3] for col in cur.execute('pragma table_info(prod_records)')]
	return cols==[('uid','text'),('start_time','datetime'),('end_time','datetime'),('code','text'),('desc','text')]
def sqlemp(cur,uids):
	for u in uids:
		cur.execute("SELECT * from employees WHERE uid=?",(u,))
		yield cur.fetchone()

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
					return (s,("%04d-%02d-%02d" %(d[2],d[0],d[1]),))
			elif '-' in t:
				print 2
				d = map(int,t.split('-',2))
				if len(d)==3:
					return (s,("%04d-%02d-%02d" %(d[0],d[1],d[2]),))
			elif len(t)==8:
				print 3
				return (s,('-'.join((t[4:8],t[0:2],t[2:4])),))
			else:
				print "Unrecognized date format."
		elif s in EXT:
			print "Enter requirement."
			t = raw_input("::::")
			if len(t)>0:
				return [s,(t,)]
	elif len(s)==0 or len(s)==2:
		if s in ORD:
			return [s,()]
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

QFTR = {
	'<' : lambda v,x: v<x,
	'>' : lambda v,x: v>x,
	'=' : lambda v,x: v==x,
	'!' : lambda v,x: v!=x,
}
class Viewer:
	def __init__(self):
		self.R, self.N = [], []
		self.R_S = self.N_S = 0
		self.conn = self.c = None
		self.ic = 10
		self.delta = 1
		self.b = {
			'L' : self.open_db,
			'Q' : self.quit_p,
			'l' : self.clookup,
			'a' : self.caddnote,
			'd' : self.cdeleterow,
			'Cr': lambda: self.cclear(True),
			'Cn': lambda: self.cclear(False),
			'1r': lambda: self.cfirst(True),
			'1n': lambda: self.cfirst(False),
			'pr': lambda: self.cprev(True),
			'pn': lambda: self.cprev(False),
			'nr': lambda: self.cnext(True),
			'nn': lambda: self.cnext(False),
			'Sr': lambda: self.csort(True),
			'Sn': lambda: self.csort(False),
			'sr': lambda: self.ctotal(True),
			'sn': lambda: self.ctotal(False),
			'er': lambda: self.cemp(True),
			'en': lambda: self.cemp(False),
			'fr': lambda: self.cfil(True),
			'fn': lambda: self.cfil(False),
		}
	def print_rows(self,R=True):
		rows,s = (self.R,self.R_S) if R else (self.N,self.N_S)
		print '_'*82
		print "RN  /  UID   /   Start Time     /    End Time      / Code  /  Desc"
		print '-'*82
		for n,i in enumerate(xrange(s,min(self.ic+s,len(rows)))):
			print "%2d | %6s | %16s | %16s | %5s | %-22s" %(n,rows[i][0],rows[i][1],rows[i][2],rows[i][3],rows[i][4])
		for _ in xrange(len(rows),s+self.ic):
			print "   | %6s | %16s | %16s | %5s |" %('','','','')
		print '='*59, "%21s %d" %("Page",s//self.ic)
	def draw_screen(self):
		print "Productivity Viewer"
		print "Results table from lookup"
		self.print_rows(True)
		print "Noted records"
		self.print_rows(False)
		print "Command Bar"
		print "(L)oad (Q)uit (l)ookup (a)dd-to-notes (d)elete-from-notes"
		print "(S)ort (C)lear (1)st-page (n)ext-page (p)rev-page (s)ummarize"
		print "(e)mployee (f)ilter"
	def open_db(self):
		print "Opening DB for viewing.\nPlease enter file name.\n"
		s = raw_input("File name: ")
		if len(s)>0:
			if os.path.isfile(s):
				conn = sqlite3.connect(s)
				c = conn.cursor()
				if sqlmat(c):
					print "'%s' loaded for viewing." %(s)
					self.conn = conn
					self.c = self.conn.cursor()
				else:
					print "Database schema does not match intended."
					print "Closing file."
					conn.close()
	def close_db(self):
		if self.conn:
			self.conn.close()
	def quit_p(self):
		self.close_db()
		self.delta = -1
	def clookup(self):
		if not self.conn:
			print "Please load a file first."
			return
		q = dlookup_d()
		if q:
			self.R = list(sqlsel(self.c,*q))
			self.delta = 1
	def cdeleterow(self):
		if not self.R:
			print "Please load a file first."
			return
		q = drowchoose(len(self.N),self.N_S,self.ic)
		if q:
			for n in q:
				self.N.pop(n)
			self.delta = 1
	def caddnote(self):
		if not self.R:
			print "Please use 'lookup' first."
			return
		q = drowchoose(len(self.R),self.R_S,self.ic)
		if q:
			for n in q:
				self.N.append(self.R[n])
			self.delta = 1
	def cprev(self,R=True):
		if R:
			if self.R_S != 0:
				self.R_S -= self.ic
				self.delta = 1
		else:
			if self.N_S != 0:
				self.N_S -= self.ic
				self.delta = 1
	def cnext(self,R=True):
		if R:
			if self.R_S<len(self.R):
				self.R_S += self.ic
				self.delta = 1
		else:
			if self.N_S<len(self.N):
				self.N_S += self.ic
				self.delta = 1
	def cfirst(self,R=True):
		if R:
			self.R_S = 0
			self.delta = 1
		else:
			self.N_S = 0
			self.delta = 1
	def cclear(self,R=True):
		if R:
			self.R, self.R_S = [], 0
		else:
			self.N, self.N_S = [], 0
		self.delta = 1
	def csort(self,R=True):
		print "Table sorting."
		print "Choose column to sort by."
		print "Append r for reversed."
		print "(u)id (c)ode (s)tart-time (e)nd-time (d)esc"
		s = raw_input("::")
		if len(s)>0:
			c = {'u':0,'c':3,'s':1,'e':2,'d':4}
			if s[0] not in c:
				print "Unknown column."
			else:
				r = len(s)==2 and s[1]=='r'
				s = c[s[0]]
				if R:
					self.R.sort(key=lambda v:v[s],reverse=r)
				else:
					self.N.sort(key=lambda v:v[s],reverse=r)
				self.delta = 1
	def ctotal(self,R=True):
		rows = self.R if R else self.N
		# by uid & total in notes
		t = {}
		for row in rows:
			if row[0] not in t:
				t[row[0]] = {}
			if row[3] not in t[row[0]]:
				t[row[0]][row[3]] = 0
			t0 = time.mktime(time.strptime(row[1], '%Y-%m-%d %H:%M'))
			t1 = time.mktime(time.strptime(row[2], '%Y-%m-%d %H:%M'))
			t[row[0]][row[3]] += t1-t0
		print 'Total time of notes by UID and Code'
		for u in t:
			print u
			s = 0
			for c in t[u]:
				print '%10s : %4d mins' %(c,t[u][c]//60)
				s += t[u][c]
			print 'Total %5d mins' %(s//60)
	def cemp(self,R=True):
		rows,offset = (self.R,self.R_S) if R else (self.N,self.N_S)
		q = drowchoose(len(rows),offset,self.ic)
		if q:
			q = set([rows[n][0] for n in q])
			print '_'*65
			print "  UID  |   First Name   |    Last Name   | E-Mail Address "
			print '-'*65
			for r in sqlemp(self.c,q):
				print "%6s | %14s | %14s | %22s" %(r[0],r[1],r[2],r[3])
			print '='*65
	def cfil(self,R=True):
		print "Filter by which column?"
		print "(u)id (c)ode (s)tart-time (e)nd-time (d)esc"
		s = raw_input("::")
		if len(s)>0:
			c = {'u':0,'c':3,'s':1,'e':2,'d':4}
			if s in c:
				print "Enter requirement."
				print "Prepend with: <>=!"
				q = raw_input("::::")
				if len(q)>0:
					if q[0] in QFTR:
						rows = self.R if R else self.N
						l = QFTR[q[0]]
						s = c[s]
						r = q[1:]
						length = len(rows)
						for i in xrange(length-1,-1,-1):
							if not l(rows[i][s],r):
								rows.pop(i)
						self.delta = 1
					else:
						print "Missing operator."
			else:
				print "Unknown Column."

	def mainloop(self):
		while self.delta!=-1:
			if self.delta:
				self.draw_screen()
				self.delta = 0
			s = raw_input(":")
			if len(s)==0:
				self.delta = 1
			elif s in self.b:
				self.b[s]()
			else:
				print "Unknown option used."

def main():
	v = Viewer()
	v.mainloop()

main()
