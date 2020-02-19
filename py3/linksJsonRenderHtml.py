from jinja2 import Environment, FileSystemLoader
from argparse import ArgumentParser
from sys import argv, exit
from abc import ABC, abstractmethod
import json
import re
import os

def default_env():
	env = Environment(loader = FileSystemLoader('templates'))
	env.trim_blocks = True
	env.lstrip_blocks = True
	return env

class LinksRenderer(ABC):
	@abstractmethod
	def render(self, data):
		pass


class TabularLinkRenderer(LinksRenderer):
	def __init__(self):
		self.env = default_env()
		self.templateFile = "linksTableView.html"
	
	def render(self, data):
		template = self.env.get_template(self.templateFile)
		return template.render(data=data['groups'])


class GroupedLinkRenderer(LinksRenderer):
	def __init__(self, tmpf: str='', regroupl=None):
		self.env = default_env()
		self.templateFile = tmpf if tmpf else "linksGroupView.html"
		self.regroupl = regroupl

	def render(self, data):
		template = self.env.get_template(self.templateFile)
		if self.regroupl:
			groupedData = self.regroupl(data)
		else:
			groupedData = data
		return template.render(data=groupedData['groups'])


def groupByDomain(data):
	groupMap = {}
	out = {'groups': []}
	for group in data['groups']:
		for link in group['urls']:
			domain = getDomain(link['url'])
			if domain not in groupMap:
				groupMap[domain] = len(groupMap)
				out['groups'].append({'name': domain, 'urls': []})
			out['groups'][groupMap[domain]]['urls'].append(link)
	return out

def getDomain(url: str):
	urlStartPat = re.compile('^((http://)|(https://))?(www\.)?')
	startIndex = urlStartPat.search(url).span()[1]
	# Finds start of URL ignoring https / http / www
	endIndex = url[startIndex:].find('/')
	if endIndex==-1:
		# Url does not end in '/'
		endIndex = len(url)
	return url[startIndex:startIndex+endIndex]


def output2file(output, outputFile):
	with open(outputFile, 'w') as o:
		o.write(output)



class MainParser:
	def __init__(self):
		self.renderers = {
			'table': TabularLinkRenderer,
			'grouped': GroupedLinkRenderer,
			'domain': lambda: GroupedLinkRenderer(regroupl=groupByDomain),
			'groupedSingle': lambda: GroupedLinkRenderer(tmpf='linksSingleView.html'),
			'domainSingle': lambda: GroupedLinkRenderer(tmpf='linksSingleView.html',regroupl=groupByDomain)
		}
		self.parser = ArgumentParser(description='URL Data Json2Html Renderer')
		self.parser.add_argument('out', help='Html output file')
		self.parser.add_argument('--json','-i', action='append', type=str, help='Json input file(s)')
		self.parser.add_argument('--render','-r', help='How to render output', choices=self.renderers.keys(), default='table')

	def parse(self, args) -> None:
		t = self.parser.parse_args(args)
		if len(t.json)==0:
			print('No json file as input!')
			exit(2)
		if os.path.exists(t.out):
			print('File: {} already exists.'.format(t.out))
			exit(1)
		data = { 'groups' : []}
		for filename in t.json:
			with open(filename, 'r') as f:
				data['groups'].extend(json.load(f)['groups'])
		renderer = self.renderers[t.render]()
		output = renderer.render(data)
		output2file(output, t.out)

if __name__=='__main__':
	m = MainParser()
	m.parse(argv[1:])
