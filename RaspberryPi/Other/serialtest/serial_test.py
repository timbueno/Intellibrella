#!/usr/bin/python

import serial
import time

from datetime import datetime

# rot13 = string.maketrans( 
#     "ABCDEFGHIJKLMabcdefghijklmNOPQRSTUVWXYZnopqrstuvwxyz", 
#     "NOPQRSTUVWXYZnopqrstuvwxyzABCDEFGHIJKLMabcdefghijklm")
msg = ":4,0123456,5,34325326@"
ser = serial.Serial("/dev/ttyAMA0",9600)
# ser.open()

try:
	while 1 :
		print "Time: ", datetime.now().strftime("%m/%d/%Y %H:%M:%S") 

		# Send message to transmission arduino
		try:
			sent = ser.write(msg)
			print "Sent: ", sent
		except:
			print "Problem Sending with Arduino"

		try:
			receive = ser.readline()
			print "Received: ", receive 
		except:
			print "Problem Receiving with Arduino"

		print "--------------------------"
		time.sleep(1)

    
except KeyboardInterrupt:
	print "Quit with Keyboard Interrupt"
	ser.close()