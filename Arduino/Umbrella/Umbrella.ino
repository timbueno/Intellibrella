#include "SPI.h"
#include "RF22.h"
#include "RTC_DS3234.h"
#include "RTClib.h"
#include "ADXL335.h"

// Singleton instance of the radio
RF22 rf22;
boolean outSideTheHouse = false;
int savedTime;

RTC_DS3234 RTC(9);

void setup() {
  
  Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");

  RTC.begin();

// TODO: Run Rx once to check if inside or outside?

}

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
     // TODO: Get the time from the real time clock
     // Placeholder:
     savedTime = 1358904325; // About 8:27PM EST
  }
  else
  {
     Serial.println("recv 2 failed");
     outSideTheHouse = true;
     savedTime = 1358904325; // About 8:27PM EST
  }

}

void Home(){

  // TODO: Replace this with subtraction of "savedTime" from current time
  int wirelessDelay = 10;
  // unsigned long endWirelessDelay = millis() + wirelessDelay;
  DateTime then = RTC.now();
  Serial.println("You are inside the house");
  // unsigned long endWirelessDelay = now.unixtime();
  DateTime now;
  do{
      now = RTC.now();
      delay(100);
      // Serial.println(now.toString(buf,len));
  }while (now.unixtime() - then.unixtime() < wirelessDelay);
  Serial.println("Starting WIRELESS");
  rx();

}

void Away(){
  // const int len = 32;
  // static char buf[len];

  // TODO: Replace this with subtraction of "savedTime" from current time
  int wirelessDelay = 10;
  // unsigned long endWirelessDelay = millis() + wirelessDelay;
  DateTime then = RTC.now();
  // unsigned long endWirelessDelay = now.unixtime();
  Serial.println("You are outside the house");
  DateTime now;
  do{ 
      now = RTC.now();
      delay(100);
      // Serial.println(now.toString(buf,len));
    }while (now.unixtime() - then.unixtime() < wirelessDelay);
  Serial.println("Starting WIRELESS");
  rx();  

}


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