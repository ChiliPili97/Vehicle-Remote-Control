#include "DisplayUI.h"

DisplayUI::DisplayUI (uint8_t lcdAddr, uint8_t width, uint8_t height, uint8_t brightness)
  :   lcdAddr_(lcdAddr),
      width_(width),
      height_(height),
      brightness_(brightness)
{}

void DisplayUI::begin()
{
  Wire.begin();
  Wire.beginTransmission(lcdAddr_);

  if (Wire.endTransmission() == 0)
  {
    lcd_.begin(width_, height_);
    lcd_.setBacklight(255);
    initialized_ = true;
  }
}

void DisplayUI::loop(bool isConnected, int percent, Joystick::Dir dir) {
  currentState_.connected = isConnected;
  currentState_.percent = percent;
  currentState_.dir = dir;
  
  if (!initialized_) return;
  if (memcmp(&lastState_, &currentState_, sizeof(UIState)) == 0) return;
  if (!isConnected) screen_ = DisplayUI::Screen::Waiting;

   switch (screen_)
  {
    case DisplayUI::Screen::Waiting:
      if (!isConnected)  showWaiting_();
      else  screen_ = DisplayUI::Screen::Empty;
      break;

    case DisplayUI::Screen::Empty:
      lcd_.clear();
      screen_ = DisplayUI::Screen::Main;
      break;

    case DisplayUI::Screen::Main:
      showDirection_(dir);
      showBattery_(percent);
      break;
  }

  lastState_ = currentState_;
}

void DisplayUI::showWaiting_() { 
  lcd_.clear();
  lcd_.setCursor(0, 0);
  lcd_.print("waiting for");
  lcd_.setCursor(0, 1);
  lcd_.print("connecting");
}

void DisplayUI::showBattery_(int percent) {
  clearRow_(0);
  lcd_.setCursor(0, 0);
  lcd_.print("connected");
  lcd_.setCursor(12, 0);
  lcd_.print(percent);
  lcd_.print("%");
}

void DisplayUI::showDirection_(Joystick::Dir dir) {
  clearRow_(1);
  lcd_.setCursor(0, 1);
  switch (dir) {
    case Joystick::Dir::Forward:
      lcd_.print("forward");
      break;
    case Joystick::Dir::Backward:
      lcd_.print("backward");
      break;
    case Joystick::Dir::Left:
      lcd_.print("left");
      break;
    case Joystick::Dir::Right:
      lcd_.print("right");
      break;
    case Joystick::Dir::Halt:
      lcd_.print("halt");
      break;
  }
}

void DisplayUI::clearRow_(uint8_t row) {
  if (!initialized_ || row >= height_) return;
  lcd_.setCursor(0, row);
  for (uint8_t i = 0; i < width_; ++i) {
    lcd_.print(' ');
  }
}
