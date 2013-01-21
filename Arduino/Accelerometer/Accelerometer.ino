#include "ADXL335.h"

ADXL335 Accel(0, 1, 2);

void setup(){
	Serial.begin(9600);
}

void loop(){

	// if(Accel.CheckForStability()){
	// 	Serial.println("Stable");
	// }
	// else{
	// 	Serial.println("Not Stable");
	// }

	if(!Accel.stable){
		Accel.CheckForStability();
	}
	else{
		Accel.CheckForMovement();
	}

	// Serial.println(Accel.AnalogString());
}