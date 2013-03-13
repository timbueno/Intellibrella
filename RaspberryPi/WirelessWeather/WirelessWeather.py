#!/usr/bin/python

# get weather ever so often and send current led state over the air
import time
import calendar
import os

from datetime import datetime
from datetime import timedelta

from wunderNotifier import wNotifier
from PiToArduino import PiToArduino
from readFile import readFileData

setTime = 0

wirelessAPI = PiToArduino()

wInterval = 300 # In Seconds

url = 'US/AZ/Phoenix.json'

if not os.path.exists('/home/pi/Intellibrella/RaspberryPi/WirelessWeather/lock.lock'):
	# Extract data from save file here...
	print 'No lock file... opening file...'
	fileData = readFileData('/home/pi/Intellibrella/RaspberryPi/WirelessWeather/prefs.conf')
	# url = 'US/OH/Cincinnati.json'
	url = fileData['jsonurl']


demomode = 0

weatherIntensity = wNotifier(url)
timeWAcquired = datetime.now()

print '\n=========================='
print 'Retrieved New Weather Data @ %s' % timeWAcquired.strftime("%H:%M:%S")
print 'Weather Intensity: %d' % weatherIntensity
print '=========================='

while 1:

	if not os.path.exists('/home/pi/Intellibrella/RaspberryPi/WirelessWeather/lock.lock'):
		# Extract data from save file here...
		print 'No lock file... opening file...'
		fileData = readFileData('/home/pi/Intellibrella/RaspberryPi/WirelessWeather/prefs.conf')
		# url = 'US/OH/Cincinnati.json'
		url = fileData['jsonurl']

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
	now = calendar.timegm(datetime.now().timetuple())
	msg = '%d,%d,%d,%d' % (setTime, now, weatherIntensity, demomode)
	# setTime = 0

	# Send message to arduino
	print 'Time: %s' % datetime.now()
	print 'Sending Message: %s' % msg
	wirelessAPI.sendWireless(msg)

	time.sleep(1)