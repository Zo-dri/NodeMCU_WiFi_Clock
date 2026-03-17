#include <LedControl.h>
#include "display.h"
#include "menu.h"

#define DIN D8
#define CLK D7
#define CS_TIME D6
#define CS_DATE D5

LedControl timeDisplay = LedControl(DIN, CLK, CS_TIME, 1);
LedControl dateDisplay = LedControl(DIN, CLK, CS_DATE, 1);

byte scrollDot_No = 0;
byte last_h, last_m, last_s, last_dd, last_mm, last_yyyy;

void prettyPrintTime(int h, int m, int s)
{
  if (h != last_h || m != last_m || s != last_s) {
    Serial.print("Time: ");
    last_h = h, last_m = m, last_s = s;
    if (h < 10) Serial.print("0");
    Serial.print(h);
    Serial.print(":");
    if (m < 10) Serial.print("0");
    Serial.print(m);
    Serial.print(":");
    if (s < 10) Serial.print("0");
    Serial.print(s);
    Serial.println();
  }
}
void prettyPrintDate(int dd, int mm, int yyyy)
{
  if (dd != last_dd || mm != last_mm || yyyy != last_yyyy) {
    Serial.print("Date: ");
    last_dd = dd, last_mm = mm, last_yyyy = yyyy;
    if (dd < 10) Serial.print("0");
    Serial.print(dd);
    Serial.print("-");
    if (mm < 10) Serial.print("0");
    Serial.print(mm);
    Serial.print("-");
    if (yyyy < 10) Serial.print("0");
    Serial.print(yyyy);
    Serial.println();
  }
}

void initDisplay() {
  timeDisplay.shutdown(0, false);
  timeDisplay.setIntensity(0, 8);
  timeDisplay.clearDisplay(0);

  dateDisplay.shutdown(0, false);
  dateDisplay.setIntensity(0, 8);
  dateDisplay.clearDisplay(0);
  Serial.println("Display Initialized");
}

void showTime(int h, int m, int s, bool colon) {
  // prettyPrintTime(h, m, s);
  bool am_PM = (h > 11 || h == 0);
  bool use24H = get24H();
  if (!use24H)
  {
    if (h == 0)
      h = 12;
    else if (h > 12)
      h -= 12;
  }

  timeDisplay.setDigit(0, 0, h / 10, (use24H) ? false : am_PM);
  timeDisplay.setDigit(0, 1, h % 10, colon);

  timeDisplay.setDigit(0, 2, m / 10, colon);
  timeDisplay.setDigit(0, 3, m % 10, (use24H) ? false : !am_PM);
}
void showDate(int d, int m, int y) {
  y = y % 100;

  dateDisplay.setDigit(0, 0, d / 10, false);
  dateDisplay.setDigit(0, 1, d % 10, false);

  dateDisplay.setDigit(0, 2, m / 10, false);
  dateDisplay.setDigit(0, 3, m % 10, false);

  dateDisplay.setDigit(0, 4, y / 10, false);
  dateDisplay.setDigit(0, 5, y % 10, false);
}

void setBrightness(int level) {
  level = constrain(level, 0, 15);

  timeDisplay.setIntensity(0, level);
  dateDisplay.setIntensity(0, level);
}

void clearTime() {
  for (int i = 0; i < 4; i++)
    timeDisplay.setChar(0, i, ' ', false);
}
void clearDate() {
  for (int i = 0; i < 6; i++)
    dateDisplay.setChar(0, i, ' ', false);
}
void clearTime(int select) {
  select = constrain(select, 0, 1);
  for (int i = select * 2; i < (select * 2) + 2; i++)
    timeDisplay.setChar(0, i, ' ', false);
}
void clearDate(int select) {
  select = constrain(select, 0, 2);
  for (int i = select * 2; i < (select * 2) + 2; i++)
    dateDisplay.setChar(0, i, ' ', false);
}
void showTimeEdit(int h, int m, int blinkDigit, bool blinkState) {
  int digits[4] = {
    h / 10,
    h % 10,
    m / 10,
    m % 10
  };

  for (int i = 0; i < 4; i++) {
    if ((i == blinkDigit || i == blinkDigit + 1) && !blinkState)
      timeDisplay.setChar(0, i, ' ', false);
    else
      timeDisplay.setDigit(0, i, digits[i], false);
  }
}
void showDateEdit(int d, int m, int y, int blinkDigit, bool blinkState) {
  y = y % 100;

  int digits[6] = {
    d / 10,
    d % 10,
    m / 10,
    m % 10,
    y / 10,
    y % 10
  };

  for (int i = 0; i < 6; i++) {
    if ((i == blinkDigit || i == blinkDigit + 1) && !blinkState)
      dateDisplay.setChar(0, i, ' ', false);
    else
      dateDisplay.setDigit(0, i, digits[i], false);
  }
}
unsigned long tScroll = 0;
void scrollingDot() {
  if (millis() - tScroll >= 200)
  {
    tScroll = millis();
    scrollDot_No++;
    setTimeDisplay("AP  ");
    if (scrollDot_No > 5)
      scrollDot_No = 0;
    switch (scrollDot_No)
    {
    case 0:
      dateDisplay.setChar(0, 5, ' ', false);
      dateDisplay.setChar(0, 0, '.', false);
      break;
    case 1:
      dateDisplay.setChar(0, 0, ' ', false);
      dateDisplay.setChar(0, 1, '.', false);
      break;
    case 2:
      dateDisplay.setChar(0, 1, ' ', false);
      dateDisplay.setChar(0, 2, '.', false);
      break;
    case 3:
      dateDisplay.setChar(0, 2, ' ', false);
      dateDisplay.setChar(0, 3, '.', false);
      break;
    case 4:
      dateDisplay.setChar(0, 3, ' ', false);
      dateDisplay.setChar(0, 4, '.', false);
      break;
    case 5:
      dateDisplay.setChar(0, 4, ' ', false);
      dateDisplay.setChar(0, 5, '.', false);
      break;
    }
  }
}

void set24H(int display, bool blinkState)
{
  display = constrain(display, 0, 1);

  if (!blinkState)
  {
    clearDate();
    clearTime();
    return;
  }

  String hourString;
  if (display)
  {
    hourString = "24H ";
  }
  else
  {
    hourString = "12H ";
  }
  for (int i = 0; i < 4; i++)
  {
    timeDisplay.setChar(0, i, hourString.charAt(i), false);
  }
  clearDate();
}
void setBrightDisplay(int value, bool blinkState)
{
  int digits[2] = {
      value / 10,
      value % 10,
  };
  if (!blinkState)
  {
    clearDate();
    clearTime();
    return;
  }

  timeDisplay.setChar(0, 0, 'B', false);
  timeDisplay.setChar(0, 1, ' ', false);
  timeDisplay.setDigit(0, 2, digits[0], false);
  timeDisplay.setDigit(0,3, digits[1], false );
  clearDate();
}

/**
 * Converts the given character to a segment index.
 *
 * Params:
 * (char)seg: segment name
 * Available values 'a', 'b', 'c', 'd', 'e', 'f', 'g'
 *
 * To be used with setChar
 * Example:
 * display.setchar(0, 0, segment('a'), false);
 */
#define segment(seg) (((int)seg) - 96)

byte alphabets[27] = {
    0b11101110, // A
    0b00111110, // b
    0b10011100, // C
    0b01111010, // d
    0b10011110, // E
    0b10001110, // F
    0b10111100, // G
    0b01101110, // H
    0b00100000, // I
    0b01111000, // J
    0b00000000, // K
    0b00011100, // L
    0b00000000, // M
    0b00101010, // n
    0b00111010, // o
    0b11001110, // P
    0b11100110, // Q
    0b00001010, // r
    0b10110110, // S
    0b00011110, // t
    0b00111000, // u
    0b00000000, // V
    0b00000000, // W
    0b00000000, // X
    0b01110110, // Y
    0b11011010, // Z
    0b00000000};

/**
 * Display the english alphabet in a 7-segment display.
 *
 * Params
 * index: position of the character in the display (0...7)
 * value: the character to display
 */
void setCharacterTime(int index, char value)
{
  byte characterByte;
  if (value == ' ')
  {
    characterByte = alphabets[26];
  }
  else
  {
    characterByte = alphabets[(int)value - 97];
  }
  for (int i = 1; i <= 7; i++)
  {
    bool characterBit = (characterByte & (1 << (8 - i))) > 0;
    timeDisplay.setLed(0, index, i, characterBit);
  }
}
void setCharacterDate(int index, char value)
{
  byte characterByte;
  if (value == ' ')
  {
    characterByte = alphabets[26];
  }
  else
  {
    characterByte = alphabets[(int)value - 97];
  }
  for (int i = 1; i <= 7; i++)
  {
    bool characterBit = (characterByte & (1 << (8 - i))) > 0;
    dateDisplay.setLed(0, index, i, characterBit);
  }
}

void setTimeDisplay(String displayString)
{
  byte iterate = displayString.length();
  iterate = constrain(iterate, 0, 4);
  for (int i = 0; i < iterate; i++)
  {
    setCharacterTime(i, displayString.charAt(i));
  }
}
void setDateDisplay(String displayString)
{
  byte iterate = displayString.length();
  iterate = constrain(iterate, 0, 6);
  for (int i = 0; i < iterate; i++)
  {
    setCharacterDate(i, displayString.charAt(i));
  }
}
void setWifiDisplay(int value)
{
  setTimeDisplay("ap  ");
  setDateDisplay("reset");
  dateDisplay.setDigit(0, 5, value, false);
}
void connectWifiDisplay(bool blinkState)
{
  if (!blinkState)
  {
    clearDate();
    clearTime();
    return;
  }
  setTimeDisplay("ap  ");
  setDateDisplay("connec");
}
void resetWifiDisplay(bool blinkState)
{
  if (!blinkState)
  {
    clearDate();
    clearTime();
    return;
  }
  setTimeDisplay("ap  ");
  setDateDisplay("reset ");
}
