#ifndef ADXL335_h
#define ADXL335_h

#include "Arduino.h"

class ADXL335 {
	public:
		ADXL335(int xpin, int ypin, int zpin);
		//~ADXL335();

		// Analog Data Structure
		struct AnalogData{
			int x;
			int y;
			int z;
		};

		// Angles Data Structure
		struct AngleData{
			double xAng;
			double yAng;
			double zAng;
		};

		// Get Analog Data from x, y, z pins
		AnalogData GetAnalog();

		// Get Angle Data from x, y, z pins
		AngleData GetAngles();

		// Useful functions
		bool CheckForStability();
		bool CheckForMovement();

		// For printing analog and angle
		// data to the serial port
		String AnalogString();
		String AnglesString();

		// Stabiliity Variables
		bool stable; // flag
		int xStable; // last known stable ADC value
		int yStable; // last known stable ADC value
		int zStable; // last known stable ADC value


		AnalogData lastread;
		
		// void Stabilize();
		// // Variables that contain the current analog
		// // output of the 3 axis' ADC readings
		// int x;
		// int y;
		// int z;
	private:

		// Analog pin numbers for the associated axis
		int _xpin;
		int _ypin;
		int _zpin;

		// Min and max value returned to the ADC,
		// used for scaling and maping purposes.
		int _minVal;
		int _maxVal;

		// ADC tolerance for stabiliity
		int _jitter;
};

#endif