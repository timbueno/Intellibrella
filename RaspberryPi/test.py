# import codecs
# import datetime
# import time
# import serial
# import sys

# ser = serial.Serial("/dev/ttyAMA0",9600)


# # configFile = "testf.txt"
# # f = codecs.open(configFile, 'w', 'utf-8')
# i = 0
# while 1:
# 	try:
# 		sent = ser.write(str(datetime.datetime.today()))
# 		print "1: ", sys.argv[1]
# 		print "2: ", sys.argv[2]
# 		print "3: ", sys.argv[3]
# 	except:
# 		print "Problem Sending with Arduino"
# 	i = i+1


import RPi.GPIO as GPIO
import time
# blinking function
def blink(pin):
    GPIO.output(pin,GPIO.HIGH)
    time.sleep(0.5)
    GPIO.output(pin,GPIO.LOW)
    time.sleep(0.5)
    return

def longblink(pin):
    GPIO.output(pin,GPIO.HIGH)
    time.sleep(5)
    GPIO.output(pin,GPIO.LOW)
    time.sleep(5)
    return
# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BOARD)
# set up GPIO output channel
GPIO.setup(12, GPIO.OUT)
# blink GPIO17 50 times

longblink(12)
longblink(12)

for i in range(0,100):
        blink(12)
GPIO.cleanup()
