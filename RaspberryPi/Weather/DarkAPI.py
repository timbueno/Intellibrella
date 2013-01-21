import requests
import datetime
from weatherobjects import HourInformation

# API Superclass
class DarkAPI(object):
	def __init__(self, darkkey='c58d8e5492c7d97fd50f84211f1f5948'):
		self.darkkey = darkkey
		self.hourlist = []

	# Hourly Forecast
	# https://developer.darkskyapp.com/docs/forecast
	#
	# Returns next hour at given location as well as next 24 hours
	def darkforecast(self, lat, lng):
		dataURL = 'https://api.darkskyapp.com/v1/forecast/%s/%s,%s' % (self.darkkey, lat, lng)
		r = requests.get(dataURL)
		return r.json()

	# Brief Forecast
	# https://developer.darkskyapp.com/docs/forecast
	#
	# Same as Hourly Forecast, however it omits many datapoints
	def darkbriefforecast(self, lat, lng):
		dataURL = 'https://api.darkskyapp.com/v1/brief_forecast/%s/%s,%s' % (self.darkkey, lat, lng)
		r = requests.get(dataURL)
		return r.json()

	# Currently Interesting Storms
	# https://developer.darkskyapp.com/docs/interesting
	#
	# Returns a list of interesting storms happening right now.
	def darkinteresting(self):
		dataURL = 'https://api.darkskyapp.com/v1/interesting/%s' % self.darkkey
		r = requests.get(dataURL)
		return r.json()

	# Multiple Points and Times
	# https://developer.darkskyapp.com/docs/precipitation
	#
	# coordstimes: (lat, lng, datetimeobject)
	def darkmultiplepoints(self, coordstimes):
		dataURL = 'https://api.darkskyapp.com/v1/precipitation/%s/' % self.darkkey
		for val in coordstimes:
			chunk = '%s,%s,%s;' % (val[0],val[1], val[2].strftime('%s'))
			dataURL += chunk
		dataURL = dataURL[0:-1] # Strip off ending semicolon
		print dataURL

	# Builds a list of hourly weather information using
	# the HourInformation object located in weatherobjects.py
	def getDarkHourData(self, lat, lng):
		data = self.darkforecast(lat, lng)
		theList = []
		for hour in data['dayPrecipitation']:
			theList.append(HourInformation(  hour['temp'],
												hour['probability'],
												hour['type'],
												hour['time'])
			)
		self.hourlist = theList