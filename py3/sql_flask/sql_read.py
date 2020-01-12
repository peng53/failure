from flask import Flask, render_template
from collections import namedtuple

app = Flask(__name__, template_folder= "templates")

datarow = namedtuple('Datarow', 'first,last,bdate')

@app.route('/read')
def mainRead():
	columns = ['First Name', 'Last Name', 'Birth Date']
	data = [
		datarow('Mike','Tyler','1-1-2002'),
		datarow('Tom','Fitch','5-10-2004')
	]
	return render_template('table.html', cols=columns, data=data)

