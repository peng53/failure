dbFile = 'filetags.db'

import sqlite3
import os

def createDefaultTables(cursor):
	cur.execute('create table files (fid integer primary key, filename text, lastAccess integer)')
	cur.execute('create table tags (tid integer primary key, tag text, lastAccess integer)')
	cur.execute('create table filetags (fid integer, tid integer)')

def createDBFile(filename):
	db = sqlite3.connect(filename)
	cur = db.cursor()
	createDefaultTables(cur)

def addFile(cursor, filename):
	cursor.execute('insert into files (filename, lastAccess) values (?,date())', (filename))

def addTag(cursor, tag):
	cursor.execute('insert into tags (tag, lastAccess) values (?, date())', (tag))

def setTags(cursor, filename, *tagIds):
	cursor.execute('select fid from files where filename=? limit 1', filename)
	result = cursor.fetchone()
	
	cursor.execute('begin transaction')
	for tagId in tagIds:
	
	cursor.execute('commit')

con = None

if os.path.isfile(dbFile):
	con = sqlite3.connect(dbFile)
else:
	con = createDBFile(dbFile)

