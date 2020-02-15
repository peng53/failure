from jinja2 import Environment, FileSystemLoader
from collections import namedtuple
import json
import re

def default_env():
	env = Environment(loader = FileSystemLoader('templates'))
	env.trim_blocks = True
	env.lstrip_blocks = True
	return env

def renderAsTable(data, out: str):
	env = default_env()
	tmpFile = "linksTableView.html"
	template = env.get_template(tmpFile)
	output = template.render(links=data['links'], groups=data['groups'])
	with open(out, 'w') as o:
		o.write(output)

Link = namedtuple('Link', 'name,url')

def renderAsGrouped(data, out: str):
	env = default_env()
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

def renderAsGroupedByDomain(data, out: str):
	env = default_env()
	tmpFile = "linksGroupView.html"
	template = env.get_template(tmpFile)

	(groups, links) = groupByDomain(data['links'])
	groupNames = [i for i in groups]

	output = template.render(groups=links, groupNames = groupNames)
	with open(out, 'w') as o:
		o.write(output)

def groupByDomain(links):
	# links is a [{'name', '..', 'url': '..'}, ..}]
	domainNames = {}
	domainData = {}
	domain_reg = re.compile(r'(http://)?(www\.)?\w+\.\D+/')
	for link in links:
		domain = getDomain(link['url'])
		if domain not in domainNames:
			i = len(domainNames)
			domainNames[domain] = i
			domainData[i] = []
		domainData[domainNames[domain]].append(Link(link['name'],link['url']))
	return (domainNames, domainData)


def getDomain(url: str):
	startIndex = 0
	if url[startIndex:].startswith('https://'):
		startIndex += len('https://')
	elif url[startIndex:].startswith('http://'):
		startIndex += len('http://')
	if url[startIndex:].startswith('www.'):
		startIndex += len('www.')
	endIndex = url[startIndex:].find('/')
	return url[startIndex:startIndex+endIndex]

def printDataGroupedByDomain(links):
	(groups, links) = groupByDomain(links)
	for i,g in enumerate(groups):
		print(g)
		for l in links[i]:
			print(f'  {l.name}')


dataf = '/home/sintel/Documents/WEB/bookmarks.json'
out = 'linksJsonOut.html'

with open(dataf, 'r') as f:
	data = json.load(f)

#renderAsTable(data, out)
#renderAsGrouped(data,out)
renderAsGroupedByDomain(data,out)
#printDataGroupedByDomain(data['links'])

