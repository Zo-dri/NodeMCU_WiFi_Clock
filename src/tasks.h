#ifndef TASKS_H
#define TASKS_H

void runRTCTask();
void runDisplayTask();
void runNTPTask();
void runBrightnessTask();
void runButtonTask();
void runAlarmTask();

void getLocalESPTime(int *tempTime);

#endif
