#pragma once
#include "Arduino.h"

class Joystick
{
  public:
    enum class Dir : uint8_t {Halt, Forward, Backward, Left, Right};

    /**
      @param pinX   — GPIO pin for X
      @param pinY   — GPIO pin for Y
      @param leftThr, rightThr, forwardThr, backThr — treasholds
    */
    Joystick(uint8_t pinX, uint8_t pinY, uint16_t leftThr, uint16_t rightThr, uint16_t forwardThr, uint16_t backThr);

    /**
           @brief   Read analog values, return direction.
           @return  Dir::…  expected to treasholds
    */
    Dir read();

    inline uint16_t rawX() const {
      return lastX_;
    }
    inline uint16_t rawY() const {
      return lastY_;
    }

  private:
    const uint8_t  pinX_, pinY_;
    const uint16_t leftThr_, rightThr_, forwardThr_, backThr_;
    uint16_t       lastX_ = 0, lastY_ = 0;
};
