// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <avr/pgmspace.h>
#include <SPI.h>
#include "RTClib.h"
#include "RTC_DS3234.h"

////////////////////////////////////////////////////////////////////////////////
// RTC_DS3234 implementation

// Registers we use
const int CONTROL_R = 0x0e;
const int CONTROL_W = 0x8e;
const int CONTROL_STATUS_R = 0x0f;
const int CONTROL_STATUS_W = 0x8f;
const int SECONDS_R = 0x00;
const int SECONDS_W = 0x80;

// Bits we use
const int EOSC = 7;
const int OSF = 7;

uint8_t RTC_DS3234::begin(void)
{
    pinMode(cs_pin,OUTPUT);
    cs(HIGH);
    SPI.setBitOrder(MSBFIRST);

    //Ugh!  In order to get this to interop with other SPI devices,
    //This has to be done in cs()
    //SPI.setDataMode(SPI_MODE1);

    //Enable oscillator, disable square wave, alarms
    cs(LOW);
    SPI.transfer(CONTROL_W);
    // SPI.transfer(0x0); // Enable Oscillator only
    // SPI.transfer(0x06); // Enable Oscillator, Interrupts and Alarm 2
    SPI.transfer(0x05); // Enable Oscillator, Interrupts and Alarm 1
    cs(HIGH);
    delay(1);

    //Clear oscilator stop flag, 32kHz pin
    cs(LOW);
    SPI.transfer(CONTROL_STATUS_W);
    SPI.transfer(0x0);
    cs(HIGH);

    //SPI.setDataMode(SPI_MODE0);
    delay(1);

    return 1;
}

void RTC_DS3234::cs(int _value)
{
    if(_value == 0)
        SPI.setDataMode(SPI_MODE1);
    else
        SPI.setDataMode(SPI_MODE0);
    digitalWrite(cs_pin,_value);
}

void RTC_DS3234::clearflags()
{
    cs(LOW);
    SPI.transfer(CONTROL_STATUS_W);
    SPI.transfer(0x0);
    cs(HIGH);
}

// flags are: A1M1 (seconds), A1M2 (minutes), A1M3 (hour), 
// A1M4 (day) 0 to enable, 1 to disable, DY/DT (dayofweek == 1/dayofmonth == 0)
void RTC_DS3234::setA1(uint8_t s, uint8_t mi, uint8_t h, uint8_t d, bool * flags)
{
    uint8_t t[4] = {s, mi, h, d};
    uint8_t i;

    for(i = 0; i <= 3; i++){
        cs(LOW);
        SPI.transfer(i + 0x87);
        if(i == 3){
            SPI.transfer(bin2bcd(t[3]) | (flags[3] << 7) | (flags[4] << 6));
        }
        else{
            SPI.transfer(bin2bcd(t[i]) | (flags[i] << 7));
        }
        cs(HIGH);
    }
}

// flags are: A2M2 (minutes), A2M3 (hour), A2M4 (day) 0 to enable, 1 to disable, DY/DT (dayofweek == 1/dayofmonth == 0) 
void RTC_DS3234::setA2(uint8_t mi, uint8_t h, uint8_t d, bool * flags)
{
    uint8_t t[3] = {mi, h, d};
    uint8_t i;

    for(i = 0; i<=2; i++){
        cs(LOW);
        SPI.transfer(i + 0x8B);
        if (i == 2){
            SPI.transfer(bin2bcd(t[2]) | (flags[2] << 7) | (flags[3] << 6));
        }
        else{
            SPI.transfer(bin2bcd(t[i]) | (flags[i] << 7));
        }
        cs(HIGH);
    }
}

uint8_t RTC_DS3234::isrunning(void)
{
    cs(LOW);
    SPI.transfer(CONTROL_R);
    uint8_t ss = SPI.transfer(-1);
    cs(HIGH);
    return !(ss & _BV(OSF));
}

void RTC_DS3234::adjust(const DateTime& dt)
{
    cs(LOW);
    SPI.transfer(SECONDS_W);
    SPI.transfer(bin2bcd(dt.second()));
    SPI.transfer(bin2bcd(dt.minute()));
    SPI.transfer(bin2bcd(dt.hour()));
    SPI.transfer(bin2bcd(dt.dayOfWeek()));
    SPI.transfer(bin2bcd(dt.day()));
    SPI.transfer(bin2bcd(dt.month()));
    SPI.transfer(bin2bcd(dt.year() - 2000));
    cs(HIGH);

}

DateTime RTC_DS3234::now()
{
    cs(LOW);
    SPI.transfer(SECONDS_R);
    uint8_t ss = bcd2bin(SPI.transfer(-1) & 0x7F);
    uint8_t mm = bcd2bin(SPI.transfer(-1));
    uint8_t hh = bcd2bin(SPI.transfer(-1));
    SPI.transfer(-1);
    uint8_t d = bcd2bin(SPI.transfer(-1));
    uint8_t m = bcd2bin(SPI.transfer(-1));
    uint16_t y = bcd2bin(SPI.transfer(-1)) + 2000;
    cs(HIGH);

    return DateTime (y, m, d, hh, mm, ss);
}

// vim:ai:cin:sw=4 sts=4 ft=cpp
