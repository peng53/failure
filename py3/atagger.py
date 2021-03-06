from mutagen.easyid3 import EasyID3
from subprocess import run
import mp3_tagger
import os

class TargetNotFoundException(Exception):
	pass


class AudioTagger:
	def tag(self, AudioSegment):
		pass

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

class MutagenTagger(AudioTagger):
	possibleTags = [
		'artist',
		'album',
		'genre',
		'title',
		'year'
	]
	def tag(self, filename, seg):
		if not os.path.isfile(filename):
			raise TargetNotFoundException('File to be tagged was not found.')
		m = EasyID3(filename)
		m.delete()
		for tag in MutagenTagger.possibleTags:
			if tag in seg.optionalAttrs:
				m[tag] = seg.optionalAttrs[tag]
		if 'track' in seg.optionalAttrs:
			m['tracknumber'] = seg.optionalAttrs['track']
		m.save(v1=2)

class mp3tagTagger(AudioTagger):
	possibleTags = {
		'artist' : 'a',
		'album' : 'l',
		'track' : 'k',
		'title' : 's',
		'genre': 'g',
		'year': 'y'
	}

	def tag(self, filename, seg):
		if not os.path.isfile(filename):
			raise TargetNotFoundException('File to be tagged was not found.')

		tagline = {
			self.possibleTags[tag] : seg.optionalAttrs[tag]
			for tag in self.possibleTags
			if tag in seg.optionalAttrs
		}

		args = ['mp3tag']
		for k in tagline:
			args.append('-{}'.format(k))
			args.append(tagline[k])
		args.append(filename)

		r = run(args, check=True)

