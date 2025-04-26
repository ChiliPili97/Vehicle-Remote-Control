#pragma once
#include "Arduino.h"
#include "BluetoothSerial.h"
#include "Joystick.h"

class BTLink
{
  public:
    /**
       @param slaveName              — name of the device with which want to establish link
       @param reconnectingInterval   — time interval for reconnecting to slave device, in ms
       @param connectedTimeout       — connected timeout. in ms
    */
    BTLink (const char* slaveName, uint32_t reconnectInterval = 5000, uint32_t connectedTimeout = 1000);

    void begin(const char* masterName);
    void loop();
    bool isConnected();

    void sendCommand(Joystick::Dir dir);
    int battery() const;

  private:
    BluetoothSerial bt_;
    const char* slaveName_;
    const uint32_t reconnectInterval_;
    const uint32_t connectedTimeout_;

    unsigned long lastAttempt_ = 0;
    int battery_ = -1;

    void tryConnect_();
    void handleIncoming_();
};
