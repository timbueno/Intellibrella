#include <SPI.h>
#include <RF22.h>
// Singleton instance of the radio
RF22 rf22;
boolean checkForNewData = true;
int time

void setup()  {
  
  Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");
}

void rx() {
  // Wireless timeout in milliseconds
  uint16_t timeout = 10000;

  if (!rf22.setFrequency(434.0))
    Serial.println("setFrequency failed");
  if (!rf22.setModemConfig(RF22::GFSK_Rb2Fd5))
    Serial.println("setModemConfig failed");
  

  uint8_t buf[RF22_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf22.recv(buf, &len)) // Should fail, no message available
    Serial.println("recv 1 failed");
  
  rf22.waitAvailableTimeout(timeout);
  
  if (rf22.recv(buf, &len) && checkForNewData)
  {
     Serial.print("got one in user: ");
     Serial.println((char*)buf);
     checkForNewData = false;
     // GET THE TIME FOR WHEN THE DATA WAS RECIEVED
     // Placeholder:
     time = 1358904325; // About 8:27PM EST
  }
  else
  {
     Serial.println("recv 2 failed");
  }

}


void loop() {

  // While not during quiet hours
  while (1) {

    if (checkForNewData = false)
    {
      int wirelessDelay = 10000;
      unsigned long endWirelessDelay = millis() + wirelessDelay;
      while (millis() < endWirelessDelay)
      {
        // Main 
      }

    }

    checkForNewData = true;
    rx();

  // {

  // delay using millis()

}