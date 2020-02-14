from jinja2 import Environment, FileSystemLoader
import json


def renderAsTable(data, out):
	env = Environment(loader = FileSystemLoader('templates'))
	tmpFile = "linksTableView.html"
	template = env.get_template(tmpFile)
	output = template.render(links=data['links'], groups=data['groups'])
	with open(out, 'w') as o:
		o.write(output)

dataf = 'linksrender.json'
out = 'linksJsonOut.html'

with open(dataf, 'r') as f:
	data = json.load(f)

renderAsTable(data, out)
