from queue import Queue
from datetime import datetime, timedelta
import re

class StringFormater:
	def __init__(self, kwDict, ifNull=''):
		self.kwDict = kwDict
		self.ifNull = ifNull
	
	pat = re.compile(r'(\%.)|([^\%]+)')
	
	def format(self, fmt, keyValues):
		parts = (fmt[m.start():m.end()] for m in self.pat.finditer(fmt))
		sub = (
			(keyValues[self.kwDict[p[1]]] if self.kwDict[p[1]] in keyValues else self.ifNull) 
			if p[0]=='%' else p for p in parts
		)
		#nonNullSub = (s for s in sub if len(s)>0)
		return ''.join(sub)

class AudioSegment:
	def __init__(self, title, startTime, endTime, additionalAttrs=None):
		self.optionalAttrs = additionalAttrs
		self.startTime = startTime
		self.endTime = endTime
		self.title = title
	
	def __repr__(self):
		r = []
		if 'album' in self.optionalAttrs:
			r.append(self.optionalAttrs['album'])
		if 'artist' in self.optionalAttrs:
			r.append(self.optionalAttrs['artist'])
		r.append(self.title)
		r.append(str(self.startTime))
		r.append(str(self.endTime))
		return ' - '.join(r)
	
	kwList = {
		'l' : 'album',
		'a' : 'artist',
		's' : 'title',
		'y' : 'year',
		'g' : 'genre',
		'n' : 'track'
	}
	
	def generateFilename(self, fmt, extension, fmter, replaceSpaces=None):
		# l : album
		# a : artist
		# s : songname
		# y : year
		# g : genre
		# n : track number
		r = fmter.format(fmt, self.optionalAttrs)+'.'+extension
		if replaceSpaces:
			return r.replace(' ',replaceSpaces)
		else:
			return r

class AudioSplitter:
	fmter = StringFormater(AudioSegment.kwList)
	def __init__(self, fileToBeSplit):
		self.encodeParams = {}
		self.fileToBeSplit = fileToBeSplit
		self.outputFilesFmt = None
	
	def splitOut(self, segment):
		if self.outputFilesFmt:
			out = segment.generateFilename('%l-%a-%s', self.outputFilesFmt, AudioSplitter.fmter,replaceSpaces='_')
			print("Generated filename: {}".format(out))
			print("Spliting job: {}".format(segment))
			print("BIN {0} -o {1}".format(self.fileToBeSplit, out))
			return out
		else:
			print("No output format has been selected!")
			return None

	def splitTo(self, fileFmt):
		self.outputFilesFmt = fileFmt

class SplitScheduler:
	def __init__(self, splitter, tagger):
		self.splitter = splitter
		self.tagger = tagger
		self.jobs = Queue()
	
	def addJob(self, job):
		# has to check if 'job' contains needed keys
		filterAttr = { k : job[k] for k in ['album','artist','title'] if k in job}
		self.jobs.put(
			AudioSegment(
				job['title'],
				job['songstart'],
				job['songend'],
				filterAttr
			)
		)
		print("Added job: #{}".format(self.jobs.qsize()))
	
	def processNext(self):
		job = self.jobs.get()
		print("Processing job: {}".format(job))
		out = self.splitter.splitOut(job)
		if not out:
			print("Split task has not produced a file")
			return
		# tag
		print("Now tagging {}".format(out))
		tagger.tag(out, job)
	
	def hasJobs(self):
		return not self.jobs.empty()


class AudioMetaProcessor:
	def __init__(self, metafileHdl, outputCall):
		self.metadata = metafileHdl
		self.outputCall = outputCall
		self.line = True
		self.tags = {}

	def processLine(self):
		self.line = self.metadata.readline()
		if self.line=='' or self.line is None:
			return
		self.line = self.line.strip()
		if len(self.line)==0:
			return
		if (self.line[0]=='#'):
			print(self.line)
		key, val = self.line.split('=', maxsplit=1)
		key = key.lower()
		if key=='songend':
			t = timeStampToTimeDelta(val)
			self.tags[key] = t
			if 'title' in self.tags:
				self.outputCall(self.tags)
		elif key=='songstart':
			t = timeStampToTimeDelta(val)
			self.tags[key] = t
		elif key=='songstartend':
			ts, te = val.split('-', maxsplit=1)
			self.tags['songstart'] = timeStampToTimeDelta(ts)
			self.tags['songend'] = timeStampToTimeDelta(te)
			self.outputCall(self.tags)
		else:
			self.tags[key] = val

	def atEOF(self):
		return not self.line


def timeStrToRelativeTime(s):
	t = datetime.strptime(s, "%H:%M:%S")
	return timedelta(hours=t.hour, minutes=t.minute, seconds=t.second)

class UndefinedTimeUnit(Exception):
	pass

def timeStampToTimeDelta(s):
	units = s.split(':')
	if len(units) > 3:
		raise UndefinedTimeUnit
	units.reverse()
	# H:M:S to S:M:H
	return timedeltaFromTimeUnits(*units)

def timedeltaFromTimeUnits(seconds, minutes=0, hours=0):
	# seconds and minutes must be <60.
	return timedelta(hours=int(hours), minutes=int(minutes), seconds=int(seconds))

class AudioTagger:
	def tag(self, AudioSegment):
		pass

import mp3_tagger
import os.path

class MP3Tagger(AudioTagger):
	# will use mp3_tagger to do it
	possibleTags = [
		'artist',
		'album',
		'track',
		'genre',
		'year'
	]
	def tag(self, filename, seg):
		if not os.path.isfile(filename):
			print("{} doesn't exist to tag".format(filename))
			return
		m = mp3_tagger.MP3File(filename)
		m.set_version(mp3_tagger.VERSION_BOTH)
		m.song = seg.title
		for tag in possibleTags:
			if 'tag' in m.optionalAttrs:
				setattr(m, tag, m.optionalAttrs[tag])
		m.save()

splitter = AudioSplitter('my_audio_file.mp3')
splitter.splitTo('mp3')
tagger = MP3Tagger()
sch = SplitScheduler(splitter, tagger)
with open("../in_out/meta.txt") as f:
	amp = AudioMetaProcessor(f, sch.addJob)
	while not amp.atEOF():
		amp.processLine()
while sch.hasJobs():
	sch.processNext()
