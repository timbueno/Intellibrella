from WunderAPI import WunderAPI

# FCT Codes
#-----------------
# SNOW:
# 16: Flurries
# 18: Chance of Snow Showers
# 19: Snow Showers
# 20: Chance of Snow
# 21: Snow
# 24: Blizzard

# RAIN:
# 10: Chance of Showers
# 11: Showers
# 12: Chance of Rain
# 13: Rain
# 14: Chance of Thunderstorm
# 15: Thunderstorm

def wNotifier(jsonURL='US/OH/Cincinnati.json', qhours=(0,6), verbose=0):

	# Get list of weather options to check
	weatherDict = { 0: {'cond': 'No Rain', 'intensity': 0},
				   10: {'cond': 'Chance of Showers', 'intensity': 1},
				   11: {'cond': 'Showers', 'intensity': 1},
				   12: {'cond': 'Chance of Rain', 'intensity': 2},
				   13: {'cond': 'Rain', 'intensity': 2},
				   14: {'cond': 'Chance of Thunderstorm', 'intensity': 3},
				   15: {'cond': 'Thunderstorm', 'intensity': 3}
				   }

	# Initialize the weather api object
	api = WunderAPI()

	# Get the location from the zipcode
	# Have Jay implement this in his code
	# We shouldn't do this every time we want to get the weather
	# Have him write the city, state, and country to a file
	# location = api.getCityState(zipcode)
	api.getWunderHourData(jsonURL)

	# Get the next 8 hours
	next8Hours = []
	for i in range(0,8):
		# Only include hours ouside of quiet time
		if api.list[i].time.hour >= qhours[1] or api.list[i].time.hour <= qhours[0]:
			next8Hours.append(api.list[i])
		else:
			break


	# Get weather ftc codes from all hours in list
	weatherCodes = []
	for hour in next8Hours:
		weatherCodes.append(hour.chance)

	# Find the intersection of the current weather conditions with
	# the weather dictionary
	intersection = set(weatherCodes).intersection(set(weatherDict.keys()))

	# Get the weather of greatest intensity
	if not intersection: # No weather of interest (no rain)
		r = 0
	else:
		r = max(intersection)

	if verbose == 1:
		print '=============='
		print 'Will it rain?'
		print '-------------'
		print 'Location: %s, %s %s' % (location['city'], location['state'], location['country'])
		print 'Times checked: %s - %s' % (next8Hours[0].time.strftime("%I %p"), next8Hours[-1].time.strftime("%I %p"))
		print 'Weather: %s' % weatherDict[r]['cond']
		print 'LED Light Mode: %d' % weatherDict[r]['intensity']
		print '=============='

	return weatherDict[r]['intensity']


if __name__ == "__main__":

	# wNotifier('New_Orleans','LA')
	# wNotifier('Abbeville','LA')
	# wNotifier('Cincinnati','OH')
	# wNotifier('North_Canton','OH')
	# wNotifier('44720') # North Canton
	# wNotifier('45219') # Cincinnati
	# wNotifier('85053') # Phoenix
	wNotifier('US/OH/North_Canton.json') # North Canton
	wNotifier('US/OH/Cincinnati.json') # Cincinnati
	wNotifier('US/AZ/Phoenix.json') # Phoenix


