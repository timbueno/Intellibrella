import time
import calendar

from datetime import datetime
# from datetime import timedelta

# from wunderNotifier import wNotifier
from PiToArduino import PiToArduino

setTime = 0

wirelessAPI = PiToArduino()

demomode = 1

while 1:
	print '\n=========================='
	print 'Demo Mode Demo Mode'
	print '=========================='

	# Build Message to be sent to the umbrella unit
	now = calendar.timegm(datetime.now().timetuple())
	msg = '%d,%d,%d,%d' % (setTime, now, 1, demomode)

	for i in range(0,5):
		# Send message to arduino
		print 'Time: %s' % datetime.now()
		print 'Sending Message: %s' % msg
		wirelessAPI.sendWireless(msg)

		time.sleep(1)

	# Build Message to be sent to the umbrella unit
	now = calendar.timegm(datetime.now().timetuple())
	msg = '%d,%d,%d,%d' % (setTime, now, 4, demomode)

	for i in range(0,5):
		# Send message to arduino
		print 'Time: %s' % datetime.now()
		print 'Sending Message: %s' % msg
		wirelessAPI.sendWireless(msg)

		time.sleep(1)
