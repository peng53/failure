from jinja2 import Environment, FileSystemLoader
from collections import namedtuple
from argparse import ArgumentParser
from sys import argv, exit
import json
import re
import os

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

def renderAsGrouped(data, out: str, single: bool=False):
	env = default_env()
	tmpFile = "linksSingleView.html" if single else "linksGroupView.html"
	template = env.get_template(tmpFile)

	groupedData = { i:[] for i,e in enumerate(data['groups'])}
	ungroupedId = len(groupedData)
	groupedData[ungroupedId] = []
	groupsCount = len(groupedData)

	for l in data['links']:
		if 'group' in l:
			if l['group']<groupsCount:
				groupedData[l['group']].append(Link(l['name'],l['url']))
		else:
			groupedData[ungroupedId].append(Link(l['name'],l['url']))

	groupNames = data['groups']+['[[Ungrouped]]']
	output = template.render(groups=groupedData, groupNames = groupNames)
	with open(out, 'w') as o:
		o.write(output)

def renderAsGroupedByDomain(data, out: str, single: bool=False):
	env = default_env()
	tmpFile = "linksSingleView.html" if single else "linksGroupView.html"
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
	urlStartPat = re.compile('^((http://)|(https://))?(www\.)?')
	startIndex = urlStartPat.search(url).span()[1]
	# Finds start of URL ignoring https / http / www
	endIndex = url[startIndex:].find('/')
	return url[startIndex:startIndex+endIndex]

def printDataGroupedByDomain(links):
	(groups, links) = groupByDomain(links)
	for i,g in enumerate(groups):
		print(g)
		for l in links[i]:
			print('  {}'.format(l.name))

renderAsGroupedSingle = lambda d,o: renderAsGrouped(d,o, True)
renderAsGroupedByDomainSingle = lambda d,o: renderAsGroupedByDomain(d,o, True)

class MainParser:
	def __init__(self):
		self.renderers = {
			'table': renderAsTable,
			'grouped': renderAsGrouped,
			'groupedSingle': renderAsGroupedSingle,
			'domain': renderAsGroupedByDomain,
			'domainSingle': renderAsGroupedByDomainSingle
		}
		self.parser = ArgumentParser(description='URL Data Json2Html Renderer')
		self.parser.add_argument('jdata', help='Json data file')
		self.parser.add_argument('out', help='Html output file')
		self.parser.add_argument('--render','-r', help='How to render output', choices=self.renderers.keys(), default='table')

	def parse(self, args) -> None:
		t = self.parser.parse_args(args)
		if os.path.exists(t.out):
			print('File: {} already exists.'.format(t.out))
			exit(1)
		with open(t.jdata, 'r') as f:
			data = json.load(f)
		self.renderers[t.render](data, t.out)

if __name__=='__main__':
	m = MainParser()
	m.parse(argv[1:])
