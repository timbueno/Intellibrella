#include "SPI.h"
#include "RTC_DS3234.h"
#include "RTClib.h"

int ledPin = 8;

int wirelesspin = 10;
int ssRTC = 9;

int state = LOW;

volatile const uint8_t incrementstowait = 5;
volatile uint8_t increments = 0;
bool doitflag = false;

int test = 0;

RTC_DS3234 RTC(ssRTC);

void setup() {
	SPI.begin();

	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);
	pinMode(wirelesspin, OUTPUT);
	digitalWrite(wirelesspin, HIGH);


	// attachInterrupt(1, HandleInterrupt, LOW);
	// attachInterrupt(1, blink, LOW);
	// attachInterrupt(1, doNothing, LOW);
	// attachInterrupt(1, HandleInterrupt3, LOW);

  // put your setup code here, to run once:
	Serial.begin(9600);

	RTC.begin();

	// bool flags[4] = {1, 1, 1, 0};
	// RTC.setA2(1, 1, 1, flags);

	bool flags[5] = {1, 1, 1, 1, 1};
	RTC.setA1(1, 1, 1, 1, flags);

	attachInterrupt(1, HandleInterrupt4, LOW);
}

void doNothing(){
	//
	RTC.clearflags();
}

void HandleInterrupt(){
	RTC.clearflags();
	if(increments == incrementstowait){
		increments = 1;
		// doitflag = true;
		state = !state;
	}
	else{
		increments++;
		// doitflag = false;
	}
}

void HandleInterrupt2(){
	RTC.clearflags();
	if(increments == incrementstowait){
		increments = 1;
		doitflag = true;
		// state = !state;
	}
	else{
		increments++;
		doitflag = false;
	}
}

void HandleInterrupt3(){
	// SPI.begin();
	RTC.clearflags();
	// doitflag = !doitflag;
	state = !state;
}

void HandleInterrupt4(){
	// SPI.begin();
	RTC.clearflags();
	if(increments == incrementstowait){
		state = true;
		increments = 1;
	}
	else{
		increments = increments+1;
		state = false;
	}
}

void blink()
{
	SPI.begin();
	RTC.clearflags();
	state = !state;
}

void loop() {

	const int len = 32;
    static char buf[len];

    // digitalWrite(ledPin, HIGH);
    // delay(1000);
    // digitalWrite(ledPin, LOW);
    // delay(1000);

	// DateTime now = RTC.now();
	// Serial.println(now.toString(buf, len));
	// delay(10);

	digitalWrite(ledPin, state);

	// if(doitflag){
	// 	state = !state;
	// 	digitalWrite(ledPin, state);
	// 	Serial.println("DoItFlag: True");
	// }else{
	// 	Serial.println("DoItFlag: False");
	// };
	
	// delay(500);

}