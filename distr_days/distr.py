#!/usr/bin/env python3
import datetime
import calendar
import heapq
import bisect

input_year = 2019 # make it an argv.
g = datetime.datetime(year=2018,month=12,day=5)
working_days = [0]*12 # count per month.
days_off = [set() for m in range(12)] # days already off.
days_off[0].add(1) # january 1st.
days_off[0].add(21)
days_off[1].add(18) # febuary 18th.
days_off[4].add(27) # may 27th.
days_off[6].add(4) # you see the pattern?
days_off[8].add(2)
days_off[10].add(11)
days_off[10].add(28)
days_off[11].add(25)

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
	print("{} {}({})".format(calendar.month_abbr[month+1],working_days[month],len(days_off[month])))

free_days = 5*3 + 1
#free_and_month = sorted(((working_days[m],m) for m in range(0,12)))
#print(free_and_month)
h = []
for month,cnt in enumerate(working_days):
	heapq.heappush(h,(-cnt,month))
to_add = [0]*12
for n in range(free_days):
	cnt, mth = heapq.heappop(h)
	#to_add.append(mth)
	to_add[mth] += 1
	working_days[mth] -= 1
	heapq.heappush(h,(cnt+1,mth))

print("-"*20)
for month in range(0,12):
	print("{} {}({}+{})".format(calendar.month_abbr[month+1],working_days[month],len(days_off[month]),to_add[month]))

def suggest_days(month,taken_s,count,last_dom):
	# find n such that 1<=n<=|cnt| and dist(n,x) is maximal for x in month.
	# Or something close. Since these 'pts' lie on just an x axis, just
	# using the max midpt(day) for each day would be sufficient.
	taken_l = [0]+list(taken_s)+[last_dom]
	# to get boundaries, we add the min and max, 0 and last+1
	for n in range(count):
		# get midpts between d1,d2; d2,d3 .. dx-1,dx.
		mid = [(taken_l[i+1]-taken_l[i],(taken_l[i+1]+taken_l[i])//2) for i in range(len(taken_l)-1)]
		d = (max(mid))[1]
		bisect.insort(taken_l,d)
	return taken_l[1:-1]

#suggest_days(0,days_off[0],3)
#print(suggest_days(0,[1,4,31],2))
for month in range(0,12):
	print(calendar.month_abbr[month+1])
	if to_add[month]>0:
		dom = calendar.monthrange(input_year,month+1)[1]
		print(suggest_days(month,days_off[month],to_add[month],dom))
	else:
		print(list(days_off[month]))

