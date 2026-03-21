#ifndef DISPLAY_H
#define DISPLAY_H

void initDisplay();
void showTime(int h, int m, int s, bool colon);
void showDate(int d, int m, int y);
void setBrightness(int level);

void prettyPrintTime(int h, int m, int s);
void prettyPrintDate(int dd, int mm, int yyyy);

void clearTime();
void clearDate();
void clearTime(int select);
void clearDate(int select);
void showTimeEdit(int h, int m, int blinkDigit, bool blinkState);
void showDateEdit(int d, int m, int y, int blinkDigit, bool blinkState);

void scrollingDot();

void setTimeDisplay(String displayString);
void setDateDisplay(String displayString);

/**
 * Display function for the set 24H menu option.
 *
 * Params
 * display: selection for hour state
 *          0: 12H
 *          1: 24H
 * blinkState: whether the display is on or off
 */
void set24H(int display, bool blinkState);
void setBrightDisplay(bool display, int value, bool blinkState);

void setAlarmDisplay(int index, bool blinkState);
void editAlarmDisplay(int index, bool enState, bool enBlinkState);

void setWifiDisplay(int value);
void connectWifiDisplay(bool blinkState);
void resetWifiDisplay(bool blinkState);

#endif
