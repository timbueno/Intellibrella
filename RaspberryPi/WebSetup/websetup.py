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

import re
import os
import time
import codecs
import datetime
import calendar
from flask import Flask, request, redirect, url_for, render_template
from WunderAPI import WunderAPI

app = Flask(__name__)

api = WunderAPI()

wireless_info = dict()
sleep_time = dict()
zip_code = dict()

# transmit_process = subprocess.Popen(["python", "/home/pi/intellibrella/test.py"], shell=True,
# 	stdin=None, stdout=None, stderr=None, close_fds=True)

@app.route('/', methods = ['GET', 'POST'])
def setup():
	if request.method == 'POST':
		time.sleep(5)
		# Kill the transmitting script
		# os.system("sudo pkill -f \"/usr/bin/python /home/pi/intellibrella/test.py\"")
		# transmit_process.kill()
		# Get the html form data
		wireless_info['SSID'] = request.form['SSID']
		wireless_info['password'] = request.form['password']
		sleep_time['starttime'] = request.form['starttime']
		sleep_time['endtime'] = request.form['endtime']

		# Get the proper datetime objects for sleep times
		next_quiet_start, next_quiet_end = parse_sleep_time(sleep_time)

		# Check for blank credentials
		if bool([a for a in wireless_info.values() if a == '']) is False:
			try:
				# Write out the wifi config file
				save_wireless_config(wireless_info)
			except:
				pass

		if request.form.get('locationcheck') == None:
			zip_code['zip'] = request.form['ZIP']
			# Check for acceptable zip code
			if re.search(r'^\d{5}?$', zip_code['zip']) == None:
				return redirect(url_for('erroracceptablezip'))
			location_info_dict = api.getCityState(zip_code['zip'])
		else:
			print request.form.get('locationcheck')
			location_info_dict = api.getIPLocation()

		try:
			# Write out the preference config file
			save_prefs(location_info_dict, next_quiet_start, next_quiet_end)
		except:
			return redirect(url_for('errorconfig'))

		# Reboot the wireless
		os.system("sudo ifdown wlan0")
		time.sleep(2)
		os.system("sudo ifup wlan0")

		# Start the transmitting script back up
		# transmit_process = subprocess.Popen(["python", "/home/pi/intellibrella/test.py"], shell=True,
		# 	stdin=None, stdout=None, stderr=None, close_fds=True)

		return redirect(url_for('finished'))

	return render_template('bootstrap.html')

@app.route('/finished')
def finished():
	return render_template('bootstrap.html')

@app.route('/confirm')
def confirm():
	return render_template('bootstrap.html')

@app.route('/errorwireless')
def errorwireless():
	print 'Error! Failed to write wireless configuration file(s)'
	return render_template('error.html')

@app.route('/errorconfig')
def errorconfig():
	print 'Error! Failed to write settings configuration file(s)'
	return render_template('error.html')

@app.route('/erroracceptablezip')
def erroracceptablezip():
	print 'Error! Failed to get acceptable zip code'
	return render_template('error.html')


def save_wireless_config(wireless_info):
	configFile = "/etc/wpa_supplicant/wpa_supplicant.conf"
	f = codecs.open(configFile, 'w', 'utf-8')

	f.write('ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev\n')
	f.write('update_config=1\n\n')
	f.write('network={\n')
	f.write('ssid="' + wireless_info['SSID'] + '"\n')
	f.write('proto=RSN\n')
	f.write('key_mgmt=WPA-PSK\n')
	f.write('pairwise=CCMP TKIP\n')
	f.write('group=CCMP TKIP\n')
	f.write('psk="' + wireless_info['password'] + '"\n')
	f.write('}\n')

	f.close()

	print wireless_info
	return

def save_prefs(location_info_dict, sleep_start, sleep_end):
	configFile = "/home/pi/Intellibrella/RaspberryPi/WirelessWeather/prefs.conf"
	f = codecs.open(configFile, 'w', 'utf-8')

	f.write('city : ' + str(location_info_dict['city']) + '\n')
	f.write('state : ' + str(location_info_dict['state']) + '\n')
	f.write('country : ' + str(location_info_dict['country']) + '\n')
	f.write('jsonurl : ' + str(location_info_dict['jsonurl']) + '\n')
	f.write('sleep_start : ' + str(sleep_start) + '\n')
	f.write('sleep_end : ' + str(sleep_end) + '\n')

	f.close()
	return

def parse_sleep_time(sleep_time):
	starttime = int(sleep_time['starttime'])
	endtime = int(sleep_time['endtime'])
	print starttime
	print endtime

	if starttime > endtime:
		next_quiet_start = datetime.datetime.today().replace(hour=starttime, minute=0)
		next_quiet_end = (datetime.datetime.today() +
						  datetime.timedelta(days=1)).replace(hour=endtime, minute=0)
	else:
		next_quiet_start = datetime.datetime.today().replace(hour=starttime, minute=0)
		next_quiet_end = datetime.datetime.today().replace(hour=endtime, minute=0)

	next_quiet_start = calendar.timegm(next_quiet_start.timetuple())
	next_quiet_end = calendar.timegm(next_quiet_end.timetuple())

	print next_quiet_start
	print next_quiet_end
	
	return next_quiet_start, next_quiet_end


if __name__ == '__main__':
	app.debug = True
	app.run(host='0.0.0.0')
	# app.run()


