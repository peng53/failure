from jinja2 import Environment, FileSystemLoader
from collections import namedtuple
import json


def renderAsTable(data, out):
	env = Environment(loader = FileSystemLoader('templates'))
	tmpFile = "linksTableView.html"
	template = env.get_template(tmpFile)
	output = template.render(links=data['links'], groups=data['groups'])
	with open(out, 'w') as o:
		o.write(output)

Link = namedtuple('Link', 'name,url')

def renderAsGrouped(data, out):
	env = Environment(loader = FileSystemLoader('templates'))
	tmpFile = "linksGroupView.html"
	template = env.get_template(tmpFile)

	groupedData = { i:[] for i,e in enumerate(data['groups'])}
	groupsCount = len(groupedData)
	for l in data['links']:
		if l['group']<groupsCount:
			groupedData[l['group']].append(Link(l['name'],l['url']))
	
	output = template.render(groups=groupedData, groupNames = data['groups'])
	with open(out, 'w') as o:
		o.write(output)

dataf = 'linksrender.json'
out = 'linksJsonOut.html'

with open(dataf, 'r') as f:
	data = json.load(f)

#renderAsTable(data, out)
renderAsGrouped(data,out)
