#ifndef DISPLAY_H
#define DISPLAY_H

void initDisplay();
void showTime(int h, int m, int s, bool colon);
void showDate(int d, int m, int y);
void setBrightness(int level);

void clearTime();
void clearDate();
void clearTime(int select);
void clearDate(int select);
void showTimeEdit(int h, int m, int blinkDigit, bool blinkState);
void showDateEdit(int d, int m, int y, int blinkDigit, bool blinkState);

void scrollingDot();

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
void setBrightDisplay(int value, bool blinkState);
void setWifiDisplay(int value);

#endif