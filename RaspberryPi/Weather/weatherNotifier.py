#
# TODO
#
#

import re
import datetime

from pygeocoder import Geocoder
from DarkAPI import DarkAPI
from weatherobjects import DayWeather


# Class describing a city and its weather
# Subclass to API class
# Contains "Day" weather objects that contain "HourlyInformation" objects
class City(DarkAPI):
	def __init__(self, city):
		super(City, self).__init__()

		# Get geocode from google
		# Currently does not detect duplicate cities
		# and is currently USA only
		self.geocode = Geocoder.geocode(city)
		self.city, self.state, self.country = str(self.geocode[0]).split(', ')
		# Fix self.state (it may contain postal code as well)
		self.state = re.search(r'[A-Z][A-Z]', self.state).group()

		self.lat, self.lng = self.geocode[0].coordinates
		self.postalcode = self.geocode[0].postal_code

		self.days = [] # List of days for the city

	# # Updates self.hourlist object containing hourly data
	# # for Wunderground
	# def getWunderHourly(self):
	# 	self.getWunderHourData(self.city, self.state)

	# Updates self.hourlist object containing hourly data
	# for DarkSky
	def getDarkHourly(self):
		self.getDarkHourData(self.lat, self.lng)

	# Adds a day to the self.days list
	def addDay(self, day):
		self.days.append(DayWeather(day, self.hourlist))

	# Convenient wrapper of addDay for Today
	def addToday(self):
		self.addDay(datetime.datetime.today())

	# Convenient wrapper of addDay for Tomorrow
	def addTomorrow(self):
		self.addDay(datetime.datetime.today()+datetime.timedelta(days=1))

#
##### Script Start #####
#
cincinnati = '45219'
city = City(cincinnati)
city.getDarkHourly()
city.addToday()
city.addTomorrow()
city.days[0].releventhours()
city.days[1].releventhours()
print '************************'
print 'City: ', city.city
print 'State: ', city.state
print 'Postal Code: ', city.postalcode
print 'Lat: %s, Long: %s' % (city.lat, city.lng)
print 'Chance of Rain Today: %s' % (city.days[0].willItRain(city.days[0].relhours))
print 'Chance of Rain Tomorrow: %s' % city.days[1].willItRain(city.days[1].relhours)
print '************************'

atlanta = '30301'
otherCity = City(atlanta)
otherCity.getDarkHourly()
otherCity.addToday()
otherCity.addTomorrow()
otherCity.days[0].releventhours()
otherCity.days[1].releventhours()
print '************************'
print 'City: ', otherCity.city
print 'State: ', otherCity.state
print 'Postal Code: ', otherCity.postalcode
print 'Lat: %s, Long: %s' % (otherCity.lat, otherCity.lng)
print 'Chance of Rain Today: %s' % otherCity.days[0].willItRain(otherCity.days[0].relhours)
print 'Chance of Rain Tomorrow: %s' % otherCity.days[1].willItRain(otherCity.days[1].relhours)
print '************************'
