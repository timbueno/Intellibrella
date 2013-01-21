# References:
#
# http://www.wunderground.com/weather/api/d/docs?d=resources/phrase-glossary
# http://www.wunderground.com/weather/api/d/docs?d=autocomplete-api

import requests
from weatherobjects import HourInformation

# API Superclass
class WunderAPI(object):
	def __init__(self, wunderkey='d4a1c5b362656ad6'):
		self.wunderkey = wunderkey
		self.list = []

	# Get hourly weather data from Wunderground
	# given a city and an abreviated state
	#
	# Returns: complete json data
	def wunderhourly(self, city, state):
		dataURL = 'http://api.wunderground.com/api/%s/hourly/q/%s/%s.json' % (self.wunderkey, state, city)
		r = requests.get(dataURL)
		return r.json()

	# Get hourly weather data from Wunderground
	# Extracts relvent data and builds HourInformation objects
	#
	# Returns: List of HourInformation objects
	def getWunderHourData(self, city, state):
		data = self.wunderhourly(city, state)
		hourlyList = []
		# Instantiate objects with relevent information
		for hour in data['hourly_forecast']:
			hourlyList.append(HourInformation(  hour['temp']['english'],
												hour['fctcode'],
												hour['condition'],
												hour['FCTTIME']['epoch'])
			)
		self.list = hourlyList