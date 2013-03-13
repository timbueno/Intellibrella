#ifndef LEDSTUFF_H
#define LEDSTUFF_H

#include "Arduino.h"

// void setColourRgb(unsigned int red, unsigned int green, unsigned int blue);

class LED {

	public:
		LED(unsigned int redPin, unsigned int greenPin, unsigned int bluePin);
		
		void allOff();

		void rotateColors(int speed);
		void startupSuccess();

		void redToggle(int state);
		void greenToggle(int state);
		void blueToggle(int state);

		void redGreenBlue(int speed);
		void colorPulse(char c, int speed, int maxb);

		void forgotten();
		void rain();
		void lightRain();
		void heavyRain();

		void continueState();
		void changedLocation();

		void error();

		int ledState; // 0 Off, 1 Light Rain; 2 Rain, 3 Heavey Rain,4 Forgotten
		int lastState; // The state of the led the previous time used

	private:
		int _redPin;
		int _greenPin;
		int _bluePin;
};
#endif