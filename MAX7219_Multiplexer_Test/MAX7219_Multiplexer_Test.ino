#include <LedControl.h>

// SPI pins
// #define DIN D7
// #define CLK D5
#define DIN D8
#define CLK D7

// separate chip selects
// #define CS_TIME D8
// #define CS_DATE D4
#define CS_TIME D6
#define CS_DATE D5

#define buzzer D3

LedControl timeDisplay = LedControl(DIN, CLK, CS_TIME, 1);
LedControl dateDisplay = LedControl(DIN, CLK, CS_DATE, 1);

byte flip = 0;

void setup() {

  pinMode(buzzer, OUTPUT);
      digitalWrite(buzzer, 0);
  timeDisplay.shutdown(0,false);
  timeDisplay.setIntensity(0,8);
  timeDisplay.clearDisplay(0);

  dateDisplay.shutdown(0,false);
  dateDisplay.setIntensity(0,8);
  dateDisplay.clearDisplay(0);
}

void loop() {
  // cycle digits 0-9
  for(int num=0; num<10; num++)
  {
    flip = 1-flip;

    // TIME (4 digits)
    for(int d=0; d<4; d++)
    {
      timeDisplay.setDigit(0,d,num,flip);
      // digitalWrite(buzzer, flip);
    }

    delay(500);
    // DATE (6 digits)
    for(int d=0; d<6; d++)
    {
      dateDisplay.setDigit(0,d,num,!flip);
    }

    delay(500);
  }
}