#!/usr/bin/env python3
import datetime
import calendar

class DateSpan:
	def __init__(self, start, end = None):
		'''	Constructs a DateSpan where start and end
				are datetime.date
		'''
		self.start = start
		self.end = end if end else start
		self.diff = (self.end-self.start).days

	def __repr__(self):
		return '{} to {} / {} days'.format(self.start,self.end,self.diff+1)

	def dayOfTheWeeks(self):
		'''	Returns list of weekdays in span
		'''
		dow = start.weekday()
		return [dow+i for i in range(self.diff+1)]
	
	def median(self):
		'''	Returns middle date of datespan.
				If even, return greater.
		'''
		return self.end-datetime.timedelta(days=self.diff//2)
	
	def splitAt(self, point):
		'''	Returns list of datespans created by
				removing a day in this datespan. Where point
				is a datetime.date
		'''
		if point < self.start or point > self.end or self.diff == 0:
			# point didn't occur or is the only day.
			return []
		tstart,tend,tmid = self.start, self.end, None
		if tstart == point:
			tstart += datetime.timedelta(days=1)
		elif tend == point:
			tend += datetime.timedelta(days=-1)
		else:
			tmid = point
		if tmid:
			return (
				DateSpan(tstart, tmid + datetime.timedelta(days=-1)),
				DateSpan(tmid + datetime.timedelta(days=1),tend)
			)
		else:
			return [DateSpan(tstart,tend)]


x = DateSpan(
	datetime.date(2019,2,4),
	datetime.date(2019,2,6)
)
splitX = datetime.date(2019,2,5)
splited = x.splitAt(splitX)
for ds in splited:
	print(ds)

# Using the 'division method'
vacation_days = 5*3
good_interval = (365 // vacation_days) - 1
year = 2020
day1 = datetime.date(year,1,1)
possible_days = [day1+datetime.timedelta(days=x) for x in range(good_interval,good_interval*(vacation_days+1),good_interval)]
# Now to 'snap' the possible days to the closest friday
better_possible_days = []
for day in possible_days:
	wd = day.weekday()
	if wd == 4:
		better_possible_days.append(day)
	elif wd == 0:
		# Closest is 'prev' friday (left)
		better_possible_days.append(day + datetime.timedelta(days=-3))
	elif wd > 4:
		# Closest is 'prev' friday (left)
		better_possible_days.append(day + datetime.timedelta(days=4-wd))
	elif wd >= 1 and wd < 4:
		# Closest is 'next' friday (right)
		better_possible_days.append(day + datetime.timedelta(days=4-wd))
for d in better_possible_days:
	print(d)
print(len(better_possible_days))
