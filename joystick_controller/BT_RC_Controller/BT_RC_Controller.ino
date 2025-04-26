#include "BTLink.h"
#include "DisplayUI.h"
#include "Joystick.h"

BTLink btLink("HC-05");
DisplayUI displayUI(0x27);
Joystick stick(25, 26, 1000, 3000, 1000, 3000);
Joystick::Dir dir, lastDir = Joystick::Dir::Halt;
int percent = -1;
bool isConnected = false;

void setup() {
  Serial.begin(115200);
  btLink.begin("ESP32_Master");
  displayUI.begin();
}

void loop() {
  btLink.loop();
  
  isConnected = btLink.isConnected();
  dir = stick.read();
  percent = btLink.battery();

  displayUI.loop(isConnected, percent, dir);
 
  if (dir != lastDir)
  {
    if (isConnected)  btLink.sendCommand(dir);
    lastDir = dir;
  }
}
