#include <Arduino.h>
#include <EEPROM.h>
#include "buzzer.h"
#include "preferences.h"

#define EEPROM_SIZE 128
#define PREF_MAGIC 0xCAFE1234

Preferences prefs;

void printPrefrences()
{
  Serial.println("____________________________");
  Serial.println("Preferences");
  Serial.print("\tuse24Hour: ");
  Serial.println(prefs.use24Hour);
  Serial.print("\tchimeEnabled: ");
  Serial.println(prefs.chimeEnabled);
  Serial.print("\tbuttonBeep: ");
  Serial.println(prefs.buttonBeep);
  Serial.print("\tnightStart: ");
  Serial.println(prefs.nightStart);
  Serial.print("\tnightEnd: ");
  Serial.println(prefs.nightEnd);
  Serial.print("\tbrightnessDay: ");
  Serial.println(prefs.brightnessDay);
  Serial.print("\tbrightnessNight: ");
  Serial.println(prefs.brightnessNight);

  Serial.println("\tALARM 1");
  Serial.print("\t\thour: ");
  Serial.println(prefs.alarm1.hour );
  Serial.print("\t\tminute: ");
  Serial.println(prefs.alarm1.minute );
  Serial.print("\t\tenabled: ");
  Serial.println(prefs.alarm1.enabled );

  Serial.println("\tALARM 2");
  Serial.print("\t\thour: ");
  Serial.println(prefs.alarm2.hour );
  Serial.print("\t\tminute: ");
  Serial.println(prefs.alarm2.minute );
  Serial.print("\t\tenabled: ");
  Serial.println(prefs.alarm2.enabled );

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

  prefs.alarm1.enabled = false;
  prefs.alarm1.hour = 7;
  prefs.alarm1.minute = 0;

  prefs.alarm2.enabled = false;
  prefs.alarm2.hour = 7;
  prefs.alarm2.minute = 0;

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
  longBeep();
  printPrefrences();
  Serial.println("Preferences Saved");
}
