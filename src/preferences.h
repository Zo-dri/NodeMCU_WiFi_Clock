#ifndef PREFERENCES_H
#define PREFERENCES_H
#include <Arduino.h>

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

  uint32_t magic;
};

extern Preferences prefs;

void loadPreferences();
void savePreferences();

#endif
