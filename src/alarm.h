#ifndef ALARM_H
#define ALARM_H
#include "preferences.h"

void checkAlarm(Alarm alarm, int hour, int minute, int second);
void runAlarm(int hour, int minute, int second);
void stopAlarm();

#endif
