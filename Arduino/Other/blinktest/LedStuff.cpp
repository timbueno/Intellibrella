#include "LedStuff.h"
#include "Arduino.h"


////////////////////////////////////////////////////////////////////////////////
// Constructor
LED::LED(unsigned int redPin, unsigned int greenPin, unsigned int bluePin){

	// Assigned colors to pins
	_redPin = redPin;
	_greenPin = greenPin;
	_bluePin = bluePin;

	// Set LED pins to OUTPUT
	pinMode(_redPin, OUTPUT);
	pinMode(_greenPin, OUTPUT);
	pinMode(_bluePin, OUTPUT);

	// LEDs OFF
	ledState = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Basic Wrappers around the digitalWrite function
void LED::redToggle(int state){
	digitalWrite(_redPin, state);
}

void LED::greenToggle(int state){
	digitalWrite(_greenPin, state);

}

void LED::blueToggle(int state){
	digitalWrite(_bluePin, state);
}

////////////////////////////////////////////////////////////////////////////////
// Turns on and off all three available colors at full power
void LED::redGreenBlue(int speed){
	redToggle(HIGH);
	delay(speed);
	redToggle(LOW);
	greenToggle(HIGH);
	delay(speed);
	greenToggle(LOW);
	blueToggle(HIGH);
	delay(speed);
	blueToggle(LOW);
}

////////////////////////////////////////////////////////////////////////////////
// Pulse the desired LED at desired speed
void LED::colorPulse(char c, int speed, int maxb){
	
	int color;
	switch(c){
		case 'r':
			color = _redPin;
			break;
		case 'g':
			color = _greenPin;
			break;
		case 'b':
			color = _bluePin;
			break;
		default:
			color = _redPin;
			break;
	}

	analogWrite(color, 0);

	int direction = 1;
	unsigned int intensity = 0;
	for(int e=0; e<=1; e++){
		for(int i = 0; i<maxb; i++){
			if(direction == 1)
				intensity = intensity + 1;
			else
				intensity = intensity - 1;

			analogWrite(color, intensity);
			delay(speed);
		}
		direction = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Rotate through colors (can be looped continuously)
void LED::rotateColors(int speed){
	analogWrite(_redPin, 0);
    analogWrite(_greenPin, 0);
    analogWrite(_bluePin, 0);
 	
 	unsigned int rgbColour[3];
 
  	// Start off with red.
  	rgbColour[0] = 255;
  	rgbColour[1] = 0;
  	rgbColour[2] = 0;  

  	// Choose the colours to increment and decrement.
  	// Ends with red being high
  	for (int decColour = 0; decColour < 3; decColour += 1) {
    	int incColour = decColour == 2 ? 0 : decColour + 1;
 
    	// cross-fade the two colours.
    	for(int i = 0; i < 255; i += 1) {
      		rgbColour[decColour] -= 1;
      		rgbColour[incColour] += 1;
      
 		    // setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
     		analogWrite(_redPin, rgbColour[0]);
      		analogWrite(_greenPin, rgbColour[1]);
      		analogWrite(_bluePin, rgbColour[2]);

 	     	delay(speed);
 	   }
  	}
}

////////////////////////////////////////////////////////////////////////////////
// Rotate through color wheel once and fade to dark (useful in Sucess situations)
void LED::startupSuccess(){

	int redVal = 255;

	rotateColors(1);


	// Fade the currently ON RED to dark
  	for(int i = 255; i > 0; i--){
  		redVal -= 1;

  		analogWrite(_redPin, redVal);
      	// analogWrite(_greenPin, rgbColour[1]);
      	// analogWrite(_bluePin, rgbColour[2]);

 	    delay(1);
  	}	
}

////////////////////////////////////////////////////////////////////////////////
// Turn All LEDs off
void LED::allOff(){
	redToggle(LOW);
	greenToggle(LOW);
	blueToggle(LOW);
}

////////////////////////////////////////////////////////////////////////////////
// Forgotten LED State
void LED::forgotten(){
	int d = 500;

	redToggle(HIGH);
	delay(d);
	redToggle(LOW);
	greenToggle(HIGH);
	delay(d);
	greenToggle(LOW);
}

////////////////////////////////////////////////////////////////////////////////
// Rain LED State
void LED::rain(){
	blueToggle(HIGH);
}

////////////////////////////////////////////////////////////////////////////////
// Light Rain LED State
void LED::lightRain(){
	greenToggle(HIGH);
}

////////////////////////////////////////////////////////////////////////////////
// Changed Location (Used when it is detected the device has changed locals)
void LED::changedLocation(){
	int d = 500;

	blueToggle(HIGH);
	delay(d);
	blueToggle(LOW);
	greenToggle(HIGH);
	delay(d);
	greenToggle(LOW);
}

////////////////////////////////////////////////////////////////////////////////
// Error LED Pattern
void LED::error(){
	
	allOff();

	int d = 250;

	for(int i=0; i<=4; i++){
		redToggle(HIGH);
		delay(d);
		redToggle(LOW);
		delay(d);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Checks the LED state variable and performs current led pattern
// 0 Off, 1 Rain; 2 Light Rain, 3 Forgotten
void LED::continueState(){

	if(ledState == lastState){
		// changedLocation();
		allOff();
	}

	switch (ledState){
		// 
		case 0:
			allOff();
			break;
		case 1:
			rain();
			break;
		case 2:
			lightRain();
			break;
		case 3:
			forgotten();
			break;
	}

	lastState = ledState;
}