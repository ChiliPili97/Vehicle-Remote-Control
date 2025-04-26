#pragma once
#include "LiquidCrystal_PCF8574.h"
#include "Wire.h"
#include "Joystick.h"

class DisplayUI
{
  public: DisplayUI(uint8_t lcdAddr, uint8_t width = 16, uint8_t height = 2, uint8_t brightness = 255);
    void begin();
    void loop(bool isConnected, int percent, Joystick::Dir dir);
    
  private:
    struct UIState {
        bool connected;
        int percent;
        Joystick::Dir dir;
    };
    enum class Screen : uint8_t {Waiting, Empty, Main};

    void showWaiting_();
    void showBattery_(int percent);
    void showDirection_(Joystick::Dir dir);
    void clearRow_(uint8_t row);

    UIState currentState_;
    UIState lastState_;
    LiquidCrystal_PCF8574 lcd_;
    Screen screen_;
    uint8_t lcdAddr_;
    uint8_t width_;
    uint8_t height_;
    uint8_t brightness_;
    bool initialized_ = false;

};
