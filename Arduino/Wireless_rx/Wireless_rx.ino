#include <SPI.h>
#include <RF22.h>
// Singleton instance of the radio
RF22 rf22;

void setup()  {
  
  Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");
}

void rx() {
  if (!rf22.setFrequency(434.0))
    Serial.println("setFrequency failed");
  if (!rf22.setModemConfig(RF22::GFSK_Rb2Fd5))
    Serial.println("setModemConfig failed");
  
  while (1)
  {
    uint8_t buf[RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf22.recv(buf, &len)) // Should fail, no message available
      Serial.println("recv 1 failed");
    rf22.waitAvailable();
    if (rf22.recv(buf, &len))
    {
       Serial.print("got one in user: ");
       Serial.println((char*)buf);
    }
    else
    {
       Serial.print("recv 2 failed");
    }
  }
}

void loop() {
  rx();
  delay(1000);
}