#include "SPI.h"
#include "RF22.h"
#include "RTC_DS3234.h"
#include "RTClib.h"
#include "ADXL335.h"
#include "LedStuff.h"
#include "DataTypes.h"


// Integer Values to Hold Used Pins
const int redLED = 9; // PWM
const int greenLED = 6; // PWM
const int blueLED = 5; // PWM

// Initialize the LED object
LED led(redLED, greenLED, blueLED);

// RTC SS pin
const int ssRTC = 8;

// Important Variables
boolean outSideTheHouse = false;
boolean lastplace = false;

// Singleton instance of the radio
// and related global variables
RF22 rf22;
bool checkRF = true; // true so check for rf on bootup

// Flag for checking accelerometer
ADXL335 Accel(0, 1, 2);
bool checkAccel = true;
DateTime movedTime;
int forgetmeminutes = 2;

// Instance of the Real Time Clock
// and related global variables
RTC_DS3234 RTC(ssRTC);
volatile const uint8_t incrementstowait = 1; // seconds (A1), minutes (A2)  
volatile int8_t increments = 0;

////////////////////////////////////////////////////////////////////////////////
// Setup
void setup() {

  // SPI necessary for RTC and Wireless
  SPI.begin();

  // Begin serial output for debugging
  Serial.begin(9600);

  // Initialize the Wireless
  if (!rf22.init()){
    Serial.println("RF22 init failed");
    led.error(); // Indicate an error on the LEDs
    software_Reset(); // Reset the unit if wireless doesn't initialize
  }
  // Initialize clock and set the alarm.
  // Make sure to change the control register in RTC_DS3234.cpp
  // for the appropriate alarm.
  RTC.begin();
  // bool flags[5] = {1, 1, 1, 1, 1}; // Alarm every second
  // RTC.setA1(1, 1, 1, 1, flags); // Alarm 1
  bool flags[4] = {1, 1, 1, 0}; // Alarm every minute
  RTC.setA2(1, 1, 1, flags);  // Alarm 2

  movedTime = RTC.now();

  // Interrupt for the RTC 
  attachInterrupt(1, HandleInterrupt, LOW);

  led.startupSuccess();
}

////////////////////////////////////////////////////////////////////////////////
// Software Reset...
void software_Reset(){
  asm volatile ("  jmp 0");  
} 

////////////////////////////////////////////////////////////////////////////////
// Receive data from wireless function
void HandleInterrupt(){
  RTC.clearflags(); // Clear the alarm flag on the RTC

  increments = increments+1;
  checkAccel = true;
  
  // Set CheckRF flag to true if desired time has elapsed
  if(increments == incrementstowait){
    checkRF = true;
    increments = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Handle the incoming data
Command HandleIncommingData(String dataString){

  String arg;
  char charBuffer[16];

  Command cmd;

  // setTime
  arg = dataString.substring(0, 1);
  arg.toCharArray(charBuffer, 16);
  cmd.setTime = atol(charBuffer);

  // time
  arg = dataString.substring(2, 12);
  arg.toCharArray(charBuffer, 16);
  cmd.time = atol(charBuffer);

  // lightStatus
  arg = dataString.substring(13, 14);
  arg.toCharArray(charBuffer, 16);
  cmd.lightStatus = atol(charBuffer);

  // // r (Red Light)
  // arg = dataString.substring(15, 16);
  // arg.toCharArray(charBuffer, 16);
  // cmd.r = atol(charBuffer);

  return cmd;

}

////////////////////////////////////////////////////////////////////////////////
// Receive data from wireless function
String rx() {
  // Remove Interrupt on the RTC
  detachInterrupt(1);

  // Set string to zero, if no data is received
  // return zero
  String data = "0";

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
     Serial.println("Receiving Data...");
     // Serial.println((char*)buf);
     data = (char*)buf;
  }
  else
  {
     Serial.println("recv 2 failed");
  }

  // Reattach Interrupt on the RTC
  attachInterrupt(1, HandleInterrupt, LOW);

  return data;
}

////////////////////////////////////////////////////////////////////////////////
// Set the Time
void SetTime(Command cmd){
  if(cmd.setTime){
    Serial.println("Setting Time... ");
    RTC.adjust(cmd.time);
  }
  else{
    // Serial.println("Not Setting Time");
  }
}

////////////////////////////////////////////////////////////////////////////////
// If location has just recently changed turn off all lights.
void CheckLocationChange(){
  if(lastplace != outSideTheHouse){
    Serial.println("Changed Location!");

    // Set moved time so lights dont go on immediately after changing location
    // (Dont act like your fogotten when you've only left the house)
    led.changedLocation();
    led.ledState = 0;
    movedTime = RTC.now(); 
  }

  lastplace = outSideTheHouse;
}

////////////////////////////////////////////////////////////////////////////////
// Check for Wireless signal and handle the resulting incoming data
void CheckWireless(){
  Serial.println("Starting Wireless...");
  String data = rx();
  checkRF = false;

  if(data == "0"){
    Serial.println("No Data Received");
    
    // Data WAS NOT recieved... were are outside
    outSideTheHouse = true;
    // Check if location was just changed;
    CheckLocationChange(); 
  }
  else{
    Serial.print("Received: ");
    Serial.println(data);

    // Data was received... were are now inside the house
    outSideTheHouse = false;
    // Check if location was just changed;
    CheckLocationChange(); 

    // Convert data string object to Command Object
    Command cmd = HandleIncommingData(data);

    // Perform actions based on the commands received
    // 0 Off, 1 Rain, 2 Light Rain
    led.ledState = cmd.lightStatus;
    SetTime(cmd);
    
  }
}

////////////////////////////////////////////////////////////////////////////////
// Check for movement of the device and act on the resulting data
void CheckMovement(){
   checkAccel = false;
   bool moved = Accel.CheckForMovement();
   if(moved){
    // checkRF = true;
    movedTime = RTC.now();

    // Turn all LEDs off (Not forgotten)
    led.ledState = 0;

   }
   else{
    DateTime nowTime = RTC.now();
    if(nowTime.unixtime()-movedTime.unixtime() > (forgetmeminutes*60)){
    Serial.println("I've been forgotten!!");

    // Set LED mode to Forgotten pattern
    led.ledState = 3;

    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Home 
void Home(){
  Serial.println("*************************");
  Serial.println("Status: Home");

  // Print the Time
  const int len = 32;
  static char buf[len];
  DateTime now = RTC.now();
  Serial.println(now.toString(buf,len));

  // Continue whatever Mode the LEDs are currently in
  led.continueState();

  // Serial.print("Increments: ");
  // Serial.println(increments);
  // Serial.print("CheckRF: ");
  // Serial.println(checkRF);
  if(checkRF){

    CheckWireless();

  }
  Serial.print("\n");
  delay(1000);
}

////////////////////////////////////////////////////////////////////////////////
// Away
void Away(){
  Serial.println("*************************");
  Serial.println("Status: Away");

  // Print the Time
  const int len = 32;
  static char buf[len];
  DateTime now = RTC.now();
  Serial.println(now.toString(buf, len));

  // Continue whatever Mode the LEDs are currently in
  led.continueState();
  
  // Serial.print("Increments: ");
  // Serial.println(increments);
  // Serial.print("CheckRF: ");
  // Serial.println(checkRF);

  if(checkAccel){

    CheckMovement();
  
  }

  if(checkRF){

    CheckWireless();

  }
  Serial.print("\n");
  
  // If NOT in Forgotten LED mode 
  if(led.ledState != 3)
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