#
# TODO
# Build in 'relevent hours'
# maybe 7am - 10pm
# 
# maybe just next 12 hours? 
# both?
#

import datetime

# Hourly information from json data
class HourInformation(object):
	def __init__(self, temp, chance, condition, time):
		self.temp = float(temp)
		self.chance = float(chance)
		self.condition = condition
		self.time = datetime.datetime.fromtimestamp(float(time))
		self.timeAcquired = datetime.datetime.now()

	# Converts stored F temperatures to C
	#
	# Returns: Temp in Celcius (float)
	def tempC(self):
		return (float(5)/9)*(self.temp - 32)

# Day weather comprised of initialized day hourly weather
class DayWeather(object):
	def __init__(self, day, hourlyList):
		self.day = day.replace(hour=23, minute=59, second=59)

		self.hours = []
		for hour in hourlyList:
			if hour.time.day == self.day.day:
				if hour.time < self.day:
					self.hours.append(hour)

		self.relhours = []

	# Analyzes precipitation chance data
	def willItRain(self, hours):
		maxchance = max(hours, key=lambda x: x.chance)
		percentchance = maxchance.chance * 100
		if maxchance.chance > 0.8:
			return '%d%%, %s' % (percentchance, maxchance.condition.capitalize())
		elif maxchance.chance > 0.5:
			return '%d%%, %s Likely' % (percentchance, maxchance.condition.capitalize())
		elif maxchance.chance > 0.15:
			return '%d%%, Possible %s' % (percentchance, maxchance.condition.capitalize())
		else:
			return '%d%%, No Precipitation' % percentchance

	def betweentimes(self, lowtime, hightime):
		theList = []
		for hour in self.hours:
			if hour.time > lowtime and hour.time < hightime:
				theList.append(hour)
		return theList

	def releventhours(self):
		lowtime = self.day.replace(hour=7, minute=0, second=0, microsecond=0) # self.day @ 7 am
		hightime = self.day.replace(hour=22, minute=0, second=0, microsecond=0) # self.day @ 10 am

		self.relhours = self.betweentimes(lowtime, hightime)




