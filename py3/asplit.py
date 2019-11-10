from queue import Queue
from datetime import datetime, timedelta
import re
import ffmpeg

class TargetNotFoundException(Exception):
	pass

class SplitOperationException(Exception):
	pass

class CannotWriteException(Exception):
	def __init__(self, message, errors):
		super().__init__(message)
		self.errors = errors

class FileAlreadyExistsException(Exception):
	def __init__(self, message, errors):
		super().__init__(message)
		self.errors = errors

class UndefinedTimeUnit(Exception):
	pass


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
	
	def generateFilename(self, fmt, fmter, replaceSpaces=None):
		# l : album
		# a : artist
		# s : songname
		# y : year
		# g : genre
		# n : track number
		r = fmter.format(fmt, self.optionalAttrs)
		if replaceSpaces:
			return r.replace(' ',replaceSpaces)
		else:
			return r


class AudioSplitter:
	supportedEncodeParams = ['acodec', 'audio_bitrate', 'aq']
	def __init__(self, fileToBeSplit):
		self.encodeParams = {}
		self.fileToBeSplit = fileToBeSplit
		self.outputFilesFmt = None
	
	def splitOut(self, outputFile, start, duration):
		if not self.outputFilesFmt:
			raise SplitOperationException('Split has failed to execute because no output format was selected.')

		infile = ffmpeg.input(self.fileToBeSplit)
		encode = ffmpeg.output(
			infile,
			outputFile,
			acodec=self.encodeParams['acodec'] if 'acodec' in self.encodeParams else 'copy',
			ss=start,
			t=duration,
			map_metadata=-1,
		)
		r = encode.run(quiet=True)
		return r

	def splitTo(self, fileFmt):
		self.outputFilesFmt = fileFmt
	
	def setEncode(self, param, value):
		if param in AudioSplitter.supportedEncodeParams:
			self.encodeParams[param] = value


class SplitScheduler:
	def __init__(self, splitter, tagger, fmter, afterSplit = None):
		self.splitter = splitter
		self.jobs = Queue()
		self.fmter = fmter
		self.afterSplit = afterSplit
		self.mainOutputDir = None
		self.groupOutputFunc = None
		self.overwrite = False
	
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
	
	def setOutputDir(self, outputDir):
		if os.path.isdir(outputDir) and os.access(outputDir, os.W_OK):
			self.mainOutputDir = outputDir
		else:
			raise CannotWriteException("Cannot write to path", outputDir)

	def processNext(self):
		job = self.jobs.get()
		print("Processing job: {}".format(job.title))
		filename = job.generateFilename('%a-%s', self.fmter,replaceSpaces='_')+'.'+self.splitter.outputFilesFmt

		outdir = self.mainOutputDir if self.mainOutputDir else os.getcwd()
		if self.groupOutputFunc:
			subdir = self.groupOutputFunc(job)
			outdir = os.path.join(outdir, subdir)
			if not os.path.isdir(outdir):
				os.mkdir(outdir)

		outFile = os.path.join(outdir, filename)
		if os.path.exists(outFile):
			if self.overwrite:
				os.remove(outFile)
			else:
				raise FileAlreadyExistsException('File already exists and overwrite is set to false!', filename)

		out = self.splitter.splitOut(outFile, job.startTime, job.endTime-job.startTime)
		if out and self.afterSplit:
			self.afterSplit((job, outFile))

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
			raise TargetNotFoundException('File to be tagged was not found.')

		m = mp3_tagger.MP3File(filename)
		print('filename is {}'.format(filename))
		print('seg is {}'.format(seg))
		m.song = str(seg.title)
		#print(m.song)
		#for tag in MP3Tagger.possibleTags:
		#	if tag in seg.optionalAttrs:
		#		setattr(m, tag, seg.optionalAttrs[tag])
		#		print('{}={}'.format(tag,seg.optionalAttrs[tag]))
		#m.set_version(mp3_tagger.VERSION_BOTH)
		m.save()

from mutagen.easyid3 import EasyID3

class MutagenMP3Tagger(AudioTagger):
	possibleTags = [
		'artist',
		'album',
		'track',
		'genre',
		'year'
	]
	def tag(self, filename, seg):
		if not os.path.isfile(filename):
			raise TargetNotFoundException('File to be tagged was not found.')

		m = EasyID3(filename)
		m.delete()
		m['title'] = seg.title
		for tag in MutagenMP3Tagger.possibleTags:
			if tag in seg.optionalAttrs:
				m[tag] = ascii(seg.optionalAttrs[tag])
				print('setting {}={}'.format(tag,seg.optionalAttrs[tag]))
		print(m)
		m.save()

splitter = AudioSplitter('/home/sintel/Music/in.ogg')
splitter.splitTo('ogg')

tagger = MP3Tagger()
tagStep = lambda task: tagger.tag(task[1], task[0])
#tagStep = lambda null: print(null)

sch = SplitScheduler(splitter, tagger, StringFormater(AudioSegment.kwList))
sch.setOutputDir('/mnt/ramdisk')
sch.overwrite = False
albumNameFromAudSeg = lambda a: a.optionalAttrs['album'] if 'album' in a.optionalAttrs else ''
sch.groupOutputFunc = albumNameFromAudSeg

with open("../in_out/meta_short.txt") as f:
	amp = AudioMetaProcessor(f, sch.addJob)
	while not amp.atEOF():
		amp.processLine()
while sch.hasJobs():
	try:
		sch.processNext()
	except FileAlreadyExistsException as e:
		print('Skipping {} because overwrite was not set.'.format(e.errors))
