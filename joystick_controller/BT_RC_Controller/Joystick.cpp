#include "Joystick.h"

Joystick::Joystick(uint8_t pinX, uint8_t pinY, uint16_t leftThr, uint16_t rightThr, uint16_t forwardThr, uint16_t backThr)
  :   pinX_(pinX),
      pinY_(pinY),
      leftThr_(leftThr),
      rightThr_(rightThr),
      forwardThr_(forwardThr),
      backThr_(backThr)
{
  pinMode(pinX_, INPUT);
  pinMode(pinY_, INPUT);
  // Set the ADC attenuation to 11 dB (up to ~3.3V input)
  analogSetAttenuation(ADC_11db);
}

Joystick::Dir Joystick::read()
{
  lastX_ = analogRead(pinX_);
  lastY_ = analogRead(pinY_);

  if (lastY_ < forwardThr_)
    return Dir::Forward;
  if (lastY_ > backThr_)
    return Dir::Backward;
  if (lastX_ < leftThr_)
    return Dir::Left;
  if (lastX_ > rightThr_)
    return Dir::Right;
  return Dir::Halt;
}
