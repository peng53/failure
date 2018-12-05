#!/usr/bin/env python3
import datetime
import calendar
input_year = 2019 # make it an argv.
g = datetime.datetime(year=2018,month=12,day=5)
working_days = [0]*12 # count per month.
days_off = [[] for m in range(12)] # days already off.
days_off[0].append(1) # january 1st.
days_off[0].append(21)
days_off[1].append(18) # febuary 18th.
days_off[4].append(27) # may 27th.
days_off[6].append(4) # you see the pattern?
days_off[8].append(2)
days_off[10].append(11)
days_off[10].append(28)
days_off[11].append(25)

#print(g)
#print(calendar.day_name[g.weekday()])
#print(calendar.monthrange(2018,2))
for month in range(1,13):
	cur_day_of_the_week,day_count = calendar.monthrange(input_year,month)
	for day in range(1,day_count+1):
		if cur_day_of_the_week<5 and day not in days_off[month-1]:
			working_days[month-1] += 1
		else:
			#print("(%s)Day-off %4d-%02d-%02d" %('WKE' if cur_day_of_the_week>=5 else 'HOL',input_year,month,day))
			print("({})Day-off {:>4}-{:>02}-{:>02}".format('WKE' if cur_day_of_the_week>=5 else 'HOL',input_year,month,day))
		cur_day_of_the_week += 1
		if cur_day_of_the_week>=7:
			cur_day_of_the_week = 0
print("-"*20)
for month in range(0,12):
	print("{0} {1}({2})".format(calendar.month_abbr[month+1],working_days[month],len(days_off[month])))
