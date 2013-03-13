import time
import calendar
import os
from readFile import readFileData

from datetime import datetime
# from datetime import timedelta

# from wunderNotifier import wNotifier
from PiToArduino import PiToArduino

setTime = 0

wirelessAPI = PiToArduino()

demomode = 1

demostate = 0

while 1:
	print '\n=========================='
	print 'Demo Mode Demo Mode'
	print '=========================='

	if os.path.exists("/home/pi/Intellibrella/RaspberryPi/WebSetup/demo/demo.lock"):
		# Build Message to be sent to the umbrella unit
		now = calendar.timegm(datetime.now().timetuple())
		msg = '%d,%d,%d,%d' % (setTime, now, demostate, demomode)
	
	else:
		pref = readFileData('/home/pi/Intellibrella/RaspberryPi/WebSetup/demo/demo.conf')
		demostate = pref['status']
		# Build Message to be sent to the umbrella unit
		now = calendar.timegm(datetime.now().timetuple())
		msg = '%d,%d,%s,%d' % (setTime, now, demostate, demomode)

		for i in range(0,5):
			# Send message to arduino
			print 'Time: %s' % datetime.now()
			print 'Sending Message: %s' % msg
			wirelessAPI.sendWireless(msg)

			time.sleep(1)

		
