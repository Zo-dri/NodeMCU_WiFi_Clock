#ifndef MENU_H
#define MENU_H

enum MenuState {
  CLOCK_VIEW,
  MAIN_MENU,
  SET_HOUR,
  SET_MINUTE,
  SET_DAY,
  SET_MONTH,
  SET_YEAR,
  RESET_WIFI
};

MenuState getState();
void initMenu();
void runMenu();
String printStatePretty();

#endif