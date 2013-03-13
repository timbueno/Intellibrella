import time
import sys

from datetime import datetime

def coolProcess(userInput):
	while 1:
		# Append text to file, repeat
		f = open("test.txt", "a")
		text = '%s - %s\n' % (datetime.now().strftime('%I:%M:%S'), userInput)
		f.write(text)
		f.close()

		time.sleep(1)

if __name__ == "__main__":
	
	userInput = sys.argv[1]
	coolProcess(userInput)