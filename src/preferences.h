#ifndef PREFERENCES_H
#define PREFERENCES_H
#include <Arduino.h>

struct Alarm
{
  bool enabled;
  uint8_t hour;
  uint8_t minute;
};
struct Preferences
{
  byte version;

  bool use24Hour;
  bool chimeEnabled;
  bool buttonBeep;

  uint8_t nightStart;
  uint8_t nightEnd;

  uint8_t brightnessDay;
  uint8_t brightnessNight;

  Alarm alarm1;
  Alarm alarm2;

  uint32_t magic;
};

extern Preferences prefs;

void loadPreferences();
void savePreferences();

#endif
