#  
#   websetup.py
#
#   Created by: Jay Hickey
#   Contact: http://jayhickey.com
#
#   ABOUT:
#
#   websetup.py is a Wifi configuration script for a Raspberry Pi
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
# 	TODO: Find the propper "except"


import codecs
from flask import Flask, request, redirect, url_for, render_template

app = Flask(__name__)

wireless_info = dict()

@app.route('/', methods = ['GET', 'POST'])
def setup():
	if request.method == 'POST':
		wireless_info['SSID'] = request.form['SSID']
		wireless_info['password'] = request.form['password']
		# Check for blank credentials
		if bool([a for a in wireless_info.values() if a == '']) is True:
			return redirect(url_for('error'))
		save_file(wireless_info)
		try:
		# Write out the wifi config file
			save_file(wireless_info)
		except:
			return redirect(url_for('error'))
		return redirect(url_for('finished'))
	return render_template('login.html')

@app.route('/finished')
def finished():
	return render_template('finished.html')


@app.route('/error')
def error():
	print 'Error! Failed to write configuration file(s)'
	return render_template('error.html')


def save_file(wireless_info):
	configFile = "/Users/johnrhickey/Desktop/log/wpa_supplicant.conf"
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


if __name__ == '__main__':
	app.debug = False
	app.run(host='0.0.0.0')


