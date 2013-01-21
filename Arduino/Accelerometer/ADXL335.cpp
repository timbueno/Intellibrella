#include "ADXL335.h"
#include "Arduino.h"

ADXL335::ADXL335(int xpin, int ypin, int zpin){
	_xpin = xpin;
	_ypin = ypin;
	_zpin = zpin;

	_jitter = 10;
	stable = false;

	_minVal = 403;
	_maxVal = 625;
};

ADXL335::AnalogData ADXL335::GetAnalog(){

	AnalogData val;

	val.x = analogRead(_xpin);
	val.y = analogRead(_ypin);
	val.z = analogRead(_zpin);

	return val;
};

bool ADXL335::CheckForStability(){

	// Grab two points of data 1 second apart
	// and compare within _jitter tolerance.
	AnalogData data1 = GetAnalog();
	delay(1000);
	AnalogData data2 = GetAnalog();

	if(data2.x<(data1.x+_jitter) && data2.x>(data1.x-_jitter)){
		if(data2.y<(data1.y+_jitter) && data2.y>(data1.y-_jitter)){
			if(data2.z<(data1.z+_jitter) && data2.z>(data1.z-_jitter)){
				Serial.println("Stable");
				stable = true;
				xStable = data2.x;
				yStable = data2.y;
				zStable = data2.z;
				return true;
			}
		}
	}
	Serial.println("Moving...");
	stable = false;
	return false;
};

bool ADXL335::CheckForMovement(){

	AnalogData data = GetAnalog();

	if(data.x<(xStable+_jitter) && data.x>(xStable-_jitter)){
		if(data.y<(yStable+_jitter) && data.y>(yStable-_jitter)){
			if(data.z<(zStable+_jitter) && data.z>(zStable-_jitter)){
				Serial.println("Did Not Move");
				return false;
			}
		}
	}
	Serial.println("Moved!!");
	stable = false;
	return true;
};

String ADXL335::AnalogString(){
	// Read the analog values from the accelerometer
	AnalogData data = GetAnalog();

	// Return the concatenation as a string
	String temp;
	char dtostrfbuffer[15];
	temp.concat("x: ");
	temp.concat(data.x);
	temp.concat(" | y: ");
	temp.concat(data.y);
	temp.concat(" | z: ");
	temp.concat(data.z);

	return temp;
};

ADXL335::AngleData ADXL335::GetAngles(){

	AngleData val;

	AnalogData data = GetAnalog();

	//convert read values to degrees -90 to 90 - Needed for atan2
	int xmap = map(data.x, _minVal, _maxVal, -90, 90);
	int ymap = map(data.y, _minVal, _maxVal, -90, 90);
	int zmap = map(data.z, _minVal, _maxVal, -90, 90);

	//Caculate 360deg values like so: atan2(-ymap, -zmap)
	//atan2 outputs the value of -π to π (radians)
	//We are then converting the radians to degrees
	val.xAng = RAD_TO_DEG * (atan2(-ymap, -zmap) + PI);
	val.yAng = RAD_TO_DEG * (atan2(-xmap, -zmap) + PI);
	val.zAng = RAD_TO_DEG * (atan2(-ymap, -xmap) + PI);

	return val;
};

String ADXL335::AnglesString(){

	AngleData data = GetAngles();

	// // Read the analog values from the accelerometer
	// AnalogData data = GetAnalog();

	// //convert read values to degrees -90 to 90 - Needed for atan2
	// int xmap = map(data.x, _minVal, _maxVal, -90, 90);
	// int ymap = map(data.y, _minVal, _maxVal, -90, 90);
	// int zmap = map(data.z, _minVal, _maxVal, -90, 90);

	// //Caculate 360deg values like so: atan2(-ymap, -zmap)
	// //atan2 outputs the value of -π to π (radians)
	// //We are then converting the radians to degrees
	// xAng = RAD_TO_DEG * (atan2(-ymap, -zmap) + PI);
	// yAng = RAD_TO_DEG * (atan2(-xmap, -zmap) + PI);
	// zAng = RAD_TO_DEG * (atan2(-ymap, -xmap) + PI);

	//Return the caculations as a string
	String temp;
	char dtostrfbuffer[15];
	temp.concat("x: ");
	temp.concat(dtostrf(data.xAng, 8, 2, dtostrfbuffer));
	temp.concat(" | y: ");
	temp.concat(dtostrf(data.yAng, 8, 2, dtostrfbuffer));
	temp.concat(" | z: ");
	temp.concat(dtostrf(data.zAng, 8, 2, dtostrfbuffer));

	return temp;
};

// void ADXL335::GetAnalog(){
// 	// Read the analog values from the axccelerometer
// 	x = analogRead(_xpin);
// 	y = analogRead(_ypin);
// 	z = analogRead(_zpin);

// 	return;
// };

// void ADXL335::Stabilize(){

// 	// Set timeout to stabilization
// 	int timeout = 2; // 0.1 seconds

// 	// Get fresh analog data
// 	GetAnalog();

// 	// Check for stability against stored stable values
// 	bool flag = false;
// 	if(x<(xStable+_jitter) && x>(xStable-_jitter)){
// 		if(y<(yStable+_jitter) && y>(yStable-_jitter)){
// 			if(z<(zStable+_jitter) && z>(zStable-_jitter)){
// 				Serial.println("Already Stable");
// 				stable = true;
// 				flag = true;
// 			}
// 		}
// 	}

// 	// If not stable, begin stability loop
// 	int t = 0;
// 	// Wait for stability
// 	while(!flag){
// 		Serial.println("Not Stable");
// 		// Wait a second to stablize
// 		delay(1000);

// 		int xTemp = analogRead(_xpin);
// 		int yTemp = analogRead(_ypin);
// 		int zTemp = analogRead(_zpin);

// 		if(xTemp<(x+_jitter) && xTemp>(x-_jitter)){
// 			if(yTemp<(y+_jitter) && yTemp>(y-_jitter)){
// 				if(zTemp<(z+_jitter) && zTemp>(z-_jitter)){
// 					xStable = xTemp;
// 					yStable = yTemp;
// 					zStable = zTemp;
// 					stable = true;
// 					flag = true;
// 					Serial.println("Stablized!!!");
// 				}
// 			}
// 		}

// 		x = xTemp;
// 		y = yTemp;
// 		z = zTemp;

// 		if(t==timeout){
// 			flag = true;
// 			Serial.println("Timeout: Stability Function");
// 		}
// 		else{
// 			t++;
// 		}
// 	}
// };