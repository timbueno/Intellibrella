#include "SPI.h"
#include "RF22.h"
#include "RTC_DS3234.h"
#include "RTClib.h"
#include "ADXL335.h"

// Integer Values to Hold Used Pins
int greenLED = 7;
int redLED = 8;
int ssRTC = 9;
int state = HIGH;

// Important Variables
boolean outSideTheHouse = false;
int savedTime;

// Singleton instance of the radio
// and related global variables
RF22 rf22;
bool checkRF = true; // true so check for rf on bootup 

// Instance of the Real Time Clock
// and related global variables
RTC_DS3234 RTC(ssRTC);
volatile const uint8_t incrementstowait = 10; // seconds (A1), minutes (A2)  
volatile int8_t increments = 0;

////////////////////////////////////////////////////////////////////////////////
// Setup
void setup() {

  SPI.begin();

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  
  Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");

  // Initialize clock and set the alarm.
  // Make sure to change the control register in RTC_DS3234.cpp
  // for the appropriate alarm.
  RTC.begin();
  bool flags[5] = {1, 1, 1, 1, 1}; // Alarm every second
  RTC.setA1(1, 1, 1, 1, flags); // Alarm 1
  // bool flags[4] = {1, 1, 1, 0}; // Alarm every minute
  // RTC.setA2(1, 1, 1, flags);  // Alarm 2

  // Interrupt for the RTC 
  attachInterrupt(1, HandleInterrupt, LOW);
}

////////////////////////////////////////////////////////////////////////////////
// Receive data from wireless function
void HandleInterrupt(){
  RTC.clearflags();

  // Toggle light
  // Remove this at some point
  // state = !state;
  // digitalWrite(greenLED, state);


  increments = increments+1;
  
  if(increments == incrementstowait){
    checkRF = true;
    increments = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Receive data from wireless function
void rx() {
  // Wireless timeout in milliseconds
  uint16_t timeout = 3000;

  if (!rf22.setFrequency(434.0))
    Serial.println("setFrequency failed");
  if (!rf22.setModemConfig(RF22::GFSK_Rb2Fd5))
    Serial.println("setModemConfig failed");
  

  uint8_t buf[RF22_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf22.recv(buf, &len)) // Should fail, no message available
    Serial.println("recv 1 failed");
  
  rf22.waitAvailableTimeout(timeout);
  if (rf22.recv(buf, &len))
  {
     Serial.print("got one in user: ");
     Serial.println((char*)buf);
     outSideTheHouse = false;

     // Set LED
     digitalWrite(redLED, HIGH);

     // TODO: Get the time from the real time clock
     // Placeholder:
     savedTime = 1358904325; // About 8:27PM EST
  }
  else
  {
     Serial.println("recv 2 failed");
     outSideTheHouse = true;
     // Set LED
     digitalWrite(redLED, LOW);

     savedTime = 1358904325; // About 8:27PM EST
  }

}

////////////////////////////////////////////////////////////////////////////////
// Home 
void Home(){
  Serial.println("You are inside the house");

  // Print the Time
  const int len = 32;
  static char buf[len];
  DateTime now = RTC.now();
  Serial.println(now.toString(buf,len));

  // Serial.print("Increments: ");
  // Serial.println(increments);
  // Serial.print("CheckRF: ");
  // Serial.println(checkRF);
  if(checkRF){
    Serial.println("Starting WIRELESS");
    detachInterrupt(1);
    rx();
    attachInterrupt(1, HandleInterrupt, LOW);
    checkRF = false;
  }
  delay(1000);
}

////////////////////////////////////////////////////////////////////////////////
// Away
void Away(){
  Serial.println("You are outside the house");

  // Print the Time
  const int len = 32;
  static char buf[len];
  DateTime now = RTC.now();
  Serial.println(now.toString(buf, len));

  // Serial.print("Increments: ");
  // Serial.println(increments);
  // Serial.print("CheckRF: ");
  // Serial.println(checkRF);
  if(checkRF){
    Serial.println("Starting WIRELESS");
    detachInterrupt(1);
    rx();
    attachInterrupt(1, HandleInterrupt, LOW);
    checkRF = false;
  }
  delay(1000);
}

////////////////////////////////////////////////////////////////////////////////
// Loop
void loop() {

  // TODO: Replace with while (!during quiet hours)
  while (1) {

    if (!outSideTheHouse)
    {
      Home();      
    }

    if (outSideTheHouse)
    {
      Away();
    }
  }
}