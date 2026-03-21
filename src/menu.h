#ifndef MENU_H
#define MENU_H

enum MenuState
{
  CLOCK_VIEW,
  MAIN_MENU,
  SET_HOUR,
  SET_MINUTE,
  SET_DAY,
  SET_MONTH,
  SET_YEAR,
  SET_ALARM,
  SET_ALARM_ENABLE,
  SET_ALARM_HOUR,
  SET_ALARM_MINUTE,
  H24_SET,
  SET_BRIGHT_DAY,
  SET_BRIGHT_NIGHT,
  SET_WIFI,
  RESET_WIFI,
  Num_MenuState
};

MenuState getState();
int getCustomBrightnessDay();
int getCustomBrightnessNight();
int setCustomBrightness(int level);

bool get24H();
void initMenu();
void runMenu();
String printStatePretty();
String getStatePretty();
void printStatePretty(char n);

#endif
