#include <LedControl.h>
#include "display.h"

#define DIN D8
#define CLK D7
#define CS_TIME D6
#define CS_DATE D5

LedControl timeDisplay = LedControl(DIN, CLK, CS_TIME, 1);
LedControl dateDisplay = LedControl(DIN, CLK, CS_DATE, 1);

byte scrollDot_No = 0;
byte last_h, last_m, last_s;

void prettyPrintTime(int h, int m, int s) {
  if (h != last_h || m != last_m || s != last_s) {
    last_h = h, last_m = m, last_s = s;
    if (h < 10) Serial.print("0");
    Serial.print(h);
    Serial.print(":");
    if (m < 10) Serial.print("0");
    Serial.print(m);
    Serial.print(":");
    if (s < 10) Serial.print("0");
    Serial.print(s);
    Serial.println();
  }
}

void initDisplay() {
  timeDisplay.shutdown(0, false);
  timeDisplay.setIntensity(0, 8);
  timeDisplay.clearDisplay(0);

  dateDisplay.shutdown(0, false);
  dateDisplay.setIntensity(0, 8);
  dateDisplay.clearDisplay(0);
  Serial.println("Display Initialized");
}

void showTime(int h, int m, int s, bool colon) {
  // prettyPrintTime(h, m, s);
  bool am_PM = (h > 11 || h == 0);

  timeDisplay.setDigit(0, 0, h / 10, am_PM);
  timeDisplay.setDigit(0, 1, h % 10, colon);

  timeDisplay.setDigit(0, 2, m / 10, colon);
  timeDisplay.setDigit(0, 3, m % 10, !am_PM);
}
void showDate(int d, int m, int y) {
  y = y % 100;

  dateDisplay.setDigit(0, 0, d / 10, false);
  dateDisplay.setDigit(0, 1, d % 10, false);

  dateDisplay.setDigit(0, 2, m / 10, false);
  dateDisplay.setDigit(0, 3, m % 10, false);

  dateDisplay.setDigit(0, 4, y / 10, false);
  dateDisplay.setDigit(0, 5, y % 10, false);
}

void setBrightness(int level) {
  level = constrain(level, 0, 15);

  timeDisplay.setIntensity(0, level);
  dateDisplay.setIntensity(0, level);
}

void clearTime() {
  for (int i = 0; i < 4; i++)
    timeDisplay.setChar(0, i, ' ', false);
}
void clearDate() {
  for (int i = 0; i < 6; i++)
    dateDisplay.setChar(0, i, ' ', false);
}
void clearTime(int select) {
  select = constrain(select, 0, 1);
  for (int i = select * 2; i < (select * 2) + 2; i++)
    timeDisplay.setChar(0, i, ' ', false);
}
void clearDate(int select) {
  select = constrain(select, 0, 2);
  for (int i = select * 2; i < (select * 2) + 2; i++)
    dateDisplay.setChar(0, i, ' ', false);
}
void showTimeEdit(int h, int m, int blinkDigit, bool blinkState) {
  int digits[4] = {
    h / 10,
    h % 10,
    m / 10,
    m % 10
  };

  for (int i = 0; i < 4; i++) {
    if ((i == blinkDigit || i == blinkDigit + 1) && !blinkState)
      timeDisplay.setChar(0, i, ' ', false);
    else
      timeDisplay.setDigit(0, i, digits[i], false);
  }
}
void showDateEdit(int d, int m, int y, int blinkDigit, bool blinkState) {
  y = y % 100;

  int digits[6] = {
    d / 10,
    d % 10,
    m / 10,
    m % 10,
    y / 10,
    y % 10
  };

  for (int i = 0; i < 6; i++) {
    if ((i == blinkDigit || i == blinkDigit + 1) && !blinkState)
      dateDisplay.setChar(0, i, ' ', false);
    else
      dateDisplay.setDigit(0, i, digits[i], false);
  }
}
void scrollingDot() {
  scrollDot_No++;
  if (scrollDot_No > 5) scrollDot_No = 0;
  switch (scrollDot_No) {
    case 0:
      dateDisplay.setChar(0, 5, ' ', false);
      dateDisplay.setChar(0, 0, '.', false);
      break;
    case 1:
      dateDisplay.setChar(0, 0, ' ', false);
      dateDisplay.setChar(0, 1, '.', false);
      break;
    case 2:
      dateDisplay.setChar(0, 1, ' ', false);
      dateDisplay.setChar(0, 2, '.', false);
      break;
    case 3:
      dateDisplay.setChar(0, 2, ' ', false);
      dateDisplay.setChar(0, 3, '.', false);
      break;
    case 4:
      dateDisplay.setChar(0, 3, ' ', false);
      dateDisplay.setChar(0, 4, '.', false);
      break;
    case 5:
      dateDisplay.setChar(0, 4, ' ', false);
      dateDisplay.setChar(0, 5, '.', false);
      break;
  }
}

void setwifi(){
  // timeDisplay.setChar(0, 0, '')
}