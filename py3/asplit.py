from queue import Queue
from datetime import datetime, timedelta

class AudioSegment:
	def __init__(self, startTime=None, endTime=None, addtionalAttrs=None):
		self.optionalAttrs = {}
		self.startTime = startTime
		self.endTime = endTime
	
	def generateFilename(self, fmt, extension):
		pass

class AudioSplitter:
	def __init__(self):
		self.encodeParams = {}
	
	def splitOut(self, segment):
		print("Got segment: {}".format(segment))

class SplitScheduler:
	def __init__(self, splitter):
		self.splitter = splitter
		self.jobs = Queue()
	
	def addJob(self, job):
		print("Got job: {}".format(job))
	
	def processNext(self):
		job = self.jobs.get()
		self.splitter.splitOut(job)
	
	def hasJobs(self):
		return not self.jobs.empty()


class AudioMetaProcessor:
	def __init__(self, metafileHdl, outputCall):
		self.metadata = metafileHdl
		self.outputCall = outputCall
		self.line = True
		self.tags = {'songstart': -1, 'songend': -1}

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
			self.outputCall(self.tags)
		elif key=='songstart':
			t = timeStampToTimeDelta(val)
			self.tags[key] = t
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

splitter = AudioSplitter()
sch = SplitScheduler(splitter)
with open("../in_out/meta.txt") as f:
	amp = AudioMetaProcessor(f, sch.addJob)
	while not amp.atEOF():
		amp.processLine()
	print(amp.tags)
while sch.hasJobs():
	sch.processNext()
