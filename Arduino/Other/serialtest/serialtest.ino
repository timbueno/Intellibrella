#include <SPI.h>
#include <RF22.h>

RF22 rf22;
String cmd;
bool cmdRec = false;
bool cmdDone = false;

void setup() {
  //Start the connection with the Raspberry Pi
  Serial.begin(9600);
  // Start the connection with the Laptop, for debugging only!
  //Serial.begin(115200);
  if (!rf22.init())
  	Serial.println("RF22 init failed");
}


void loop() {

  handleCmd();

}

void serialEvent() {
  while(Serial.available() > 0) {
    char inByte = (char)Serial.read();
    if(inByte == ':') {
      cmdRec = true;
      return;
    } else if(inByte == '@') {
      cmdRec = false;
      cmdDone = true;
      return;
    } else {
      if(cmdRec){
        cmd += inByte;
      }
      return;
    }
  }
}

void Transmit(String str) {

  // Create byte array of message string
  uint8_t bytes[str.length() + 1];
  str.getBytes(bytes, str.length()+1);

  // Serial.print("Size: ");
  // Serial.print(sizeof(bytes));
  // Serial.print("\n");

  // Serial.print("Bytes: ");
  // for(int i=0; i<str.length()+1; i++){
  //   Serial.print("[");
  //   Serial.print(bytes[i]);
  //   Serial.print("]");
  // }

  // Serial.println("1");

  // Transmit wireless
  if (!rf22.setFrequency(434.0))
    Serial.print("setFrequency failed");
  if (!rf22.setModemConfig(RF22::GFSK_Rb2Fd5))
    Serial.print("setModemConfig failed");
  rf22.send(bytes, sizeof(bytes));
  rf22.waitPacketSent();
  // Serial.println("-------------");

  // Serial.print("\n");
  // Serial.print("\n");
}

void handleCmd() {

  if(!cmdDone)
    return;
  
  Serial.print("String: ");
  Serial.println(cmd);

  // Transfer the command via wireless here
  Transmit(cmd);

  // Reset command finished flag
  cmdDone = false;
  // Reset command string
  cmd = "";

}