#include "SPI.h"
#include "RF22.h"
#include "RTC_DS3234.h"
#include "RTClib.h"
#include "ADXL335.h"
#include "DataTypes.h"

// Integer Values to Hold Used Pins
int redLED = 8;
int greenLED = 7;
int blueLED = 6;

int statusLED = 5;

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
// Handle the incoming data
Command HandleIncommingData(String dataString){

  String arg;
  char charBuffer[16];

  Command cmd;

  arg = dataString.substring(0, 1);
  arg.toCharArray(charBuffer, 16);
  cmd.setTime = atol(charBuffer);

  arg = dataString.substring(2, 12);
  arg.toCharArray(charBuffer, 16);
  cmd.time = atol(charBuffer);

  arg = dataString.substring(13, 14);
  arg.toCharArray(charBuffer, 16);
  cmd.lightStatus = atol(charBuffer);

  arg = dataString.substring(15, 16);
  arg.toCharArray(charBuffer, 16);
  cmd.r = atol(charBuffer);

  arg = dataString.substring(17, 18);
  arg.toCharArray(charBuffer, 16);
  cmd.g = atol(charBuffer);

  arg = dataString.substring(19);
  arg.toCharArray(charBuffer, 16);
  cmd.b = atol(charBuffer);

  return cmd;

}

////////////////////////////////////////////////////////////////////////////////
// Receive data from wireless function
String rx() {
  // Remove Interrupt on the RTC
  detachInterrupt(1);

  // String
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
     // outSideTheHouse = false;
     data = (char*)buf;
     // Serial.println(data);


     // Set LED
     // digitalWrite(redLED, HIGH);

     // TODO: Get the time from the real time clock
     // Placeholder:
     savedTime = 1358904325; // About 8:27PM EST
  }
  else
  {
     Serial.println("recv 2 failed");
     // outSideTheHouse = true;
     // Set LED
     // digitalWrite(redLED, LOW);

     savedTime = 1358904325; // About 8:27PM EST
  }

  // Reattach Interrupt on the RTC
  attachInterrupt(1, HandleInterrupt, LOW);

  return data;
}

void ToggleLights(Command cmd){
  if(!cmd.lightStatus){
    // Turn all lights off
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, LOW);
  }
  else{
    if(cmd.r)
      digitalWrite(redLED, HIGH);
    else
      digitalWrite(redLED, LOW);

    if(cmd.g)
      digitalWrite(greenLED, HIGH);
    else
      digitalWrite(greenLED, LOW);

    if(cmd.b)
      digitalWrite(blueLED, HIGH);
    else
      digitalWrite(blueLED, LOW);
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

  // Serial.print("Increments: ");
  // Serial.println(increments);
  // Serial.print("CheckRF: ");
  // Serial.println(checkRF);
  if(checkRF){
    Serial.println("Starting Wireless...");
    String data = rx();
    checkRF = false;

    if(data == "0"){
      Serial.println("No Data Received");
      outSideTheHouse = true;
    }
    else{
      Serial.print("Received: ");
      Serial.println(data);
      Command cmd = HandleIncommingData(data);
      ToggleLights(cmd);
      outSideTheHouse = false;
    }
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

  // Serial.print("Increments: ");
  // Serial.println(increments);
  // Serial.print("CheckRF: ");
  // Serial.println(checkRF);
  if(checkRF){
    Serial.println("Starting Wireless...");
    detachInterrupt(1);
    String data = rx();
    attachInterrupt(1, HandleInterrupt, LOW);
    checkRF = false;

    if(data == "0"){
      Serial.println("No Data Received");
      outSideTheHouse = true;
    }
    else{
      Serial.print("Received: ");
      Serial.println(data);   
      Command cmd = HandleIncommingData(data);
      ToggleLights(cmd);
      outSideTheHouse = false;
    }
  }
  Serial.print("\n");
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