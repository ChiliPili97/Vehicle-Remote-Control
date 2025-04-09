#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

#define X_PIN       25 
#define Y_PIN       35
#define LED_CI_PIN  2   //LED indicator of the connected slave device

#define LEFT_THRESHOLD      1000
#define RIGHT_THRESHOLD     3000
#define FORWARD_THRESHOLD   1000
#define BACKWARD_THRESHOLD  3000

#define HALT      0x00
#define LEFT      0x01
#define RIGHT     0x02
#define FORWARD   0x04
#define BACKWARD  0x08

int valueX = 0 ; 
int valueY = 0 ; 
int command = HALT;
int oldCommand = HALT;
bool connected = false;
String name = "HC-05";

void setup() {

    Serial.begin(115200);
    SerialBT.begin("ESP32_Master", true);  // true = Master mode
    connected = SerialBT.connect(name);
    if (connected) {
        Serial.println("Connected to HC-05");
    } else {
        Serial.println("Failed to connect");
    }
    
    // Set the ADC attenuation to 11 dB (up to ~3.3V input)
    analogSetAttenuation(ADC_11db);
    pinMode(LED_CI_PIN, OUTPUT);
    pinMode(4, OUTPUT);
}

void loop() {
  static unsigned long lastReconnectAttempt = 0;
  connected = SerialBT.connected();
  
  if (connected)
    digitalWrite(LED_CI_PIN, HIGH);
  else
  {
    digitalWrite(LED_CI_PIN, LOW);
    
    unsigned long now = millis();
    if (now - lastReconnectAttempt > 5000) {  // Retry every 5 seconds
      Serial.println("Attempting to reconnect to HC-05...");
      connected = SerialBT.connect(name);
      if (connected) {
        Serial.println("Reconnected to HC-05");
      } else {
        Serial.println("Failed to reconnect");
      }
      lastReconnectAttempt = now;
    }
    return; // Skip further processing if not connected
  }
  
  // Read joystick values
  int valueX = analogRead(X_PIN);
  int valueY = analogRead(Y_PIN);

  // Convert analog values to commands
  if (valueY < FORWARD_THRESHOLD)
    command = FORWARD;
  else if (valueY > BACKWARD_THRESHOLD)
    command = BACKWARD;
  else if (valueX < LEFT_THRESHOLD)
    command = LEFT;
  else if (valueX > RIGHT_THRESHOLD)
    command = RIGHT;
  else
    command = HALT;

  // Send command only if it changes
  if (command != oldCommand) {
    switch (command) {
      case FORWARD:
        Serial.println("COMMAND FORWARD");
        SerialBT.println("F");
        break;
      case BACKWARD:
        Serial.println("COMMAND BACKWARD");
        SerialBT.println("B");
        break;
      case LEFT:
        Serial.println("COMMAND LEFT");
        SerialBT.println("L");
        break;
      case RIGHT:
        Serial.println("COMMAND RIGHT");
        SerialBT.println("R");
        break;
      case HALT:
        Serial.println("COMMAND HALT");
        SerialBT.println("H");
        break;
    }
    
    oldCommand = command;  // Update only after sending a new command
  }
}
