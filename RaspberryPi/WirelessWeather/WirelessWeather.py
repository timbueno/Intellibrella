# get weather ever so often and send current led state over the air
import time
import calendar

from datetime import datetime
from datetime import timedelta

from wunderNotifier import wNotifier
from PiToArduino import PiToArduino

setTime = 1

wirelessAPI = PiToArduino()

wInterval = 300 # In Seconds

# Extract data from save file here...
url = 'US/OH/Cincinnati.json'

weatherIntensity = wNotifier(url)
timeWAcquired = datetime.now()

print '\n=========================='
print 'Retrieved New Weather Data @ %s' % timeWAcquired.strftime("%H:%M:%S")
print 'Weather Intensity: %d' % weatherIntensity
print '=========================='

while 1:

	# Get new weather data based on the time interval assigned above
	if datetime.now() > timeWAcquired+timedelta(seconds=wInterval):
		weatherIntensity = wNotifier(url)
		timeWAcquired = datetime.now()
		
		print '\n==========================='
		print 'Retrieved New Weather Data @ %s' % timeWAcquired.strftime("%H:%M:%S")
		print 'Weather Intensity: %d' % weatherIntensity
		print '==========================='

	else:
		pass


	# Build Message to be sent to the umbrella unit
	# now = time.mktime(datetime.now().timetuple())
	now = calendar.timegm(datetime.now().timetuple())
	msg = '%d,%d,%d' % (setTime, now, weatherIntensity)
	# setTime = 0

	# Send message to arduino
	print 'Time: %s' % datetime.now()
	print 'Sending Message: %s' % msg
	wirelessAPI.sendWireless(msg)

	time.sleep(1)