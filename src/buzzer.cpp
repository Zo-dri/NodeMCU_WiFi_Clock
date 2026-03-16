#include <Arduino.h>
#include "buzzer.h"
#include "rtc.h"

#define BUZZER_PIN D3

#define BEEP_LENGTH 120
#define BEEP_GAP 200

static bool uiBeep = false;

static bool buzzing = false;
static int remainingBeeps = 0;
static bool toneState = false;

static unsigned long lastToggle = 0;

static int lastHour = -1;
static int lastHalfHour = -1;

void initBuzzer()
{
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void runBuzzerTask()
{
  int hour,  minute,  second;
  getTime(hour, minute, second);
  unsigned long now = millis();

  // ---- trigger hourly chime ----
  if (minute == 0 && second == 0 && hour != lastHour)
  {
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
