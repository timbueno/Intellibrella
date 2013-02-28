from datetime import datetime
from pygeocoder import Geocoder

from DarkAPI import DarkAPI

def wNotifier(lat=33.76, lng=-84.39, qhours=(0,6)):
	
	# Could just take the zipcode
	# Better to have webapp give us lat and lng
	# that way we dont have to call google for
	# geocode every time we run script

	# Get weather from API
	dAPI = DarkAPI()
	dAPI.getDarkHourData(lat, lng)
	
	# Get the next 8 hours
	next8Hours = []
	for i in range(0,8):
		# Only include  hours outside of quite time
		if dAPI.hourlist[i].time.hour >= qhours[1] or dAPI.hourlist[i].time.hour <= qhours[0]:
			next8Hours.append(dAPI.hourlist[i])

	print 'Hour Range: %d - %d' % (next8Hours[0].time.hour, next8Hours[-1].time.hour)
	print 'Total Hours: %d' % len(next8Hours)

	for hour in next8Hours:
		print '%s - Chance: %.3f' % (hour.time.strftime('%H:%M'), hour.chance)
		# print hour.chance

	result = willItRain(next8Hours);
	return result

def formatMessage(weather):
	pass

def willItRain(hours):
	maxchance = max(hours, key=lambda x: x.chance)
	percentchance = maxchance.chance * 100
	if maxchance.chance > 0.8:
		print '%d%%, %s' % (percentchance, maxchance.condition.capitalize())
		return 1
	elif maxchance.chance > 0.3:
		print '%d%%, %s Likely' % (percentchance, maxchance.condition.capitalize())
		return 2
	# elif maxchance.chance > 0.15:
	# 	print '%d%%, Possible %s' % (percentchance, maxchance.condition.capitalize())
	# 	return 3
	else:
		print '%d%%, No Precipitation' % percentchance
		return 0


w = wNotifier()
print w