#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include "core_esp8266_features.h"
#include "display.h"
#include "HardwareSerial.h"
#include "ntp.h"
#include "rtc.h"
#include "wl_definitions.h"

#define WIFI_TIMEOUT  10
#define WIFI_RETRIES 2
#define PORTAL_TIMEOUT 180

bool updateRTConConnect()
{
  WiFiManager wm;
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
  else
  {
    Serial.print("Wifi not connected");
    return false;
  }
  // WiFi.disconnect(true);
  wm.disconnect();
  Serial.println("WiFi Disconnected");
  return true;
}
unsigned long tCaptivePortal;
bool initWiFi() {
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

  return updateRTConConnect();
  // Serial.println("Stopping AP and Portal");
  // wm.stopConfigPortal();
  // Serial.println("Stopping portal");
  // wm.stopWebPortal();
  if (!res)
  {
    Serial.println("Restarting Clock");
    ESP.restart();
  }
}
unsigned long tWifiConnect;
bool useWiFi()
{
  tWifiConnect = millis();
  WiFiManager wm;

  wm.setConnectTimeout(WIFI_TIMEOUT);
  wm.setConfigPortalTimeout(PORTAL_TIMEOUT);

  wm.setConfigPortalBlocking(false);
  wm.setDisableConfigPortal(true);
  wm.setConnectRetries(WIFI_RETRIES);
  bool res = wm.autoConnect("Clock_Setup");

  while (WiFi.status() != WL_CONNECTED && ((millis() - tWifiConnect) / 1000) < WIFI_TIMEOUT)
  {
    wm.process();
    delay(200);
    scrollingDot();
  }
  return updateRTConConnect();
}

void resetWifi() {
  WiFiManager wm;
  wm.resetSettings();
  // ESP.restart();
}

bool isWifiSaved()
{
  WiFiManager wm;
  return wm.getWiFiIsSaved();
}
