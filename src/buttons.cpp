#include <Arduino.h>
#include "buttons.h"

#define BTN_PIN A0

#define HOLD_DELAY 600
#define REPEAT_RATE 120
#define DEBOUNCE_TIME 100

Button readButton() {
  static Button lastButton = BTN_NONE;
  static unsigned long lastChange = 0;

  int v = analogRead(BTN_PIN);

  Button b = BTN_NONE;

  if (v < 740) b = BTN_NONE;
  else if (v < 780) b = BTN_BACK;
  else if (v < 850) b = BTN_DOWN;
  else if (v < 950) b = BTN_UP;
  else if (v < 1023) b = BTN_SELECT;

  // debounce
  if (b != lastButton) {
    if (millis() - lastChange > 50) {
      lastButton = b;
      lastChange = millis();
      return b;
    }
  }

  return BTN_NONE;
}

Button readButtonRaw() {
  int v = analogRead(BTN_PIN);

  if (v < 740) return BTN_NONE;     // Ext: 637 712
  if (v < 780) return BTN_BACK;     // Ext: 684 763
  if (v < 850) return BTN_DOWN;     // Ext: 732 822
  if (v < 950) return BTN_UP;       // Ext: 805 892
  if (v < 1023) return BTN_SELECT;  // Ext: 885 974

  return BTN_NONE;
}

Button getButtonEvent()
{
  static Button stableButton = BTN_NONE;
  static Button lastReading = BTN_NONE;

  static unsigned long debounceTimer = 0;
  static unsigned long pressTime = 0;
  static unsigned long repeatTimer = 0;

  Button reading = readButtonRaw();
  unsigned long now = millis();

  // reading changed → start debounce timer
  if (reading != lastReading)
  {
    debounceTimer = now;
    lastReading = reading;
  }

  // wait until stable
  if (now - debounceTimer > DEBOUNCE_TIME)
  {
    if (reading != stableButton)
    {
      stableButton = reading;

      if (stableButton != BTN_NONE)
      {
        pressTime = now;
        repeatTimer = now;
        return stableButton; // new press
      }
    }
  }

  // handle hold repeat
  if (stableButton != BTN_NONE)
  {
    if (now - pressTime > HOLD_DELAY)
    {
      if (now - repeatTimer > REPEAT_RATE)
      {
        repeatTimer = now;
        return stableButton;
      }
    }
  }

  return BTN_NONE;
}
/*
Button getButtonEvent() {
  static Button lastButton = BTN_NONE;
  static unsigned long pressTime = 0;
  static unsigned long repeatTimer = 0;

  Button b = readButtonRaw();
  unsigned long now = millis();

  if (b != lastButton) {
    lastButton = b;

    if (b != BTN_NONE) {
      pressTime = now;
      repeatTimer = now;
      return b;  // first press
    }

    return BTN_NONE;
  }

  if (b != BTN_NONE) {
    if (now - pressTime > HOLD_DELAY) {
      if (now - repeatTimer > REPEAT_RATE) {
        repeatTimer = now;
        return b;  // repeating event
      }
    }
  }

  return BTN_NONE;
}
*/
void printButtonState(Button b)
{
  if (b == BTN_UP)
    Serial.println("UP");

  if (b == BTN_DOWN)
    Serial.println("DOWN");

  if (b == BTN_SELECT)
    Serial.println("SELECT");

  if (b == BTN_BACK)
    Serial.println("BACK");
}