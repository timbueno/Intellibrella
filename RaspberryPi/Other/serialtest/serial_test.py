#!/usr/bin/python

import serial
import time

from datetime import datetime

# rot13 = string.maketrans( 
#     "ABCDEFGHIJKLMabcdefghijklmNOPQRSTUVWXYZnopqrstuvwxyz", 
#     "NOPQRSTUVWXYZnopqrstuvwxyzABCDEFGHIJKLMabcdefghijklm")
# msg = ":4,0123456,5,34325326@"
# msg = ":0,1358904325,1,0,0,1@"

setTime = "0"

lightStatus = "1"
r = "0"
g = "1"
b = "0"

ser = serial.Serial("/dev/ttyAMA0",9600)
# ser.open()

try:
	while 1 :
		now = datetime.now()
		print "Time: ", now.strftime("%m/%d/%Y %H:%M:%S") 

		msg = ":"+setTime+","+now.strftime("%s")+","+lightStatus+","+r+","+g+","+b+"@"

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

		print "--------------------------\n"
		time.sleep(1)

    
except KeyboardInterrupt:
	print "Quit with Keyboard Interrupt"
	ser.close()