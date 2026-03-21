#include <Arduino.h>
#include "alarm.h"
#include "preferences.h"
#include "buzzer.h"

static bool alarmActive = false;

static unsigned long lastBeep = 0;
static unsigned long alarmStart;

void checkAlarm(Alarm alarm, int hour, int minute, int second)
{
  if (alarm.enabled &&
      hour == alarm.hour &&
      minute == alarm.minute &&
      second == 0)
  {
    Serial.println("Alarm is alarming");
    alarmStart = millis();
    alarmActive = true;
  }
}
void stopAlarm()
{
  alarmActive = false;
}

void runAlarm(int hour, int minute, int second)
{
  checkAlarm(prefs.alarm1, hour, minute, second);
  checkAlarm(prefs.alarm2, hour, minute, second);

  if (!alarmActive)
    return;

  if (millis() - alarmStart > 60000)
  {
    stopAlarm();
  }

  if (millis() - lastBeep > 600)
  {
    lastBeep = millis();
    buttonBeep(); // reuse buzzer
  }
}
