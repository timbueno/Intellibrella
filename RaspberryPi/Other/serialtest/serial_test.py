#!/usr/bin/python

import serial
import time

# rot13 = string.maketrans( 
#     "ABCDEFGHIJKLMabcdefghijklmNOPQRSTUVWXYZnopqrstuvwxyz", 
#     "NOPQRSTUVWXYZnopqrstuvwxyzABCDEFGHIJKLMabcdefghijklm")
msg = ":4,0123456,5,34325326@"
ser = serial.Serial("/dev/ttyAMA0",9600)
# ser.open()

try:
	while 1 :
		sent = ser.write(msg)
		print "sent: "
		print sent
		try:
			receive = ser.read(len(msg))
			print "receive: " 
			print receive
		except:
			pass
		time.sleep(1)

    
except KeyboardInterrupt:
	ser.close()