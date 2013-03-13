#  
#   websetup.py
#
#   ABOUT:
#
#   Wifi configuration script for a Raspberry Pi
#
#	This script assumes /etc/network/interfaces already looks like this:
# 		
#		auto lo		
#		iface lo inet loopback
#		iface eth0 inet dhcp
#		
#		auto wlan0
#		allow-hotplug wlan0
#		iface wlan0 inet manual
#		wpa-roam /etc/wpa_supplicant/wap_supplicant.conf
#		iface default inet dhcp
#   
#
#
#
import time
import codecs
import os
from flask import Flask, request, redirect, url_for, render_template


app = Flask(__name__)


@app.route('/', methods = ['GET', 'POST'])
def setup():
	if request.method == 'POST':
		i = 0
		while(i < 5):
			print "Starting..."
			time.sleep(1)
			i = i+1
		return redirect(url_for('demo0'))

	return render_template('start.html')

@app.route('/demo0', methods = ['GET', 'POST'])
def demo0():
	save_demo_state('0')
	# if request.method == 'POST':
		# save_demo_state('4')
		# i = 0
		# while(i < 5):
		# 	print "I'm in demo 4"
		# 	time.sleep(3)
		# 	i = i+1
		# return redirect(url_for('demo1'))
	return render_template('demo0.html')


@app.route('/demo1', methods = ['GET', 'POST'])
def demo1():
	save_demo_state('1')
	# if request.method == 'POST':
		# save_demo_state('1')
		# i = 0
		# while(i < 5):
		# 	print "I'm in demo 1"
		# 	time.sleep(3)
		# 	i = i+1
		# return redirect(url_for('demo2'))
	return render_template('demo1.html')

@app.route('/demo2', methods = ['GET', 'POST'])
def demo2():
	save_demo_state('2')
	# if request.method == 'POST':
		# save_demo_state('2')
		# i = 0
		# while(i < 5):
		# 	print "I'm in demo2"
		# 	time.sleep(3)
		# 	i = i+1
		# return redirect(url_for('demo3'))
	return render_template('demo2.html')

@app.route('/demo3', methods = ['GET', 'POST'])
def demo3():
	save_demo_state('3')
	# if request.method == 'POST':
		# save_demo_state('3')
		# i = 0
		# while(i < 5):
		# 	print "I'm in demo3"
		# 	time.sleep(3)
		# 	i = i+1
		# return redirect(url_for('demo4'))
	return render_template('demo3.html')

@app.route('/demo4', methods = ['GET', 'POST'])
def demo4():
	save_demo_state('4')
	# if request.method == 'POST':
		# save_demo_state('4')
		# i = 0
		# while(i < 5):
		# 	print "I'm in demo 4"
		# 	time.sleep(3)
		# 	i = i+1
		# return redirect(url_for('demo1'))
	return render_template('demo4.html')



def save_demo_state(state_no):
	# Create a lock file
	lockFile = "demo.lock"
	lockf = codecs.open(lockFile, 'w', 'utf-8')

	configFile = "demo.conf"
	f = codecs.open(configFile, 'w', 'utf-8')

	f.write('state : ' + str(state_no) + '\n')

	f.close()

	lockf.close()
	os.remove(lockFile)

	return




if __name__ == '__main__':
	app.debug = True
	app.run(host='0.0.0.0')
	# app.run()


