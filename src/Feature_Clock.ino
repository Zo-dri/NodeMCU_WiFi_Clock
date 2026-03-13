#include <Arduino.h>
#include "buttons.h"
#include "display.h"
#include "menu.h"
#include "ntp.h"
#include "rtc.h"
#include "tasks.h"
#include "wifi.h"

void setup() {
  Serial.begin(115200);
  Serial.println();

  initDisplay();
  initRTC();
  // initWiFi();
  initNTP();
  initMenu();

  long epoch;

  if (updateNTP(epoch)) {
    setRTC(epoch);
  }
}

void loop() {
  String stateString = getStatePretty();
  if (stateString != "UNCHANGED")
  {
    Serial.print("State:");
    Serial.println(stateString);
  }
  runRTCTask();
  runDisplayTask();
  runBrightnessTask();
  runButtonTask();
  runMenu();
  runNTPTask();
}