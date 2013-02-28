#!/usr/bin/python

import serial
import time

# from datetime import datetime

class PiToArduino(object):
	def __init__(self):
	
		self.ser = serial.Serial("/dev/ttyAMA0",9600)
	
	def sendWireless(self, msg):

		message = ':'+msg+'@'

		# Send message to transmission arduino
		try:
			sent = self.ser.write(message)
			# print "Sent: ", sent
		except:
			print "Problem Sending with Arduino"

		try:
			receive = self.ser.readline()
			# print "Received: ", receive 
		except:
			print "Problem Receiving with Arduino"

		# print "--------------------------\n"
		time.sleep(1)


if __name__ == "__main__":
	
	wireless = PiToArduino()

	try:
		while 1:
			wireless.sendWireless('0,1362086675,3')

	except KeyboardInterrupt:
		print "Quit with KeyboardInterrupt"
		wireless.ser.close()