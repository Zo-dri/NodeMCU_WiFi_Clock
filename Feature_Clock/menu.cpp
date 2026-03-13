#include "WString.h"
#include "menu.h"
#include "buttons.h"
#include "display.h"
#include <RTClib.h>
#include "rtc.h"
#include "wifi.h"

MenuState state = CLOCK_VIEW;
MenuState lastState = MAIN_MENU;

int menuIndex = 0;

int editHour;
int editMinute;
int editDay;
int editMonth;
int editYear;

unsigned long blinkTimer = 0;
bool blinkState = true;

String printStatePretty() {
  if (state != lastState) {
    lastState = state;
    Serial.print("State: ");
    switch (state) {
      case CLOCK_VIEW:
        Serial.print("CLOCK_VIEW");
        return "CLOCK_VIEW";
      case MAIN_MENU:
        Serial.print("MAIN_MENU");
        return "MAIN_MENU";
      case SET_HOUR:
        Serial.print("SET_HOUR");
        return "SET_HOUR";
      case SET_MINUTE:
        Serial.print("SET_MINUTE");
        return "SET_MINUTE";
      case SET_DAY:
        Serial.print("SET_DAY");
        return "SET_DAY";
      case SET_MONTH:
        Serial.print("SET_MONTH");
        return "SET_MONTH";
      case SET_YEAR:
        Serial.print("SET_YEAR");
        return "SET_YEAR";
    }
  }
  return "UNCHANGED";
}
void printStatePretty(byte n) {
  n = 0;  // to avoid unused warning
  if (state != lastState) {
    lastState = state;
    Serial.print("State: ");
    switch (state) {
      case CLOCK_VIEW:
        Serial.print("CLOCK_VIEW");
        break;
      case MAIN_MENU:
        Serial.print("MAIN_MENU");
        break;
      case SET_HOUR:
        Serial.print("SET_HOUR");
        break;
      case SET_MINUTE:
        Serial.print("SET_MINUTE");
        break;
      case SET_DAY:
        Serial.print("SET_DAY");
        break;
      case SET_MONTH:
        Serial.print("SET_MONTH");
        break;
      case SET_YEAR:
        Serial.print("SET_YEAR");
        break;
    }
  }
}

MenuState getState() {
  return state;
}

void initMenu() {}

void updateBlink() {
  if (millis() - blinkTimer > 500) {
    blinkTimer = millis();
    blinkState = !blinkState;
  }
}

void runMenu() {
  updateBlink();
  Button b = getButtonEvent();

  int h, m, s, d, mo, y;

  printStatePretty(0);

  switch (state) {

    case CLOCK_VIEW:

      if (b == BTN_SELECT) {
        menuIndex = 0;

        getTime(h, m, s);
        getDate(d, mo, y);
        editHour = h;
        editMinute = m;
        editDay = d;
        editMonth = mo;
        editYear = y;
        Serial.println();
        Serial.print(editHour);
        Serial.print(":");
        Serial.print(editMinute);
        Serial.print(" ");
        Serial.print(editDay);
        Serial.print("/");
        Serial.print(editMonth);
        Serial.print("/");
        Serial.print(editYear);

        state = MAIN_MENU;
      }

      break;


    case MAIN_MENU:

      if (b == BTN_UP) menuIndex--;
      if (b == BTN_DOWN) menuIndex++;

      if (menuIndex < 0) menuIndex = 2;
      if (menuIndex > 2) menuIndex = 0;

      if (b == BTN_SELECT) {
        switch (menuIndex) {
          case 0:
            state = SET_HOUR;
            break;

          case 1:
            state = SET_DAY;
            break;
          case 2:
            state = RESET_WIFI;
          default:
            break;
        }
      }

      switch (menuIndex) {
        case 0:
          if (blinkState) {
            showTime(editHour, editMinute, 0, true);
          } else {
            clearTime();
          }
          break;
        case 1:
          if (blinkState) {
            showDate(editDay, editMonth, editYear);
          } else {
            clearDate();
          }
          break;
        default:
          break;
      }
      // }
      // else {
      //   switch (menuIndex) {
      //     case 0:
      //       clearTime();
      //       break;
      //     case 1:
      //       break;
      //     default:
      //       break;
      //   }
      // }
      break;

    case SET_HOUR:

      if (b == BTN_UP) editHour = (editHour + 1) % 24;
      if (b == BTN_DOWN) editHour = (editHour + 23) % 24;

      if (b == BTN_SELECT) state = SET_MINUTE;
      if (b == BTN_BACK) state = MAIN_MENU;

      showTimeEdit(editHour, editMinute, 1, blinkState);
      // if (blinkState)
      //   showTime(editHour, editMinute, s, true);
      // else
      //   // showTime(99, 99, s, false);  // blank
      //   clearTime(0);  // Clear Hour
      break;

    case SET_MINUTE:

      if (b == BTN_UP) editMinute = (editMinute + 1) % 60;
      if (b == BTN_DOWN) editMinute = (editMinute + 59) % 60;

      if (b == BTN_SELECT) {
        int d, mo, y;
        getDate(d, mo, y);

        setRTC(DateTime(y, mo, d, editHour, editMinute, 0).unixtime());
        state = CLOCK_VIEW;
      }

      if (b == BTN_BACK) state = MAIN_MENU;
      showTimeEdit(editHour, editMinute, 3, blinkState);
      // if (blinkState)
      //   showTime(editHour, editMinute, s, true);
      // else
      //   // showTime(99, 99, s, false);
      //   clearTime(1);  // Clear Minute
      break;

    case SET_DAY:

      if (b == BTN_UP) editDay++;
      if (b == BTN_DOWN) editDay--;

      if (editDay < 1) editDay = 31;
      if (editDay > 31) editDay = 1;

      if (b == BTN_SELECT) state = SET_MONTH;
      if (b == BTN_BACK) state = MAIN_MENU;

      // showDate(editDay, 1, 0);
      showDateEdit(editDay, editMonth, editYear, 1, blinkState);

      break;


    case SET_MONTH:

      if (b == BTN_UP) editMonth++;
      if (b == BTN_DOWN) editMonth--;

      if (editMonth < 1) editMonth = 12;
      if (editMonth > 12) editMonth = 1;

      if (b == BTN_SELECT) state = SET_YEAR;
      if (b == BTN_BACK) state = MAIN_MENU;

      // showDate(editDay, editMonth, 0);
      showDateEdit(editDay, editMonth, editYear, 3, blinkState);

      break;


    case SET_YEAR:

      if (b == BTN_UP) editYear++;
      if (b == BTN_DOWN) editYear--;

      if (editYear < 2000) editYear = 2099;
      if (editYear > 2099) editYear = 2000;

      if (b == BTN_SELECT) {
        int h, m, s;
        getTime(h, m, s);

        setRTC(DateTime(editYear, editMonth, editDay, h, m, s).unixtime());
        state = CLOCK_VIEW;
      }

      if (b == BTN_BACK) state = MAIN_MENU;

      // showDate(editDay, editMonth, editYear);
      showDateEdit(editDay, editMonth, editYear, 5, blinkState);

      break;
    case RESET_WIFI:
      resetWifi();
      break;
  }
}
