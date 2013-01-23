uint8_t bcd2bin (uint8_t val)
{
    return val - 6 * (val >> 4);
}

uint8_t bin2bcd (uint8_t val)
{
    return val + 6 * (val / 10);
}

void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);
}

void loop() {
 	Serial.println(bin2bcd(5));
	Serial.println(bin2bcd(15));
	delay(1000);
}