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
		self.inGroupFilters = []
		self.outGroupFilters = []
		self.urlFilters = []
	
	def render(self, data):
		template = self.env.get_template(self.templateFile)
		processedData = data
		if self.inGroupFilters:
			 processedData = {
				'groups' : list(chainFilters(self.inGroupFilters, processedData['groups']))
			 }
		if self.urlFilters:
			processedData = {
				'groups' : [
					{k:(list(chainFilters(self.urlFilters, v)) if k=='urls' else v) for (k,v) in g.items()} for g in processedData['groups']
				]
			}
		if self.regroupf:
			processedData = self.regroupf(processedData)
		if self.outGroupFilters:
			processedData = {
				'groups' : list(chainFilters(self.outGroupFilters, processedData['groups']))
			}
		if self.sorturl:
			sortByProp(processedData, self.sorturl)
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

def getCountry(url: str):
	urlStartPat = re.compile('^((http://)|(https://))?(www\.)?')
	startIndex = urlStartPat.search(url).span()[1]
	# skip past the http / www
	endIndex = url[startIndex:].find('/')
	# find end of top level
	if endIndex==-1:
		# Url does not end in '/'
		endIndex = len(url)
	else:
		endIndex += startIndex 
	startIndex += url[startIndex:endIndex].rfind('.')
	# find period separating domain and country
	return url[startIndex+1:endIndex]


def sortByProp(data, keys):
	for g in data['groups']:
		g['urls'].sort(key=itemgetter(*keys))

def chainFilters(filters, data):
	for value in data:
		if all(f(value) for f in filters):
			# if value passes through all filters..
			yield value



def concatGroups(data):
	out = {'groups' : [{'name': 'All Urls', 'urls':[]}]}
	for g in data['groups']:
		out['groups'][0]['urls'].extend(g['urls'])
	return out



def output2file(output, outputFile):
	with open(outputFile, 'w') as o:
		o.write(output)

class UniqueSetContainer:
	def __init__(self, key, verbose=False):
		self.items = set()
		self.key = key
		self.verbose = verbose
	
	def isUnique(self, item):
		if item[self.key] in self.items:
			if self.verbose:
				print('id: {}, dropped {}: {}'.format(id(self),self.key, item))
			return False
		else:
			self.items.add(item[self.key])
			return True

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

		self.regroupArg()
		self.userFilterArg()
		self.stdFilterArg()

	def regroupArg(self):
		self.parser.add_argument('--gdomain', action='store_true', help='Group URLS by domain')
		self.parser.add_argument('--gcountry', action='store_true', help='Group URLS by country')
		self.parser.add_argument('--gletter', action='store_true', help='Group URLS by letter')
		self.parser.add_argument('--gconcat', action='store_true', help='Group URLS in single group')
		self.parser.add_argument('--sorted', action='append', help='Sort URLS by a property (per group)', default=[])
	
	def userFilterArg(self):
		self.parser.add_argument('--uf', action='append', help='Filter urls', default=[])
		self.parser.add_argument('--gf', action='append', help='Filter incoming groups', default=[])
		self.parser.add_argument('--fg', action='append', help='Filter outcoming groups', default=[])	

	def stdFilterArg(self):
		self.parser.add_argument('--dropEmpty', action='store_true', help='Drop empty groups')
		self.parser.add_argument('--unique', action='append', help='Keep only first occurances of either url or name', choices=('url','name'), default=[])


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
		for a in t.unique:
			renderer.urlFilters.append(UniqueSetContainer(a).isUnique)
		renderer.urlFilters.extend(MainParser.parseUserFilters(t.uf))

		if t.dropEmpty:
			renderer.inGroupFilters.append(lambda d: len(d['urls'])>0)
		renderer.inGroupFilters.extend(MainParser.parseUserFilters(t.gf))

		renderer.outGroupFilters = MainParser.parseUserFilters(t.fg)
		if t.gdomain:
			renderer.regroupf = lambda d: groupByF(d, lambda x: getDomain(x['url']))
		elif t.gcountry:
			renderer.regroupf = lambda d: groupByF(d, lambda x: getCountry(x['url']))
		elif t.gletter:
			renderer.regroupf = lambda d: groupByF(d, lambda x: x['name'][0].upper())
		elif t.gconcat:
			renderer.regroupf = concatGroups
		if t.sorted:
			renderer.sorturl = t.sorted

		output = renderer.render(data)
		output2file(output, t.out)

	def parseUserFilters(filters):
		generatedFilters = []
		for f in filters:
			kv = f.split('=', maxsplit=1)
			if len(kv)==2:
				generatedFilters.append(HasKeyAndValue(*kv).passes)
		return generatedFilters

class HasKeyAndValue:
	def __init__(self, key, value):
		self.key, self.value = key, value
	
	def passes(self, item):
		return self.key in item and self.value in item[self.key]


if __name__=='__main__':
	m = MainParser()
	m.parse(argv[1:])
