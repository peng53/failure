# dprod_build.py
from sys import argv
import os
import sqlite3
import time

def main(args):
	pass
	s = 'test.db'
	if len(args)==1:
		print "Using test.db as name."
	else:
		s = args[1]
	if os.path.exists(s):
		print "File already exists.\n Aborting."
		return 1
	print "Writing test database to", s
	conn = sqlite3.connect(s)
	c = conn.cursor()
	#c.execute('CREATE TABLE prod_records(uid text,start_time integer,end_time integer,code text,desc text)')
	c.execute('CREATE TABLE prod_records(uid text,start_time datetime,end_time datetime,code text,desc text)')
	t1='2022-01-04 03:27'
	t2='2022-01-04 03:33'
	t3='2022-01-04 03:46'
	t4='2022-01-04 05:02'
	#t1=time.mktime(time.struct_time([2001,1,4,3,27,0,0,0,-1]))
	#t2=time.mktime(time.struct_time([2001,1,4,3,33,0,0,0,-1]))
	#t3=time.mktime(time.struct_time([2001,1,4,3,46,0,0,0,-1]))
	#t4=time.mktime(time.struct_time([2001,1,4,5,2,0,0,0,-1]))
	c.execute('INSERT into prod_records VALUES("s203",?,?,"112","work")',[t1,t2])
	c.execute('INSERT into prod_records VALUES("s203",?,?,"222","break")',[t2,t3])
	c.execute('INSERT into prod_records VALUES("s203",?,?,"112","work")',[t3,t4])
	c.execute('INSERT into prod_records VALUES("b777",?,?,"112","basic desc")',[t1,t3])
	c.execute('INSERT into prod_records VALUES("b777",?,?,"908","lunch")',[t3,t4])
	c.execute('INSERT into prod_records VALUES("s203",?,?,"112","work")',[t3,t4])
	#t1=time.mktime(time.struct_time([2000,12,29,3,27,0,0,0,-1]))
	#t2=time.mktime(time.struct_time([2000,12,29,3,33,0,0,0,-1]))
	#t3=time.mktime(time.struct_time([2000,12,29,3,46,0,0,0,-1]))
	t1='2023-12-29 03:27'
	t2='2023-12-29 03:33'
	t3='2023-12-29 03:46'
	c.execute('INSERT into prod_records VALUES("t421",?,?,"222","break")',[t1,t2])
	c.execute('INSERT into prod_records VALUES("t421",?,?,"908","lunch")',[t2,t3])
	conn.commit()
	conn.close()

main(argv)
