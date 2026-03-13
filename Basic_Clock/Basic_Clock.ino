#include <LedControl.h>
#include <Wire.h>
#include <RTClib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

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

LedControl timeDisplay = LedControl(DIN, CLK, CS_TIME, 1);
LedControl dateDisplay = LedControl(DIN, CLK, CS_DATE, 1);

RTC_DS3231 rtc;

const char* ssid = "DryIce";
const char* password = "NoRandomCharacters";

WiFiUDP ntpUDP;

// IST offset = 5h30m = 19800 seconds
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800);

bool colon = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  // Run once to set RTC
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected");

  timeClient.begin();
  timeClient.update();

  int hour = timeClient.getHours();
  int minute = timeClient.getMinutes();
  int second = timeClient.getSeconds();

  // get today's date from compile time
  DateTime compileTime(F(__DATE__), F(__TIME__));

  rtc.adjust(DateTime(
    compileTime.year(),
    compileTime.month(),
    compileTime.day(),
    hour,
    minute,
    second));

  Serial.println("RTC synced with NTP");
}

void displayTime(int h, int m, bool colon) {
  timeDisplay.setDigit(0, 0, h / 10, false);
  timeDisplay.setDigit(0, 1, h % 10, colon);

  timeDisplay.setDigit(0, 2, m / 10, colon);
  timeDisplay.setDigit(0, 3, m % 10, false);
}

void displayDate(int d, int m, int y) {
  y = y % 100;

  dateDisplay.setDigit(0, 0, d / 10, false);
  dateDisplay.setDigit(0, 1, d % 10, false);

  dateDisplay.setDigit(0, 2, m / 10, false);
  dateDisplay.setDigit(0, 3, m % 10, false);

  dateDisplay.setDigit(0, 4, y / 10, false);
  dateDisplay.setDigit(0, 5, y % 10, false);
}

void loop() {
  colon = !colon;

  DateTime now = rtc.now();

  displayTime(now.hour(), now.minute(), colon);
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.print(now.second());
  Serial.print(" ");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.year());
  Serial.println();

  displayDate(
    now.day(),
    now.month(),
    now.year());

  delay(1000);
}