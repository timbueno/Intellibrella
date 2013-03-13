import time
import calendar
import os

from datetime import datetime
# from datetime import timedelta

# from wunderNotifier import wNotifier
from PiToArduino import PiToArduino
from readFile import readFileData

setTime = 0

wirelessAPI = PiToArduino()

demomode = 1

if not os.path.exists('/home/pi/Intellibrella/RaspberryPi/WebSetup/demo/demo.lock'):
	# Extract data from save file here...
	print 'No lock file... opening file...'
	fileData = readFileData('/home/pi/Intellibrella/RaspberryPi/WebSetup/demo/demo.conf')
	# url = 'US/OH/Cincinnati.json'
	status = int(fileData['status'])

while 1:
	print '\n=========================='
	print 'Demo Mode Demo Mode'
	print '=========================='

	if not os.path.exists('/home/pi/Intellibrella/RaspberryPi/WebSetup/demo/demo.lock'):
		# Extract data from save file here...
		print 'No lock file... opening file...'
		fileData = readFileData('/home/pi/Intellibrella/RaspberryPi/WebSetup/demo/demo.conf')
		# url = 'US/OH/Cincinnati.json'
		status = int(fileData['status'])

	# Build Message to be sent to the umbrella unit
	now = calendar.timegm(datetime.now().timetuple())
	msg = '%d,%d,%d,%d' % (setTime, now, status, demomode)

	# for i in range(0,5):
	# 	# Send message to arduino
	# 	print 'Time: %s' % datetime.now()
	# 	print 'Sending Message: %s' % msg
	# 	wirelessAPI.sendWireless(msg)

	# 	time.sleep(1)

	# Send message to arduino
	print 'Time: %s' % datetime.now()
	print 'Sending Message: %s' % msg
	wirelessAPI.sendWireless(msg)

	# time.sleep(1)

	# # Build Message to be sent to the umbrella unit
	# now = calendar.timegm(datetime.now().timetuple())
	# msg = '%d,%d,%d,%d' % (setTime, now, 4, demomode)

	# for i in range(0,5):
	# 	# Send message to arduino
	# 	print 'Time: %s' % datetime.now()
	# 	print 'Sending Message: %s' % msg
	# 	wirelessAPI.sendWireless(msg)

	# 	time.sleep(1)
