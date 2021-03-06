#!/bin/env python3
from typing import List
from typing import Dict
from typing import Set
from copy import deepcopy
import json
import sys
from itertools import islice

class Tray:
	f_duplicate_found = []
	def __init__(self, location: str, number: int, codes = None):
		self.location = location
		self.number = number
		self.codes = set(codes) if codes else set()
	
	def add(self, *codes: int):
		"""
		Adds codes to Tray. Duplicate codes will not
		be added and will call the DuplicateFound events.
		"""
		for code in codes:
			if code in self.codes:
				for f in Tray.f_duplicate_found:
					f(self.location, self.number, code)
			else:
				self.codes.add(code)
	
	def distinctFrom(self, otherTray):
		"""
		Returns the codes that this Tray has that
		the other does not.
		"""
		return self.codes - otherTray.codes
	
	def __repr__(self):
		return "#{} @ {}".format(self.number, self.location)
		
	def __len__(self):
		"""
		Returns # of codes.
		"""
		return len(self.codes)
	
	@staticmethod
	def AddEvent_DuplicateFound(f):
		"""
		Adds a function to be called when a duplicate
		is added. Function must take 3 args, which are
		this tray's location and number, and the duplicate
		code.
		"""
		Tray.f_duplicate_found.append(f);

class Tray_Err:
	def __init__(self):
		self.err_trays = {}
	
	def logError(self, location: str, number: int, code: int):
		"""
		Records an unique Tray Error. Errors are seperated by
		location and number.
		"""
		if location not in self.err_trays:
			self.err_trays[location] = {}
		if number not in self.err_trays[location]:
			self.err_trays[location][number] = set()
		self.err_trays[location][number].add(code)

class Collective:
	f_empty_removed = []
	def __init__(self, location: str, trays: Dict = None):
		self.location = location
		self.trays = trays if trays else {}
	
	@classmethod
	def fromDict(cls, vDict):
		"""
		Creates a Collective from a dictionary. Will raise an
		exception if the expected keys aren't there.
		"""
		if "location" not in vDict or "trays" not in vDict:
			raise KeyError
		t = {number : Tray(vDict["location"],number,codes) for number,codes, in vDict["trays"].items()}
		return cls(location = vDict["location"], trays = t)
	
	@classmethod
	def fromJson(cls, filename: str):
		"""
		Creates a Collective from a JSON file.
		"""
		with open(filename) as f:
			j = json.load(f)
			return cls.fromDict(j)
	
	def addTray(self, number: int):
		"""
		Adds a Tray with number to Collective.
		Duplicates are ignored.
		Returns the resultant tray (if new)
		"""
		if number in self.trays:
			pass
		else:
			self.trays[number] = Tray(self.location, number)
		return self.trays[number]
	
	def remTray(self, number: int):
		"""
		Removes a tray, if it exists, else nothing happens.
		"""
		if number in trays:
			trays.remove(number)
	
	def tray(self, number):
		"""
		Returns stored Tray with number.
		If said Tray wasn't found, an exception is raised.
		"""
		if number in self.trays:
			return self.trays[number]
		else:
			raise InvalidAccessErr
	
	def trackCode(self, code: int):
		"""
		A generator that yields numbers of Trays that
		contain that code.
		""" 
		# Finds the tray(s) that contains this code.
		yield from filter((lambda number: code in self.trays[number].codes), self.trays.keys())

	def compare(self,other):
		"""
		Returns the difference between this and another
		collective.
		"""
		return CollectiveDifference(self,other)

	def dropEmpty(self):
		"""
		Removes empty Trays.
		"""
		#empties = filter(
			#lambda t: len(self.trays[t])==0,
			#self.trays.keys()
		#)
		self.trays = {number : tray for number,tray in self.trays.items() if len(tray)>0}
		# I do not know if this 'dict comprehension' will recreate the Trays
		# but, it is the correct logic.

	def __len__(self):
		"""
		Returns count of trays stored.
		"""
		return len(self.trays)
	
	def __repr__(self):
		return "Collective of trays @ {}".format(self.location)

	def dict(self):
		"""
		Returns Collective as dict
		"""
		temp = {
			"location" : self.location,
			"trays" : {}
		}
		for number, tray in self.trays.items():
			temp["trays"][number] = list(tray.codes)
		return temp

	def saveJson(self, filename: str):
		"""
		Saves Collective as Json to filename
		"""
		with open(filename, 'w') as jout:
			json.dump(self.dict(), jout)
	
class CollectiveDifference:
	def __init__(self, lhs, rhs):
		self.lhs_name = lhs.location
		self.rhs_name = rhs.location
		self.lhs_distinct,self.lhs_item_count = self.get_distinct(lhs,rhs)
		self.rhs_distinct,self.rhs_item_count = self.get_distinct(rhs,lhs)
		self.limitCodes = 0
		self.trayFormat = "Tray #{}"
		self.codeFormat = "  {}"
		self.codeStr = None
	
	def get_distinct(self, lhs, rhs):
		"""
		Returns, what lhs (a Collective) has that rhs (another) doesn't, and
		the count of codes.
		"""
		distinct = {}
		count = 0
		for number, tray in lhs.trays.items():
			if number in rhs.trays:
				t = tray.distinctFrom(rhs.tray(number))
				if t:
					distinct[number] = t
					count += len(t)
			else:
				distinct[number] = '*'
				count += len(tray.codes)
				
		return (distinct, count)

	def yieldReport(self, name: str, dis: Dict[int,Set[int]], cnt: int):
		"""
		Yields formated report using name, dis, and count,
		
		"""
		yield "Distinct from {}".format(name)
		yield "----------"
		for number, codes in dis.items():
			yield self.trayFormat.format(number)
			lCodes = codes if self.limitCodes==0 else islice(codes, self.limitCodes)
			fCodes = map(self.codeStr, lCodes) if self.codeStr else lCodes
			yield from map(self.codeFormat.format, fCodes)
		yield "----------"
		yield "Tray Count = {}".format(len(dis))
		yield "Item Count = {}".format(cnt)
		yield "__________"
		yield ""
	
	def reportLeft(self):
		"""
		Shortcut to yieldReport on lhs
		"""
		yield from self.yieldReport(self.lhs_name, self.lhs_distinct, self.lhs_item_count)
		
	def reportRight(self):
		"""
		Shortcut to yieldReport on rhs
		"""
		yield from self.yieldReport(self.rhs_name, self.rhs_distinct, self.rhs_item_count)

	def genReport(self):
		"""
		Shortcut to yieldReport on lhs and rhs.
		"""
		yield from self.reportLeft()
		yield from self.reportRight()
	
	def __repr__(self):
		return '\n'.join(self.genReport())
	
	def printOut(self, out = print):
		"""
		Calls 'print' function with every line from genReport.
		"""
		for line in self.genReport():
			out(line)


def main(argv : List[str] = []):
	# Adding tray dup code events
	Tray.AddEvent_DuplicateFound(lambda l,n,c: print("DUP CODE %d AT %s #%d" %(c,l,n)))
	duplicate_barn = []
	Tray.AddEvent_DuplicateFound(lambda l,n,c: duplicate_barn.append((l,n,c)))
	te = Tray_Err()
	Tray.AddEvent_DuplicateFound(te.logError)

	# Init bbb collective
	bbb = Collective("bed body and booty")
	bbb.addTray(123).add(*range(5005,5055))
	bbb.addTray(456).add(*range(6006,6023))
	bbb.addTray(777).add(5043)

	# Init tspin collective
	tsp = Collective("tspin's")
	tsp.addTray(123).add(*range(5010,5070))
	
	#print(bbb.compare(tsp))
	bbb.compare(tsp).printOut(print)

	#print(list(bbb.trackCode(5023)))
	#print(list(bbb.trackCode(5043)))
	print(bbb.dict())

if __name__=="__main__":
	main(sys.argv)
