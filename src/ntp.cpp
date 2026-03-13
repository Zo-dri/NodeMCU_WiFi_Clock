#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "ntp.h"
#include "display.h"

const char* ssid = "DryIce";
const char* password = "NoRandomCharacters";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800);

void initNTP() {
  Serial.println();

  timeClient.begin();

  Serial.println("NTP Initialized");
}

bool updateNTP(long& epoch) {
  if (timeClient.update()) {
    epoch = timeClient.getEpochTime();
    Serial.print("Time Fetched Successfully");
    return true;
  }

  return false;
}