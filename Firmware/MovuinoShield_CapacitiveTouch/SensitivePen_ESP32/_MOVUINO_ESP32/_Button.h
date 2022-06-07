#if !defined(_MOVUINOESP32_BUTTON_)
#define _MOVUINOESP32_BUTTON_

#include <Arduino.h>
#include <Yabl.h>

#define PIN_BUTTON 13

unsigned long __timerPress0;
bool __isPressed = false;
bool __isReleased = false;
bool __isHold = false;
bool __isDoubleTap = false;

class MovuinoButton
{
private:
  Button _button;

public:
  MovuinoButton(/* args */);
  ~MovuinoButton();

  void begin();
  void update();
  void reset();

  static void onPress();
  static void onRelease();
  static void onHold();
  static void onDoubleTap();

  bool isPressed();
  bool isReleased();
  bool isDoubleTap();
  unsigned int timeHold();
};

#endif // _MOVUINOESP32_BUTTON_