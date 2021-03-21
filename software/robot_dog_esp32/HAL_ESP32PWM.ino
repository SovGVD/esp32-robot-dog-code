#if PWM_CONTROLLER_TYPE == ESP32PWM
int esp32pwmServos[12];

void initServoHAL() {
  Serial.print("ESP32_ISR ");
  Serial.println(ESP32_ISR_SERVO_VERSION);
  ESP32_ISR_Servos.useTimer(USE_ESP32_TIMER_NO);

  /**
   * Init servos
   */
  for (uint8_t i = 0; i < LEG_NUM; i++) {
    esp32pwmServos[i*3+0] = ESP32_ISR_Servos.setupServo(legs[i].hal.pin.alpha, SERVO_MIN, SERVO_MAX);
    esp32pwmServos[i*3+1] = ESP32_ISR_Servos.setupServo(legs[i].hal.pin.beta,  SERVO_MIN, SERVO_MAX);
    esp32pwmServos[i*3+2] = ESP32_ISR_Servos.setupServo(legs[i].hal.pin.gamma, SERVO_MIN, SERVO_MAX);
  }
}

void setLegPWM(leg &_leg)
{
  uint8_t l = _leg.id.id*3;
  ESP32_ISR_Servos.setPosition(esp32pwmServos[l+0], radToDeg(limitServoAngle(getHALAngle(_leg.angle.alpha, _leg.hal.mid.alpha, _leg.hal.trim.alpha, _leg.hal.ratio.alpha, _leg.inverse.alpha))));
  ESP32_ISR_Servos.setPosition(esp32pwmServos[l+1], radToDeg(limitServoAngle(getHALAngle(_leg.angle.beta,  _leg.hal.mid.beta,  _leg.hal.trim.beta,  _leg.hal.ratio.beta,  _leg.inverse.beta ))));
  ESP32_ISR_Servos.setPosition(esp32pwmServos[l+2], radToDeg(limitServoAngle(getHALAngle(_leg.angle.gamma, _leg.hal.mid.gamma, _leg.hal.trim.gamma, _leg.hal.ratio.gamma, _leg.inverse.gamma))));
//  Serial.print("Servo on leg ");
//  Serial.println(_leg.id.id);
//  Serial.print("Servo ID ");
//  Serial.println(esp32pwmServos[l+0]);
//  Serial.println(esp32pwmServos[l+1]);
//  Serial.println(esp32pwmServos[l+2]);
//  Serial.print("   ");
//  Serial.println(limitServoAngle(getHALAngle(_leg.angle.alpha, _leg.hal.mid.alpha, _leg.hal.trim.alpha, _leg.hal.ratio.alpha, _leg.inverse.alpha)));
//  Serial.print("   ");
//  Serial.println(limitServoAngle(getHALAngle(_leg.angle.beta,  _leg.hal.mid.beta,  _leg.hal.trim.beta,  _leg.hal.ratio.beta,  _leg.inverse.beta )));
//  Serial.print("   ");
//  Serial.println(limitServoAngle(getHALAngle(_leg.angle.gamma, _leg.hal.mid.gamma, _leg.hal.trim.gamma, _leg.hal.ratio.gamma, _leg.inverse.gamma)));
}

#endif
