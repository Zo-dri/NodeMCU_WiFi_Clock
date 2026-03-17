#include <Arduino.h>
#include "buzzer.h"
#include "rtc.h"
#include "display.h"
#include "tasks.h"

#define BUZZER_PIN D0

#define BEEP_LENGTH 700
#define BEEP_GAP 300
#define BUTTON_BEEP 120

static bool uiBeep = false;

static bool buzzing = false;
static int remainingBeeps = 0;
static bool toneState = false;

static unsigned long lastToggle = 0;

static int lastHour = -1;
static int lastHalfHour = -1;
int *localTime = new int[6];
void initBuzzer()
{
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void buttonBeep()
{
  digitalWrite(BUZZER_PIN, HIGH);
  uiBeep = true;
  lastToggle = millis();
}

void silencio()
{
  uiBeep = false;
  remainingBeeps = 0;
  toneState = false;
  buzzing = false;
  digitalWrite(BUZZER_PIN, LOW);
}

void runBuzzerTask()
{
  int hour,  minute,  second;
  // int *localTime = getLocalESPTime();
  getLocalESPTime(localTime);
  hour = localTime[0];
  minute = localTime[1];
  second = localTime[2];

  // prettyPrintTime(hour, minute, second);

  unsigned long now = millis();

  // ---- handle UI beep ----
  if (uiBeep)
  {
    if (now - lastToggle > BUTTON_BEEP)
    {
      digitalWrite(BUZZER_PIN, LOW);
      uiBeep = false;
    }
    return;
  }

  // ---- trigger hourly chime ----
  if (minute == 0 && second == 0 && hour != lastHour)
  {
    Serial.print("Hourly chime");
    lastHour = hour;

    int h = hour % 12;
    if (h == 0)
      h = 12;

    remainingBeeps = h;
    buzzing = true;
    toneState = false;
  }

  // ---- trigger half-hour beep ----
  if (minute == 30 && second == 0 && hour != lastHalfHour)
  {
    Serial.print("Half-Hourly chime");
    lastHalfHour = hour;

    remainingBeeps = 1;
    buzzing = true;
    toneState = false;
  }

  // ---- run beeper ----
  if (!buzzing)
    return;

  if (!toneState && now - lastToggle >= BEEP_GAP)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    toneState = true;
    lastToggle = now;
  }
  else if (toneState && now - lastToggle >= BEEP_LENGTH)
  {
    digitalWrite(BUZZER_PIN, LOW);
    toneState = false;
    lastToggle = now;

    remainingBeeps--;

    if (remainingBeeps <= 0)
    {
      buzzing = false;
    }
  }
}
