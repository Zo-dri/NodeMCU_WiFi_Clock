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

void set24H(int display, bool blinkState);
void setBrightDisplay(int value, bool blinkState);
void setWifiDisplay(int value);

#endif