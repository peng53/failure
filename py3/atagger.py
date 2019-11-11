from mutagen.easyid3 import EasyID3
import ffmpeg
import mp3_tagger

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
