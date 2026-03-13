#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include "core_esp8266_features.h"
#include "display.h"
#include "HardwareSerial.h"
#include "ntp.h"
#include "rtc.h"
#include "wl_definitions.h"

#define WIFI_TIMEOUT  10
#define PORTAL_TIMEOUT 180

void initWiFi() {
  WiFiManager wm;

  wm.setConnectTimeout(WIFI_TIMEOUT);
  wm.setConfigPortalTimeout(PORTAL_TIMEOUT);

  wm.setConfigPortalBlocking(false);
  bool res = wm.autoConnect("Clock_Setup");

  while (WiFi.status() != WL_CONNECTED && millis()/1000 < PORTAL_TIMEOUT) {
    wm.process();
    delay(200);
    scrollingDot();
  }
  // if (WiFi.status() != WL_CONNECTED) {
  // }

  if (!res)
  {
    Serial.println("Restarting Clock");
    ESP.restart();
  }
}

void useWiFi()
{
  WiFiManager wm;

  wm.setConnectTimeout(WIFI_TIMEOUT);
  wm.setConfigPortalTimeout(PORTAL_TIMEOUT);

  wm.setConfigPortalBlocking(false);
  bool res = wm.autoConnect("Clock_Setup");

  while (WiFi.status() != WL_CONNECTED && (millis() / 1000) < WIFI_TIMEOUT)
  {
    wm.process();
    delay(200);
    scrollingDot();
  }
  if (!res) return;
  
  if (WiFi.status() == WL_CONNECTED)
  {
    long epoch;

    if (updateNTP(epoch))
      setRTC(epoch);
  }
}

void resetWifi() {
  WiFiManager wm;
  wm.resetSettings();
  ESP.restart();
}