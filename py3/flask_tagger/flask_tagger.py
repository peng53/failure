from flask import Flask, render_template, url_for, request, redirect
from mutagen.easyid3 import EasyID3

app = Flask(__name__, template_folder= "templates")

@app.route('/req', methods = ['POST', 'GET'])
def reqpage():
	if request.method == 'POST':
		info = request.form
		with open('/mnt/ramdisk/testflask', 'a') as f:
			f.write(str(info))
		return redirect(url_for('sendinfo'))
	else:
		return redirect(url_for('sendinfo'))

@app.route('/tag', methods=['POST', 'GET'])
def tag():
	if request.method == 'POST':
		info = request.form
		try:
			meta = EasyID3(info['filename'])
			return render_template(
				'tag.html',
				to=url_for('reqpage'),
				mfile=info['filename'],
				meta = meta
			)
		except Exception as e:
			print(e)
			return 'opps!'


@app.route('/select')
def selectfile():
	return render_template('select_file.html', to=url_for('tag'))
