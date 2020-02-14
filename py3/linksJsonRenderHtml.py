from jinja2 import Environment, FileSystemLoader
import json

env = Environment(loader = FileSystemLoader('templates'))
tmpFile = "linksTableView.html"
template = env.get_template(tmpFile)

with open('linksrender.json', 'r') as f:
	data = json.load(f)

output = template.render(links=data['links'])
with open('linksJsonOut.html', 'w') as o:
	o.write(output)
