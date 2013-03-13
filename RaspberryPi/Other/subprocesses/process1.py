import envoy
import time

r = envoy.connect('python /Users/timbueno/Projects/Intellibrella/RaspberryPi/Other/subprocesses/process2.py cool')

run = True
count = 0
while run:
	print 'Doing other things\n'
	
	if count == 10:
		r.kill()
		print 'Killed the process...\n'

	if count == 20:
		run = False

	count = count+1

	time.sleep(1)

print 'The program has run its course'

