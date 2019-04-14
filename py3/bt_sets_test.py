#!/bin/env python3
# bt_sets_test.py
import sys
import bt_sets
import random
from typing import List

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
	with open("../in_out/trays.txt", "r") as trays:
		ltrays = list(map(int, filter((lambda t: random.randint(1,10) > 2), trays)))
	with open("../in_out/codes.txt", "r") as codes:
		johns = randomCollectiveFromFilesSameTrays("Johns", ltrays, codes);
		#johns = randomCollectiveFromFiles("Johns",trays,codes)
	with open("../in_out/codes.txt", "r") as codes:
		marcos = randomCollectiveFromFilesSameTrays("Marcos", ltrays, codes);
		#marcos = randomCollectiveFromFiles("Marcos",trays,codes)

	diff = johns.compare(marcos)
	diff.printOut(print)
	#print(diff)
	


if __name__=="__main__":
	main(sys.argv)
