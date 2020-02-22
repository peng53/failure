from jinja2 import Environment, FileSystemLoader
from argparse import ArgumentParser
from sys import argv, exit
from operator import itemgetter
import json
import re
import os

def default_env():
	env = Environment(loader = FileSystemLoader('templates'))
	env.trim_blocks = True
	env.lstrip_blocks = True
	return env

class LinksRenderer():
	def __init__(self, templateFile):
		self.env = default_env()
		self.templateFile = templateFile
		self.regroupf = None
		self.sorturl = None
		self.groupFilters = []
		self.urlFilters = []
	
	def render(self, data):
		template = self.env.get_template(self.templateFile)
		processedData = data
		if self.urlFilters:
			processedData = {
				'groups' : [
					{k:(list(chainFilters(self.urlFilters, v)) if k=='urls' else v) for (k,v) in g.items()} for g in data['groups']
				]
			}
		if self.regroupf:
			processedData = self.regroupf(processedData)
		if self.groupFilters:
			processedData = {
				'groups' : list(chainFilters(self.groupFilters, processedData['groups']))
			}
		if self.sorturl:
			self.sorturl(processedData)
		return template.render(data=processedData['groups'])


def groupByF(data, f):
	groupMap = {}
	out = {'groups': []}
	for group in data['groups']:
		for link in group['urls']:
			key = f(link)
			if key not in groupMap:
				groupMap[key] = len(groupMap)
				out['groups'].append({'name': key, 'urls': []})
			out['groups'][groupMap[key]]['urls'].append(link)
	out['groups'].sort(key=itemgetter('name'))
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


def sortByName(data):
	for g in data['groups']:
		g['urls'].sort(key=itemgetter('name'))

def chainFilters(filters, data):
	for value in data:
		if all(f(value) for f in filters):
			# if value passes through all filters..
			yield value


def output2file(output, outputFile):
	with open(outputFile, 'w') as o:
		o.write(output)



class MainParser:
	def __init__(self):
		self.templates = {
			'table': 'linksTableView.html',
			'group': 'linksGroupView.html',
			'single' : 'linksSingleView.html',
			'bullet' : 'linksBulletView.html',
			'xml' : 'linksXml.xml',
		}
		self.parser = ArgumentParser(description='URL Data Json2Html Renderer')
		self.parser.add_argument('out', help='Html output file')
		self.parser.add_argument('--json','-i', action='append', type=str, help='Json input file(s)')
		self.parser.add_argument('--template','-t', help='Which template to use for rendering', choices=self.templates.keys(), default='table')
		self.parser.add_argument('--gdomain', action='store_true', help='Group URLS by domain instead')
		self.parser.add_argument('--gletter', action='store_true', help='Group URLS by letter instead')
		self.parser.add_argument('--nsorted', action='store_true', help='Sort URLS by name (per group)')
		

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

		renderer = LinksRenderer(self.templates[t.template])
		renderer.urlFilters.append(lambda u: u['url'].startswith('www.'))
		renderer.urlFilters.append(lambda u: u['name'].startswith('Website'))
		renderer.groupFilters.append(lambda g: g['name'].startswith('group1'))
		if t.gdomain:
			renderer.regroupf = lambda d: groupByF(d, lambda x: getDomain(x['url']))
		elif t.gletter:
			renderer.regroupf = lambda d: groupByF(d, lambda x: x['name'][0].upper())
		if t.nsorted:
			renderer.sorturl = sortByName

		output = renderer.render(data)
		output2file(output, t.out)

if __name__=='__main__':
	m = MainParser()
	m.parse(argv[1:])
