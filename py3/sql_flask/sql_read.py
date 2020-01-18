from flask import Flask, render_template
from collections import namedtuple

app = Flask(__name__, template_folder= "templates")

datarow = namedtuple('Datarow', 'first,last,bdate,sex')

@app.route('/read')
def mainRead():
	columns = ['First Name', 'Last Name', 'Birth Date', 'Sex']
	data = [
		datarow('Mike','Tyler','1-1-2002', 'M'),
		datarow('Tom','Fitch','5-10-2004', 'M'),
		datarow('Lura','Marlis','2-5-1994', 'F')
	]
	return render_template('table.html', cols=columns, data=data)

