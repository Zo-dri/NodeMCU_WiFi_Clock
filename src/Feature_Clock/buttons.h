#ifndef BUTTONS_H
#define BUTTONS_H

enum Button
{
  BTN_NONE,
  BTN_UP,
  BTN_DOWN,
  BTN_SELECT,
  BTN_BACK
};

Button readButton();
Button readButtonRaw();
Button getButtonEvent();

#endif