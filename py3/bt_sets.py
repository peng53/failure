#!/bin/env python3
from typing import List

class Tray:
	f_duplicate_found = []
	def __init__(self, location: str, number: int):
		self.location = location
		self.number = number
		self.codes = set()
	
	def add(self, code: int):
		if code in self.codes:
			for f in Tray.f_duplicate_found:
				f(self.location, self.number, code)
		else:
			self.codes.add(code)
	
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
		

def main(argv : List[str] = []):
	a = Tray("bed body and booty", 306)
	Tray.AddEvent_DuplicateFound(lambda l,n,c: print("DUP CODE %d AT %s #%d" %(c,l,n)))
	duplicate_barn = []
	Tray.AddEvent_DuplicateFound(lambda l,n,c: duplicate_barn.append((l,n,c)))
	te = Tray_Err()
	Tray.AddEvent_DuplicateFound(te.logError)
	a.add(10)
	a.add(3)
	a.add(10)
	print(duplicate_barn)
	print(te.err_trays)
