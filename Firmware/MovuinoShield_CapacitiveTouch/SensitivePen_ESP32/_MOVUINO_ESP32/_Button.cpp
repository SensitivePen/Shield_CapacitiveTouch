#include "_Button.h"

MovuinoButton::MovuinoButton(/* args */)
{
}

MovuinoButton::~MovuinoButton()
{
}

void MovuinoButton::begin()
{
  this->_button.attach(PIN_BUTTON, INPUT_PULLUP); // pin configured to pull-up mode

  this->_button.callback(this->onPress, PRESS);
  this->_button.callback(this->onRelease, RELEASE);
  this->_button.callback(this->onHold, HOLD);
  this->_button.callback(this->onDoubleTap, DOUBLE_TAP);
}

void MovuinoButton::update()
{
  __isPressed = false;
  __isReleased = false;
  __isDoubleTap = false;

  this->_button.update();
}

void MovuinoButton::reset()
{
  __isPressed = false;
  __isReleased = false;
  __isDoubleTap = false;
  __isHold = false;
}

void MovuinoButton::onPress()
{
  __isPressed = true;
  __timerPress0 = millis();
}

void MovuinoButton::onRelease()
{
  __isReleased = true;
  __isHold = false;
}

void MovuinoButton::onHold()
{
  __isHold = true;
}

void MovuinoButton::onDoubleTap()
{
  __isDoubleTap = true;
}

bool MovuinoButton::isPressed()
{
  return __isPressed;
}

bool MovuinoButton::isReleased()
{
  return __isReleased;
}

bool MovuinoButton::isDoubleTap()
{
  return __isDoubleTap;
}

unsigned int MovuinoButton::timeHold()
{
  unsigned int time_ = 0;
  if (__isHold)
  {
    time_ = millis() - __timerPress0;
  }
  return time_;
}