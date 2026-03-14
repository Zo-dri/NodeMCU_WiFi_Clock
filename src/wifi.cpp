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

void updateRTConConnect()
{
  Serial.println("Updating time from NTP");
  if (WiFi.status() == WL_CONNECTED)
  {
    long epoch;

    if (updateNTP(epoch))
    {
      Serial.print("\nEpoch:");
      Serial.println(epoch);
      Serial.println("NTP: Time Updated Successfully.");
      setRTC(epoch);
    }
    Serial.println("RTC: Time Updated Successfully.");
  }
  WiFi.disconnect(true);
  Serial.println("WiFi Disconnected");
}
unsigned long tCaptivePortal;
void initWiFi() {
  tCaptivePortal = millis();

  WiFiManager wm;

  wm.setConnectTimeout(WIFI_TIMEOUT);
  wm.setConfigPortalTimeout(PORTAL_TIMEOUT);

  wm.setConfigPortalBlocking(false);
  bool res = wm.autoConnect("Clock_Setup");

  while (WiFi.status() != WL_CONNECTED && ((millis() - tCaptivePortal) / 1000) < PORTAL_TIMEOUT)
  {
    wm.process();
    delay(200);
    scrollingDot();
  }
  // if (WiFi.status() != WL_CONNECTED) {
  // }

  updateRTConConnect();
  if (!res)
  {
    Serial.println("Restarting Clock");
    ESP.restart();
  }
}
unsigned long tWifiConnect;
void useWiFi()
{
  tWifiConnect = millis();
  WiFiManager wm;

  wm.setConnectTimeout(WIFI_TIMEOUT);
  wm.setConfigPortalTimeout(PORTAL_TIMEOUT);

  wm.setConfigPortalBlocking(false);
  bool res = wm.autoConnect("Clock_Setup");

  while (WiFi.status() != WL_CONNECTED && ((millis() - tWifiConnect) / 1000) < WIFI_TIMEOUT)
  {
    wm.process();
    delay(200);
    scrollingDot();
  }
  updateRTConConnect();
  if (!res)
    return;
}

void resetWifi() {
  WiFiManager wm;
  wm.resetSettings();
  // ESP.restart();
}