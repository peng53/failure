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

class LinksRenderer:
	def __init__(self):
		self.env = default_env()
		self.templateFile = ""
	
	def render(self, data):
		pass

class TabularLinkRenderer(LinksRenderer):
	def __init__(self):
		self.env = default_env()
		self.templateFile = "linksTableView.html"
	
	def render(self, data):
		template = self.env.get_template(self.templateFile)
		return template.render(data=data['groups'])

Link = namedtuple('Link', 'name,url')

class GroupedLinkRenderer(LinksRenderer):
	def __init__(self, tmpf: str=''):
		self.env = default_env()
		self.templateFile = tmpf if tmpf else "linksGroupView.html"

	def render(self, data):
		template = self.env.get_template(self.templateFile)
		'''
		groupedData = { i:[] for i,e in enumerate(data['groups'])}
		groupsCount = len(groupedData)

		ungroupedId, unknownId = -1, -2
		groupedData[ungroupedId] = []
		groupedData[unknownId] = []

		for l in data['links']:
			if 'group' in l:
				if l['group']<groupsCount:
					groupedData[l['group']].append(Link(l['name'],l['url']))
				else:
					groupedData[unknownId].append(Link('g{}|{}'.format(l['group'],l['name']),l['url']))
			else:
				groupedData[ungroupedId].append(Link(l['name'],l['url']))

		groupNames = data['groups']+['[[Ungrouped]]','[[Unknown Group]]']
		return template.render(groups=groupedData, groupNames = groupNames)
		'''
		return template.render(data=data['groups'])

class DomainGroupedLinkRenderer(LinksRenderer):
	'''Will eventually be combined with GroupLinkRenderer as an option'''
	def __init__(self, tmpf: str=''):
		self.env = default_env()
		self.templateFile = tmpf if tmpf else "linksGroupView.html"

	def render(self, data):
		template = self.env.get_template(self.templateFile)
		(groups, links) = groupByDomain(data['links'])
		groupNames = [i for i in groups]
		return template.render(groups=links, groupNames = groupNames)

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
	if endIndex==-1:
		# Url does not end in '/'
		endIndex = len(url)
	return url[startIndex:startIndex+endIndex]

def printDataGroupedByDomain(links):
	(groups, links) = groupByDomain(links)
	for i,g in enumerate(groups):
		print(g)
		for l in links[i]:
			print('  {}'.format(l.name))

def output2file(output, outputFile):
	with open(outputFile, 'w') as o:
		o.write(output)

class MainParser:
	def __init__(self):
		self.renderers = {
			'table': TabularLinkRenderer,
			'grouped': GroupedLinkRenderer,
			#'domain': DomainGroupedLinkRenderer,
			#'groupedSingle': lambda: GroupedLinkRenderer(tmpf='linksSingleView.html'),
			#'domainSingle': lambda: DomainGroupedLinkRenderer(tmpf='linksSingleView.html')
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
		renderer = self.renderers[t.render]()
		output = renderer.render(data)
		output2file(output, t.out)

if __name__=='__main__':
	m = MainParser()
	m.parse(argv[1:])
