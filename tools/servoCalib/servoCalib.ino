
// Select different ESP32 timer number (0-3) to avoid conflict
#define USE_ESP32_TIMER_NO          3

#include "ESP32_ISR_Servo.h"

int servoIndex = -1;
String inputString = "";
bool stringComplete = false;

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting ESP32_ISR_MultiServos on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_ISR_SERVO_VERSION);
  
  //Select ESP32 timer USE_ESP32_TIMER_NO
  ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);

  servoIndex = ESP32_ISR_Servos.setupServo(14, 0, 3000);

  ESP32_ISR_Servos.setPulseWidth(servoIndex, 1500);

  inputString.reserve(200);
}

void loop()
{
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }


  if (stringComplete) {
    ESP32_ISR_Servos.setPulseWidth(servoIndex, inputString.toInt());
    Serial.println(inputString.toInt());
    inputString = "";
    stringComplete = false;
  }
}
