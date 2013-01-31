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

	led.forgotten();
	// led.rotateColors(10);
}
