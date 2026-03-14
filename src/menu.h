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
  H24_SET,
  SET_BRIGHT,
  SET_WIFI,
  RESET_WIFI,
  Num_MenuState
};

MenuState getState();
int getCustomBrightness();
int setCustomBrightness(int level);

bool get24H();
void initMenu();
void runMenu();
String printStatePretty();
String getStatePretty();
void printStatePretty(char n);

#endif