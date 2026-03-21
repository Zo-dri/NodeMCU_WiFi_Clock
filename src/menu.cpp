#include <RTClib.h>
#include "buttons.h"
#include "buzzer.h"
#include "display.h"
#include "menu.h"
#include "preferences.h"
#include "rtc.h"
#include "wifi.h"
#include "WString.h"

MenuState state = CLOCK_VIEW;
MenuState lastState = MAIN_MENU;

String MenuStateName[Num_MenuState] = {"CLOCK_VIEW", "MAIN_MENU", "SET_HOUR",
                                       "SET_MINUTE", "SET_DAY", "SET_MONTH",
                                       "SET_YEAR", "SET_ALARM", "SET_ALARM_ENABLE",
                                       "SET_ALARM_HOUR", "SET_ALARM_MINUTE", "H24_SET",
                                       "SET_BRIGHT_DAY", "SET_BRIGHT_NIGHT", "SET_WIFI",
                                       "RESET_WIFI"};

// Loops from high to low on overflow and vice-versa
#define loopValue(amt, low, high) ((amt) < (low) ? (high) : ((amt) > (high) ? (low) : (amt)))

#define MENU_TIMEOUT 30000
static unsigned long lastInteraction = 0;

int menuIndex = 0;
int alarmIndex = 0;

int editHour, lastEditHour;
int editMinute, lastEditMinute;
int editDay, lastEditDay;
int editMonth, lastEditMonth;
int editYear, lastEditYear;
int editAlarmHour, lastEditAlarmHour;
int editAlarmMinute, lastEditAlarmMinute;
int editAlarmEnabled, lastEditAlarmEnable;

unsigned long blinkTimer = 0;
bool blinkState = true;

bool use24H = false;
bool edit24H = false;

int useBrightnessDay = 8, useBrightnessNight = 2;
int editBrightnessDay, editBrightnessNight;

String printStatePretty() {
  if (state != lastState) {
    lastState = state;
    Serial.print("State: ");
    Serial.print(MenuStateName[state]);
    return MenuStateName[state];
    // switch (state) {
    //   case CLOCK_VIEW:
    //     Serial.print("CLOCK_VIEW");
    //     return "CLOCK_VIEW";
    //   case MAIN_MENU:
    //     Serial.print("MAIN_MENU");
    //     return "MAIN_MENU";
    //   case SET_HOUR:
    //     Serial.print("SET_HOUR");
    //     return "SET_HOUR";
    //   case SET_MINUTE:
    //     Serial.print("SET_MINUTE");
    //     return "SET_MINUTE";
    //   case SET_DAY:
    //     Serial.print("SET_DAY");
    //     return "SET_DAY";
    //   case SET_MONTH:
    //     Serial.print("SET_MONTH");
    //     return "SET_MONTH";
    //   case SET_YEAR:
    //     Serial.print("SET_YEAR");
    //     return "SET_YEAR";
    //   case RESET_WIFI:
    //     Serial.print("RESET_WIFI");
    //     return "RESET_WIFI";
    //   case SET_WIFI:
    //     Serial.print("SET_WIFI");
    //     return "SET_WIFI";
    //   }
  }
  return "UNCHANGED";
}
String getStatePretty()
{
  if (state != lastState) {
    lastState = state;
    Serial.print("State: ");
    return MenuStateName[state];
    // switch (state) {
    //   case CLOCK_VIEW:
    //     return "CLOCK_VIEW";
    //   case MAIN_MENU:
    //     return "MAIN_MENU";
    //   case SET_HOUR:
    //     return "SET_HOUR";
    //   case SET_MINUTE:
    //     return "SET_MINUTE";
    //   case SET_DAY:
    //     return "SET_DAY";
    //   case SET_MONTH:
    //     return "SET_MONTH";
    //   case SET_YEAR:
    //     return "SET_YEAR";
    //   case RESET_WIFI:
    //     return "RESET_WIFI";
    //   case SET_WIFI:
    //     return "SET_WIFI";
    //   }
  }
  return "UNCHANGED";
}
void printStatePretty(char n)
{
  n = 0; // to avoid unused warning
  if (state != lastState)
  {
    lastState = state;
    Serial.print("State: ");
    Serial.print(MenuStateName[state]);
    // switch (state)
    // {
    // case CLOCK_VIEW:
    //   Serial.print("CLOCK_VIEW");
    //   break;
    // case MAIN_MENU:
    //   Serial.print("MAIN_MENU");
    //   break;
    // case SET_HOUR:
    //   Serial.print("SET_HOUR");
    //   break;
    // case SET_MINUTE:
    //   Serial.print("SET_MINUTE");
    //   break;
    // case SET_DAY:
    //   Serial.print("SET_DAY");
    //   break;
    // case SET_MONTH:
    //   Serial.print("SET_MONTH");
    //   break;
    // case SET_YEAR:
    //   Serial.print("SET_YEAR");
    //   break;
    // case RESET_WIFI:
    //   Serial.print("RESET_WIFI");
    //   break;
    // case SET_WIFI:
    //   Serial.print("SET_WIFI");
    //   break;
    // }
  }
}

MenuState getState() {
  return state;
}
bool get24H()
{
  return use24H;
}
int getCustomBrightnessDay()
{
  return useBrightnessDay;
}
int getCustomBrightnessNight()
{
  return useBrightnessNight;
}
int setCustomBrightness(int level)
{
  useBrightnessDay = level;
  return useBrightnessDay;
}

void initMenu() {
  use24H = prefs.use24Hour;
  useBrightnessDay = prefs.brightnessDay;
  useBrightnessNight = prefs.brightnessNight;
}

void updateBlink() {
  if (millis() - blinkTimer > 500) {
    blinkTimer = millis();
    blinkState = !blinkState;
  }
}

void runMenu() {
  updateBlink();
  Button b = getButtonEvent();

  if (b != BTN_NONE)
  {
    buttonBeep();
    lastInteraction = millis();
  }
  printButtonState(b);

  if (state != CLOCK_VIEW)
  {
    if (millis() - lastInteraction > MENU_TIMEOUT)
    {
      state = CLOCK_VIEW;
      return;
    }
  }
  int h, m, s, d, mo, y;

  // printStatePretty(0);
  switch (state)
  {
  case CLOCK_VIEW:
  {
    if (b == BTN_SELECT)
    {
      menuIndex = 0;

      getTime(h, m, s);
      getDate(d, mo, y);
      editHour = h, lastEditHour = editHour;
      editMinute = m, lastEditMinute = editMinute;
      editDay = d, lastEditDay = editDay;
      editMonth = mo, lastEditMonth = editMonth;
      editYear = y, lastEditYear = editYear;

      edit24H = use24H;
      editBrightnessDay = prefs.brightnessDay;
      editBrightnessNight = prefs.brightnessNight;

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
  }

    case MAIN_MENU:
    {
      if (b == BTN_UP)
        menuIndex++;
      if (b == BTN_DOWN)
        menuIndex--;

      menuIndex = loopValue(menuIndex, 0, 7);

      if (b == BTN_BACK)
      {
        state = CLOCK_VIEW;
      }
      if (b == BTN_SELECT)
      {
        switch (menuIndex)
        {
        case 0:
        {
          state = SET_HOUR;
          break;
        }
        case 1:
        {
          state = SET_DAY;
          break;
        }
        case 2:
        {
          state = SET_ALARM;
          alarmIndex = 0;
          break;
        }
        case 3:
        {
          state = H24_SET;
          break;
        }
        case 4:
        {
          state = SET_BRIGHT_DAY;
          break;
        }
        case 5:
        {
          state = SET_BRIGHT_NIGHT;
          break;
        }
        case 6:
        {
          state = SET_WIFI;
          break;
        }
        case 7:
        {
          state = RESET_WIFI;
          break;
        }
        default:
          break;
        }
      }

      switch (menuIndex)
      {
      case 0:
      {
        if (blinkState)
        {
          showTime(editHour, editMinute, 0, true);
        }
        else
        {
          clearTime();
        }
        showDate(editDay, editMonth, editYear);
        break;
      }
      case 1:
      {
        showTime(editHour, editMinute, 0, true);
        if (blinkState)
        {
          showDate(editDay, editMonth, editYear);
        }
        else
        {
          clearDate();
        }
        break;
      }
      case 2:
      {
        if (blinkState)
        {
          setTimeDisplay("set ");
          setDateDisplay(" ala  ");
        }
        else
        {
          clearTime();
          clearDate();
        }
        break;
      }
      case 3:
      {
        set24H(edit24H, blinkState);
        break;
      }
      case 4:
      {
        setBrightDisplay(0, editBrightnessDay, blinkState);
        break;
      }
      case 5:
      {
        setBrightDisplay(1, editBrightnessNight, blinkState);
        break;
      }
      case 6:
      {
        connectWifiDisplay(blinkState);
        break;
      }
      case 7:
      {
        resetWifiDisplay(blinkState);
        break;
      }
      default:
        break;
      }

      break;
    }

    case SET_HOUR:
    {
      if (b == BTN_UP)
        editHour = (editHour + 1) % 24;
      if (b == BTN_DOWN)
        editHour = (editHour + 23) % 24;

      if (b == BTN_BACK)
      {
        editHour = lastEditHour;
        state = MAIN_MENU;
      }

      if (b == BTN_SELECT)
      {
        state = SET_MINUTE;
      }

      showTimeEdit(editHour, editMinute, 0, blinkState);
      showDate(editDay, editMonth, editYear);
      break;
    }
    case SET_MINUTE:
    {
      if (b == BTN_UP)
        editMinute = (editMinute + 1) % 60;
      if (b == BTN_DOWN)
        editMinute = (editMinute + 59) % 60;

      if (b == BTN_BACK)
      {
        editMinute = lastEditMinute;
        state = SET_HOUR;
      }

      if (b == BTN_SELECT)
      {
        int d, mo, y;
        getDate(d, mo, y);

        setRTC(DateTime(y, mo, d, editHour, editMinute, 0).unixtime());
        // menuIndex = 0;
        state = MAIN_MENU;
      }

      showTimeEdit(editHour, editMinute, 2, blinkState);
      showDate(editDay, editMonth, editYear);
      break;
    }
    case SET_DAY:
    {
      if (b == BTN_UP)
        editDay++;
      if (b == BTN_DOWN)
        editDay--;

      if (editDay < 1)
        editDay = 31;
      if (editDay > 31)
        editDay = 1;

      if (b == BTN_SELECT)
      {
        state = SET_MONTH;
      }
      if (b == BTN_BACK)
      {
        editDay = lastEditDay;
        state = MAIN_MENU;
      }

      // showDate(editDay, 1, 0);
      showTime(editHour, editMinute, 0, true);
      showDateEdit(editDay, editMonth, editYear, 0, blinkState);

      break;
    }
    case SET_MONTH:
    {
      if (b == BTN_UP)
        editMonth++;
      if (b == BTN_DOWN)
        editMonth--;

      if (editMonth < 1)
        editMonth = 12;
      if (editMonth > 12)
        editMonth = 1;

      if (b == BTN_SELECT)
      {
        state = SET_YEAR;
      }
      if (b == BTN_BACK)
      {
        editMonth = lastEditMonth;
        state = SET_DAY;
      }

      showTime(editHour, editMinute, 0, true);
      showDateEdit(editDay, editMonth, editYear, 2, blinkState);

      break;
    }
    case SET_YEAR:
    {
      if (b == BTN_UP)
        editYear++;
      if (b == BTN_DOWN)
        editYear--;

      if (editYear < 2000)
        editYear = 2099;
      if (editYear > 2099)
        editYear = 2000;

      if (b == BTN_SELECT)
      {
        int h, m, s;
        getTime(h, m, s);

        setRTC(DateTime(editYear, editMonth, editDay, h, m, s).unixtime());
        // menuIndex = 1;
        state = MAIN_MENU;
      }

      if (b == BTN_BACK)
      {
        editYear = lastEditYear;
        // menuIndex = 1;
        state = SET_MONTH;
      }

      showTime(editHour, editMinute, 0, true);
      showDateEdit(editDay, editMonth, editYear, 4, blinkState);

      break;
    }

    case SET_ALARM:
    {
      if (b == BTN_UP)
      {
        alarmIndex++;
      }
      if (b == BTN_DOWN)
      {
        alarmIndex--;
      }
      alarmIndex = loopValue(alarmIndex, 0, 1);

      if (b == BTN_BACK)
      {
        // menuIndex = 2;
        state = MAIN_MENU;
      }
      if (b == BTN_SELECT)
      {
        switch (alarmIndex)
        {
        case 0:
          editAlarmHour = prefs.alarm1.hour;
          editAlarmMinute = prefs.alarm1.minute;
          editAlarmEnabled = prefs.alarm1.enabled;
          break;

        case 1:
          editAlarmHour = prefs.alarm2.hour;
          editAlarmMinute = prefs.alarm2.minute;
          editAlarmEnabled = prefs.alarm2.enabled;
          break;

        default:
          break;
        }
        lastEditAlarmHour = editAlarmHour;
        lastEditAlarmMinute = editAlarmMinute;
        lastEditAlarmEnable = editAlarmEnabled;
        state = SET_ALARM_ENABLE;
      }

      setAlarmDisplay(alarmIndex, blinkState);
      break;
    }
    case SET_ALARM_ENABLE:
    {
      if (b == BTN_UP || b == BTN_DOWN)
      {
        editAlarmEnabled = !editAlarmEnabled;
      }
      if (b == BTN_BACK)
      {
        editAlarmEnabled = lastEditAlarmEnable;
        state = SET_ALARM;
      }
      if (b == BTN_SELECT)
      {
        bool valueChanged = false;
        switch (alarmIndex)
        {
        case 0:
        {
          if (editAlarmEnabled != prefs.alarm1.enabled)
          {
            prefs.alarm1.enabled = editAlarmEnabled;
            valueChanged = true;
          }
          break;
        }
        case 1:
        {
          if (editAlarmEnabled != prefs.alarm2.enabled)
          {
            prefs.alarm2.enabled = editAlarmEnabled;
            valueChanged = true;
          }
          break;
        }
        }
        if (valueChanged)
        {
          savePreferences();
        }
        state = SET_ALARM_HOUR;
      }
      showTime(editAlarmHour, editAlarmMinute, 0, true);
      editAlarmDisplay(alarmIndex, editAlarmEnabled, blinkState);
      break;
    }
    case SET_ALARM_HOUR:
    {
      if (b == BTN_UP)
        editAlarmHour = (editAlarmHour + 1) % 24;
      if (b == BTN_DOWN)
        editAlarmHour = (editAlarmHour + 23) % 24;

      if (b == BTN_SELECT)
        state = SET_ALARM_MINUTE;
      if (b == BTN_BACK)
      {
        editAlarmHour = lastEditAlarmHour;
        state = SET_ALARM_ENABLE;
      }
      showTimeEdit(editAlarmHour, editAlarmMinute, 0, blinkState);
      editAlarmDisplay(alarmIndex, editAlarmEnabled, 1);
      break;
    }
    case SET_ALARM_MINUTE:
    {
      if (b == BTN_UP)
        editAlarmMinute = (editAlarmMinute + 1) % 60;
      if (b == BTN_DOWN)
        editAlarmMinute = (editAlarmMinute + 59) % 60;

      if (b == BTN_BACK)
      {
        editAlarmMinute = lastEditAlarmMinute;
        state = SET_ALARM_HOUR;
      }

      if (b == BTN_SELECT)
      {
        bool valueChanged = false;
        switch (alarmIndex)
        {
        case 0:
        {
          if (editAlarmHour != prefs.alarm1.hour)
          {
            prefs.alarm1.hour = editAlarmHour;
            valueChanged = true;
          }
          if (editAlarmMinute != prefs.alarm1.minute)
          {
            prefs.alarm1.minute = editAlarmMinute;
            valueChanged = true;
          }
          if (editAlarmEnabled != prefs.alarm1.enabled)
          {
            prefs.alarm1.enabled = editAlarmEnabled;
            valueChanged = true;
          }
          break;
        }
        case 1:
        {
          if (editAlarmHour != prefs.alarm2.hour)
          {
            prefs.alarm2.hour = editAlarmHour;
            valueChanged = true;
          }
          if (editAlarmMinute != prefs.alarm2.minute)
          {
            prefs.alarm2.minute = editAlarmMinute;
            valueChanged = true;
          }
          if (editAlarmEnabled != prefs.alarm2.enabled)
          {
            prefs.alarm2.enabled = editAlarmEnabled;
            valueChanged = true;
          }
          break;
        }
        }
        if (valueChanged)
        {
          savePreferences();
        }
        // menuIndex = 2;
        state = MAIN_MENU;
      }

      showTimeEdit(editAlarmHour, editAlarmMinute, 2, blinkState);
      editAlarmDisplay(alarmIndex, editAlarmEnabled, 1);
      break;
    }

    case H24_SET:
    {
      if (b == BTN_UP || b == BTN_DOWN)
      {
        edit24H = !edit24H;
      }
      if (b == BTN_SELECT)
      {
        // menuIndex = 2;
        use24H = edit24H;
        if (use24H != prefs.use24Hour)
        {
          prefs.use24Hour = use24H;
          savePreferences();
        }
        state = MAIN_MENU;
      }
      if (b == BTN_BACK)
      {
        // menuIndex = 2;
        edit24H = use24H;
        state = MAIN_MENU;
      }
      set24H(edit24H, true);
      break;
    }
    case SET_BRIGHT_DAY:
    {
      if (b == BTN_UP)
      {
        editBrightnessDay++;
      }
      if (b == BTN_DOWN)
      {
        editBrightnessDay--;
      }
      if (b == BTN_SELECT)
      {
        useBrightnessDay = editBrightnessDay;
        if (editBrightnessDay != prefs.brightnessDay)
        {
          prefs.brightnessDay = editBrightnessDay;
          savePreferences();
        }
        // menuIndex = 3;
        state = MAIN_MENU;
      }
      if (b == BTN_BACK)
      {
        editBrightnessDay = useBrightnessDay;
        // menuIndex = 3;
        state = MAIN_MENU;
      }
      editBrightnessDay = constrain(editBrightnessDay, 1, 15);
      setBrightness(editBrightnessDay);
      setBrightDisplay(0, editBrightnessDay, true);
      break;
    }
    case SET_BRIGHT_NIGHT:
    {
      if (b == BTN_UP)
      {
        editBrightnessNight++;
      }
      if (b == BTN_DOWN)
      {
        editBrightnessNight--;
      }
      editBrightnessNight = constrain(editBrightnessNight, 1, 15);

      if (b == BTN_SELECT)
      {
        useBrightnessNight = editBrightnessNight;
        if (editBrightnessNight != prefs.brightnessNight)
        {
          prefs.brightnessNight = editBrightnessNight;
          savePreferences();
        }
        // menuIndex = 4;
        state = MAIN_MENU;
      }
      if (b == BTN_BACK)
      {
        editBrightnessNight = useBrightnessNight;
        // menuIndex = 4;
        state = MAIN_MENU;
      }
      setBrightness(editBrightnessNight);
      setBrightDisplay(1, editBrightnessNight, true);
      break;
    }

    case SET_WIFI:
    {
      if (!isWifiSaved())
      {
        setTimeDisplay("no  ");
        setDateDisplay("ap set");
        delay(2000);
        state = CLOCK_VIEW;
        break;
      }
      silencio();
      setTimeDisplay("ap  ");
      setDateDisplay("trying");
      if (!useWiFi())
      {
        setTimeDisplay("noap");
        setDateDisplay("found ");
        delay(2000);
      }
      else
      {
        clearTime();
        setDateDisplay("succes");
        delay(2000);
      }
      buttonBeep();
      state = CLOCK_VIEW;
      break;
    }
    case RESET_WIFI:
    {
      resetWifi();
      silencio();
      if (!initWiFi())
      {
        setTimeDisplay("not ");
        setDateDisplay(" set  ");
        delay(2000);
      }
      else
      {
        clearTime();
        setDateDisplay("succes");
        delay(2000);
      }
      buttonBeep();
      state = CLOCK_VIEW;
      break;
    }
    default:
      break;
    }
}
