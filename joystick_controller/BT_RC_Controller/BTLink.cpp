#include "BTLink.h"

BTLink::BTLink (const char* slaveName, uint32_t reconnectInterval, uint32_t connectedTimeout)
  : slaveName_(slaveName),
    reconnectInterval_(reconnectInterval),
    connectedTimeout_(connectedTimeout)
{}

//PUBLIC
void BTLink::begin(const char* masterName)
{
  bt_.begin(masterName, true);
  tryConnect_();
}

void BTLink::loop()
{
  if (bt_.connected(connectedTimeout_))
  {
    handleIncoming_();
    return;
  }

  unsigned long now = millis();
  if (now - lastAttempt_ >= reconnectInterval_)
  {
    tryConnect_();
    lastAttempt_ = now;
  }
}

bool BTLink::isConnected()
{
  return bt_.connected(connectedTimeout_);
}

void BTLink::sendCommand(Joystick::Dir dir)
{
  if (!isConnected())
    return;

  switch (dir)
  {
    case Joystick::Dir::Forward:   bt_.print('F'); break;
    case Joystick::Dir::Backward:  bt_.print('B'); break;
    case Joystick::Dir::Left:      bt_.print('L'); break;
    case Joystick::Dir::Right:     bt_.print('R'); break;
    case Joystick::Dir::Halt:      bt_.print('H'); break;
  }
}

int BTLink::battery() const
{
  return battery_;
}



//PRIVATE
void BTLink::tryConnect_()
{
  Serial.printf("[BT] Connecting to %s ...\n", slaveName_);
  if (bt_.connect(slaveName_))
    Serial.println("[BT] Connected!");
  else
    Serial.println("[BT] Failed");
}

void BTLink::handleIncoming_()
{
  if (bt_.available())
  {
    int b = bt_.read();
    if (b >= 0 && b <= 100)
    {
      battery_ = b;
      Serial.printf("[BT] Battery %d%%\n", battery_);
    }
  }
}
