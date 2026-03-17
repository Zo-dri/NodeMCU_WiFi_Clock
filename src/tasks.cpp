#include <Arduino.h>
#include "tasks.h"
#include "buttons.h"
#include "preferences.h"
#include "rtc.h"
#include "display.h"
#include "menu.h"
#include "ntp.h"

unsigned long tRTC = 0;
unsigned long tDisplay = 0;
unsigned long tButtons = 0;
unsigned long tNTP = 0;

bool colon = false;

int h, m, s;
int d, mo, y;

void runRTCTask()
{
    if (millis() - tRTC >= 1000)
    {
        tRTC = millis();

        getTime(h, m, s);
        getDate(d, mo, y);

        colon = !colon;
    }
}
void getLocalESPTime(int* tempTime)
{
  // int *tempTime = new int[6];
  tempTime[0] = h;
  tempTime[1] = m;
  tempTime[2] = s;
  tempTime[3] = d;
  tempTime[4] = mo;
  tempTime[5] = y;
  // return tempTime;
}

void runDisplayTask()
{
    MenuState state = getState();
    if (state == CLOCK_VIEW)
    {
        if (millis() - tDisplay >= 200)
        {
            tDisplay = millis();

            // Serial.println("Printing Time");

            showTime(h, m, s, colon);
            showDate(d, mo, y);
        }
    }
}

void runNTPTask()
{
    if (millis() - tNTP >= 21600000)
    {
        tNTP = millis();

        long epoch;

        if (updateNTP(epoch))
            setRTC(epoch);
    }
}

unsigned long tBrightness = 0;
void runBrightnessTask()
{
    if (millis() - tBrightness >= 5000)
    {
        tBrightness = millis();

        if (h >= 22 || h < 7)
          setBrightness(setCustomBrightness(prefs.brightnessNight)); // night mode
        else
          setBrightness(setCustomBrightness(prefs.brightnessDay)); // daytime
    }
}

void runButtonTask()
{
    if (millis() - tButtons >= 20)
    {
        tButtons = millis();

        Button b = getButtonEvent();

        if (b == BTN_UP)
            Serial.println("UP");

        if (b == BTN_DOWN)
            Serial.println("DOWN");

        if (b == BTN_SELECT)
            Serial.println("SELECT");

        if (b == BTN_BACK)
            Serial.println("BACK");
    }
}
