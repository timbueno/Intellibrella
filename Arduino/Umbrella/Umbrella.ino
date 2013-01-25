#include "SPI.h"
#include "RF22.h"
#include "RTC_DS3234.h"
#include "RTClib.h"
#include "ADXL335.h"

// Integer Values to Hold Used Pins
int greenLED = 7;
int redLED = 8;
int ssRTC = 9;

// Important Variables
boolean outSideTheHouse = false;
int savedTime;

// Singleton instance of the radio
// and related global variabls
RF22 rf22;
bool checkRF = false; // set inside interrupt 

// Instance of the Real Time Clock
// and related global variables
RTC_DS3234 RTC(ssRTC);
volatile const uint8_t incrementstowait = 10; // seconds (A1), minutes (A2)  
volatile uint8_t increments = 0;

////////////////////////////////////////////////////////////////////////////////
// Setup
void setup() {
  SPI.begin();

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  
  Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");

  // Initialize clock and set the timer
  RTC.begin();
  bool flags[5] = {1, 1, 1, 1, 1}; // Alarm every second
  RTC.setA1(1, 1, 1, 1, flags); // Alarm 1
  // bool flags[4] = {1, 1, 1, 0}; // Alarm every minute
  // RTC.setA2(1, 1, 1, flags);  // Alarm 2
  attachInterrupt(1, HandleInterrupt, LOW);


// TODO: Run Rx once to check if inside or outside?

}

////////////////////////////////////////////////////////////////////////////////
// Receive data from wireless function
void HandleInterrupt(){
  // SPI.begin();
  RTC.clearflags();
  if(increments == incrementstowait){
    checkRF = true;
    increments = 1;
  }
  else{
    increments = increments+1;
    checkRF = false;
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
  const int len = 32;
  static char buf[len];

  // TODO: Replace this with subtraction of "savedTime" from current time
  // int wirelessDelay = 10;
  // unsigned long endWirelessDelay = millis() + wirelessDelay;
  // DateTime then = RTC.now();
  // Serial.println("You are inside the house");

  // unsigned long endWirelessDelay = now.unixtime();
  // DateTime now;
  // do{
  //     now = RTC.now();
  //     delay(100);
  //     // Serial.println(now.toString(buf,len));
  // }while (now.unixtime() - then.unixtime() < wirelessDelay);
  // Serial.println("Starting WIRELESS");
  // rx();

  Serial.println("You are inside the house");
  DateTime now = RTC.now();
  Serial.println(now.toString(buf,len));
      
  if(checkRF){
    Serial.println("Starting WIRELESS");
    rx();
    checkRF = false;
  }

  delay(1000);
}

////////////////////////////////////////////////////////////////////////////////
// Away
void Away(){
  const int len = 32;
  static char buf[len];

  // // TODO: Replace this with subtraction of "savedTime" from current time
  // int wirelessDelay = 10;
  // // unsigned long endWirelessDelay = millis() + wirelessDelay;
  // DateTime then = RTC.now();
  // // unsigned long endWirelessDelay = now.unixtime();
  // Serial.println("You are outside the house");
  // DateTime now;
  // do{ 
  //     now = RTC.now();
  //     delay(100);
  //     // Serial.println(now.toString(buf,len));
  //   }while (now.unixtime() - then.unixtime() < wirelessDelay);
  // Serial.println("Starting WIRELESS");
  // rx();  

  Serial.println("You are outside the house");
  DateTime now = RTC.now();
  Serial.println(now.toString(buf, len));

  if(checkRF){
    Serial.println("Starting WIRELESS");
    rx();
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
      // const int len = 32;
      // static char buf[len];

      // Print current time
      // DateTime now = RTC.now();
      // Serial.println(now.toString(buf,len));

      // TODO: Replace this with subtraction of "savedTime" from current time
      // int wirelessDelay = 10;
      // // unsigned long endWirelessDelay = millis() + wirelessDelay;
      // DateTime then = RTC.now();
      // Serial.println("You are inside the house");
      // // unsigned long endWirelessDelay = now.unixtime();
      // DateTime now;
      // do
      // { 
      //   now = RTC.now();
      //   // Serial.println(now.toString(buf,len));
      // }while (now.unixtime() - then.unixtime() < wirelessDelay);
      // Serial.println("Starting WIRELESS");
      // rx();
    }

    if (outSideTheHouse)
    {
      Away();
      // const int len = 32;
      // static char buf[len];

      // TODO: Replace this with subtraction of "savedTime" from current time
      // int wirelessDelay = 10;
      // // unsigned long endWirelessDelay = millis() + wirelessDelay;
      // DateTime then = RTC.now();
      // // unsigned long endWirelessDelay = now.unixtime();
      // Serial.println("You are outside the house");
      // DateTime now;
      // do 
      // { 
      //   now = RTC.now();
      //   // Serial.println(now.toString(buf,len));
      // }while (now.unixtime() - then.unixtime() < wirelessDelay);
      // Serial.println("Starting WIRELESS");
      // rx();
    }
  }
}