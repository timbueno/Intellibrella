# References:
#
# http://www.wunderground.com/weather/api/d/docs?d=resources/phrase-glossary
# http://www.wunderground.com/weather/api/d/docs?d=autocomplete-api

import requests
import string
from weatherobjects import HourInformation

# API Superclass
class WunderAPI(object):
	def __init__(self, key='d4a1c5b362656ad6'):
		self.key = key
		self.list = []

	# Get hourly weather data from Wunderground
	# given a city and an abreviated state
	#
	# Returns: complete json data
	def wunderhourly(self, requesturl):
		dataURL = 'http://api.wunderground.com/api/%s/hourly/q/%s' % (self.key, requesturl)
		r = requests.get(dataURL)
		return r.json()

	# Get hourly weather data from Wunderground
	# Extracts relvent data and builds HourInformation objects
	#
	# Returns: List of HourInformation objects
	def getWunderHourData(self, requesturl):
		data = self.wunderhourly(requesturl)
		hourlyList = []
		# Instantiate objects with relevent information
		for hour in data['hourly_forecast']:
			hourlyList.append(HourInformation(  hour['temp']['english'],
												hour['fctcode'],
												hour['condition'],
												hour['FCTTIME']['epoch'])
			)
		self.list = hourlyList

	# Takes Zip Code
	# Returns dictionary with city and state and country and request url
	def getCityState(self, zipcode):
		dataURL = 'http://api.wunderground.com/api/%s/geolookup/q/%s.json' % (self.key, zipcode)
		r = requests.get(dataURL)

		data = r.json()

		location = 0

		if 'error' not in data['response']:
			htmlurl = str(data['location']['requesturl'])
			jsonurl = htmlurl.replace('html','json')

			location = {'city': str(data['location']['city']),
						'state': str(data['location']['state']),
						'country': str(data['location']['country_iso3166']),
						'jsonurl': jsonurl
						}

		return location

	# Automatically gets the locations of the ip address
	def getIPLocation(self):
		dataURL = 'http://api.wunderground.com/api/%s/geolookup/q/autoip.json' % self.key
		r = requests.get(dataURL)

		data = r.json()

		location = 0

		if 'error' not in data['response']:

			htmlurl = str(data['location']['requesturl'])
			jsonurl = htmlurl.replace('html','json')

			location = {'city': str(data['location']['city']),
						'state': str(data['location']['state']),
						'country': str(data['location']['country_iso3166']),
						'jsonurl': jsonurl
						}

		return location

if __name__ == "__main__":

	api = WunderAPI()
	location = api.getCityState('44720')
	print location['jsonurl']

