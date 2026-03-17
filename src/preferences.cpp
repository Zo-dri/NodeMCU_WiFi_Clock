#include <Arduino.h>
#include <EEPROM.h>
#include "preferences.h"

#define EEPROM_SIZE 128
#define PREF_MAGIC 0xCAFE1234

Preferences prefs;

void printPrefrences()
{
  Serial.print("use24Hour: ");
  Serial.println(prefs.use24Hour);
  Serial.print("chimeEnabled: ");
  Serial.println(prefs.chimeEnabled);
  Serial.print("buttonBeep: ");
  Serial.println(prefs.buttonBeep);
  Serial.print("nightStart: ");
  Serial.println(prefs.nightStart);
  Serial.print("nightEnd: ");
  Serial.println(prefs.nightEnd);
  Serial.print("brightnessDay: ");
  Serial.println(prefs.brightnessDay);
  Serial.print("brightnessNight: ");
  Serial.println(prefs.brightnessNight);
}

void setDefaults()
{
  prefs.version = 1;

  prefs.use24Hour = false;
  prefs.chimeEnabled = true;
  prefs.buttonBeep = true;

  prefs.nightStart = 22;
  prefs.nightEnd = 7;

  prefs.brightnessDay = 8;
  prefs.brightnessNight = 2;

  prefs.magic = PREF_MAGIC;
  printPrefrences();
  Serial.println("Loaded Defaults");
}

void loadPreferences()
{
  EEPROM.begin(EEPROM_SIZE);

  EEPROM.get(0, prefs);

  if (prefs.magic != PREF_MAGIC)
  {
    setDefaults();
    savePreferences();
  }
  printPrefrences();
  Serial.println("Preferences Loaded");
}

void savePreferences()
{
  EEPROM.put(0, prefs);
  EEPROM.commit();
  printPrefrences();
  Serial.println("Preferences Saved");
}
