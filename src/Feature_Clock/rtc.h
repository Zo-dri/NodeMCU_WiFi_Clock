#ifndef RTC_CLOCK_H
#define RTC_CLOCK_H

void initRTC();
void getTime(int &h, int &m, int &s);
void getDate(int &d, int &m, int &y);
void setRTC(int epoch);

#endif