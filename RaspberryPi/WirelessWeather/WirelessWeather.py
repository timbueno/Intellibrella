# get weather ever so often and send current led state over the air
import time

from datetime import datetime
from datetime import timedelta

from wunderNotifier import wNotifier
from PiToArduino import PiToArduino

zipcode = '45219'
setTime = 0

wirelessAPI = PiToArduino()

wInterval = 60 # In Seconds
weatherIntensity = wNotifier(zipcode)
timeWAcquired = datetime.now()

print '\n=========================='
print 'Retrieved New Weather Data @ %s' % timeWAcquired.strftime("%H:%M:%S")
print 'Weather Intensity: %d' % weatherIntensity
print '=========================='

while 1:

	# Get new weather data based on the time interval assigned above
	if datetime.now() > timeWAcquired+timedelta(seconds=wInterval):
		weatherIntensity = wNotifier(zipcode)
		timeWAcquired = datetime.now()
		
		print '\n==========================='
		print 'Retrieved New Weather Data @ %s' % timeWAcquired.strftime("%H:%M:%S")
		print 'Weather Intensity: %d' % weatherIntensity
		print '==========================='

	else:
		pass


	# Build Message to be sent to the umbrella unit
	now = datetime.now()
	placeholder = 1362086675
	msg = '%d,%d,%d' % (setTime, placeholder, weatherIntensity)

	# Send message to arduino
	print 'Time: %s' % datetime.now()
	print 'Sending message to arduino'
	wirelessAPI.sendWireless(msg)

	time.sleep(1)