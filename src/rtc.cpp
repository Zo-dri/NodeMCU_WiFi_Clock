#include <Wire.h>
#include <RTClib.h>
#include "display.h"
#include "rtc.h"

RTC_DS3231 rtc;

void initRTC() {
  Wire.begin();
  rtc.begin();
  Serial.println("RTC Initialized");
}

void getTime(int &h, int &m, int &s) {
  DateTime now = rtc.now();
  prettyPrintTime(now.hour(), now.minute(), now.second());

  h = now.hour();
  m = now.minute();
  s = now.second();
}

void getDate(int &d, int &m, int &y) {
  DateTime now = rtc.now();
  prettyPrintDate(now.day(), now.month(), now.year());

  d = now.day();
  m = now.month();
  y = now.year();
}

void setRTC(int epoch) {
  rtc.adjust(DateTime(epoch));
}
