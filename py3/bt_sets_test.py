#!/bin/env python3
# bt_sets_test.py
import sys
import bt_sets
import random
from itertools import islice
from typing import List

def generateNumberCodes(fTrays, trayMaxCnt, fCodes, codeMaxCnt):
	"""
	Returns a List of Tuple consisting of a int, and a list of ints
	e.g. [ (1, [1,2,3]), (2, [2,3,4])]
	"""
	gTrays = map(int, filter((lambda t: random.randint(1,10) > 2), fTrays))
	gTraysLimit = islice(gTrays,trayMaxCnt)
	return [
		(
			number,
			list(
				map(int,(fCodes.readline() for _ in range(random.randint(0,codeMaxCnt))))
			)
		)
		for number in gTraysLimit
	]

def collectiveFromNumberCodes(location: str, numberCodes):
	"""
	Returns collective made from numberCodes
	"""
	return bt_sets.Collective.fromDict({
		"location" : location,
		"trays" : {
			number : codes for number,codes in numberCodes
		}
	})

def randomCollectiveFromFiles(location: str, fTrays, fCodes):
	"""
	Creates a random Collective from data in fTrays and fCodes.
	"""
	collective = bt_sets.Collective(location)
	gTrays = map(int, filter((lambda t: random.randint(1,10) > 2), fTrays))
	lCode = lambda: map(int, (fCodes.readline() for _ in range(random.randint(0,4))))
	vDict = {
		"location" : location,
		"trays" : {
			number : list(lCode()) for number in gTrays
		}
	}
	return bt_sets.Collective.fromDict(vDict)

def randomCollectiveFromFilesSameTrays(location: str, lTrays: List[str], fCodes):
	lCode = lambda: map(int, (fCodes.readline() for _ in range(random.randint(0,4))))
	vDict = {
		"location" : location,
		"trays" : {
			number : list(lCode()) for number in lTrays
		}
	}
	return bt_sets.Collective.fromDict(vDict)

def main(argv):
	"""
	Tests bt_sets with large data sets.
	Doesn't use argv.
	"""
	with open("../in_out/trays.txt", 'r') as trays, open("../in_out/codes.txt", 'r') as codes:
		numberCodes = generateNumberCodes(trays,250,codes,120)
	johns = collectiveFromNumberCodes(
		"Johns",
		numberCodes
	)
	# evenCodes = ((nc[0],[c for c in nc[1] if c%2]) for nc in numberCodes)
	evenTrays = filter(lambda nc: nc[0]%2, numberCodes)
	# If i filter for even trays first, I can save alot of work.
	evenTraysWithEvenCodes = map(
		lambda nc: (nc[0],[c for c in nc[1] if c%2 ]),
		evenTrays
	)
	# Marco's trays are only even with even codes.
	marcos = collectiveFromNumberCodes(
		"Marcos",
		evenTraysWithEvenCodes
	)
	marcos.addTray(123).add(909, 304, 90210)
	marcos.addTray(90210)
	marcos.dropEmpty()

	diff = johns.compare(marcos)
	diff.limitCodes = 5
	diff.trayPrintf = "Tray #{0:,d}"
	diff.printOut(print)
	

if __name__=="__main__":
	main(sys.argv)
