#!/bin/env python3
from typing import List
from copy import deepcopy

class Tray:
	f_duplicate_found = []
	def __init__(self, location: str, number: int):
		self.location = location
		self.number = number
		self.codes = set()
	
	def add(self, *codes: List[int]):
		for code in codes:
			if code in self.codes:
				for f in Tray.f_duplicate_found:
					f(self.location, self.number, code)
			else:
				self.codes.add(code)
	
	def distinctFrom(self, otherTray):
		return self.codes - otherTray.codes
	
	def __repr__(self):
		return "#{} @ {}".format(self.number, self.location)
	
	@staticmethod
	def AddEvent_DuplicateFound(f):
		Tray.f_duplicate_found.append(f);

class Tray_Err:
	def __init__(self):
		self.err_trays = {}
	
	def logError(self, location: str, number: int, code: int):
		if location not in self.err_trays:
			self.err_trays[location] = {}
		if number not in self.err_trays[location]:
			self.err_trays[location][number] = set()
		self.err_trays[location][number].add(code)
		

class Collective:
	def __init__(self, location: str):
		self.location = location
		self.trays = {}
	
	def addTray(self, number):
		if number in self.trays:
			pass
		else:
			self.trays[number] = Tray(self.location, number)
		return self.trays[number]
	
	def tray(self, number):
		if number in self.trays:
			return self.trays[number]
		else:
			raise InvalidAccessErr
	
	def trackCode(self, code):
		# Finds the tray(s) that contains this code.
		out = set()
		for tray in self.trays.values():
			if code in tray.codes:
				out.add(tray.number)
		return out

	def compare(self,other):
		return CollectiveDifference(self,other)

	def __len__(self):
		return len(self.trays)
	
	def __repr__(self):
		return "Collective of trays @ {}".format(self.location)

class CollectiveDifference:
	def __init__(self, lhs, rhs):
		self.lhs_name = lhs.location
		self.rhs_name = rhs.location
		self.lhs_distinct = self.get_distinct(lhs,rhs)
		self.rhs_distinct = self.get_distinct(rhs,lhs)
	
	def get_distinct(self, lhs, rhs):
		distinct = {}
		for tray in lhs.trays:
			if tray in rhs.trays:
				t = lhs.tray(tray).distinctFrom(rhs.tray(tray))
				if t:
					distinct[tray] = t
			else:
				distinct[tray] = set('*')
		return distinct
	
	def __str__(self):
		report = []
		report.append("Distinct from {}".format(self.lhs_name))
		report.append("--")
		for tray in self.lhs_distinct:
			report.append("Tray #{}".format(tray))
			report.append('  '+"\n  ".join(str(c) for c in self.lhs_distinct[tray]))

		report.append("--")
		report.append("Distinct from {}".format(self.rhs_name))
		report.append("--")
		for tray in self.rhs_distinct:
			report.append("Tray #{}".format(tray)) 
			report.append('  '+"\n  ".join(str(c) for c in self.rhs_distinct[tray]))
		
		return '\n'.join(report)

def main(argv : List[str] = []):
	# Adding tray dup code events
	Tray.AddEvent_DuplicateFound(lambda l,n,c: print("DUP CODE %d AT %s #%d" %(c,l,n)))
	duplicate_barn = []
	Tray.AddEvent_DuplicateFound(lambda l,n,c: duplicate_barn.append((l,n,c)))
	te = Tray_Err()
	Tray.AddEvent_DuplicateFound(te.logError)

	# Init bbb collective
	bbb = Collective("bed body and booty")
	bbb.addTray(123).add(*list(range(5005,5055)))
	bbb.addTray(456).add(*list(range(6006,6023)))

	# Init tspin collective
	tsp = Collective("tspin's")
	tsp.addTray(123).add(*list(range(5010,5070)))
	
	print(str(bbb.compare(tsp)))
