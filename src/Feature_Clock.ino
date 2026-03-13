#include <Arduino.h>
#include "display.h"
#include "rtc.h"
#include "ntp.h"
#include "wifi.h"
#include "buttons.h"
#include "menu.h"

unsigned long tRTC = 0;
unsigned long tDisplay = 0;
unsigned long tButtons = 0;
unsigned long tNTP = 0;

int h, m, s;
int d, mo, y;
bool colon = false;

void runRTCTask() {
  if (millis() - tRTC >= 1000) {
    tRTC = millis();

    getTime(h, m, s);
    getDate(d, mo, y);

    colon = !colon;
  }
}

void runDisplayTask() {
  MenuState state = getState();
  if (state == CLOCK_VIEW) {
    if (millis() - tDisplay >= 200) {
      tDisplay = millis();

      Serial.println("Printing Time");

      showTime(h, m, s, colon);
      showDate(d, mo, y);
    }
  }
}

void runNTPTask() {
  if (millis() - tNTP >= 21600000) {
    tNTP = millis();

    long epoch;

    if (updateNTP(epoch))
      setRTC(epoch);
  }
}

unsigned long tBrightness = 0;
void runBrightnessTask() {
  if (millis() - tBrightness >= 5000) {
    tBrightness = millis();

    if (h >= 22 || h < 7)
      setBrightness(2);  // night mode
    else
      setBrightness(8);  // daytime
  }
}

void runButtonTask() {
  if (millis() - tButtons >= 20) {
    tButtons = millis();

    Button b = getButtonEvent();

    if (b == BTN_UP)
      Serial.println("UP");

    if (b == BTN_DOWN)
      Serial.println("DOWN");

    if (b == BTN_SELECT)
      Serial.println("SELECT");

    if (b == BTN_BACK)
      Serial.println("BACK");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  initDisplay();
  initRTC();
  initWiFi();
  initNTP();
  initMenu();

  long epoch;

  if (updateNTP(epoch)) {
    setRTC(epoch);
  }
}

void loop() {
  runRTCTask();
  runDisplayTask();
  runBrightnessTask();
  runButtonTask();
  runMenu();
  runNTPTask();
}