#include "LedStuff.h"

const int redPin = 9;
const int greenPin = 6;
const int bluePin = 5;

LED led(redPin, greenPin, bluePin);

bool flag = true;

void setup() {

	led.startupSuccess();
	delay(1000);
	led.error();
	delay(1000);
}
 
void loop() {

	// led.forgotten();
	// led.redGreenBlue(500);
	led.colorPulse('g',10, 100);
	led.colorPulse('b',10, 100);
	// led.rotateColors(10);
}
