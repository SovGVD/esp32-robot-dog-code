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

uint16_t angleToPulse(double angleRad) {
  double angleDeg = radToDeg(angleRad);

  if (angleDeg < servoMainProfile.minAngle) angleDeg = servoMainProfile.minAngle;
  if (angleDeg > servoMainProfile.maxAngle) angleDeg = servoMainProfile.maxAngle;

  // TODO how to make it better???

  if (angleDeg < 30) {
    return mapf(angleDeg, servoMainProfile.minAngle, 30, servoMainProfile.degMin, servoMainProfile.deg30);
  }

  if (angleDeg < 50) {
    return mapf(angleDeg, 30, 50, servoMainProfile.deg30, servoMainProfile.deg50);
  }

  if (angleDeg < 70) {
    return mapf(angleDeg, 50, 70, servoMainProfile.deg50, servoMainProfile.deg70);
  }

  if (angleDeg < 90) {
    return mapf(angleDeg, 70, 90, servoMainProfile.deg70, servoMainProfile.deg90);
  }

  if (angleDeg < 110) {
    return mapf(angleDeg, 90, 110, servoMainProfile.deg90, servoMainProfile.deg110);
  }

  if (angleDeg < 130) {
    return mapf(angleDeg, 110, 130, servoMainProfile.deg110, servoMainProfile.deg130);
  }

  if (angleDeg < 150) {
    return mapf(angleDeg, 130, 150, servoMainProfile.deg130, servoMainProfile.deg150);
  }

  if (angleDeg <= servoMainProfile.maxAngle) {
    return mapf(angleDeg, 150, servoMainProfile.maxAngle, servoMainProfile.deg150, servoMainProfile.degMax);
  }


  return 1500;  // TODO actualy we should fail here...
}


void setLegPWM(leg &_leg)
{
  uint8_t l = _leg.id.id*3;
  ESP32_ISR_Servos.setPulseWidth(esp32pwmServos[l+0], angleToPulse(limitServoAngle(getHALAngle(_leg.angle.alpha, _leg.hal.mid.alpha, _leg.hal.trim.alpha, _leg.hal.ratio.alpha, _leg.inverse.alpha))));
  ESP32_ISR_Servos.setPulseWidth(esp32pwmServos[l+1], angleToPulse(limitServoAngle(getHALAngle(_leg.angle.beta,  _leg.hal.mid.beta,  _leg.hal.trim.beta,  _leg.hal.ratio.beta,  _leg.inverse.beta ))));
  ESP32_ISR_Servos.setPulseWidth(esp32pwmServos[l+2], angleToPulse(limitServoAngle(getHALAngle(_leg.angle.gamma, _leg.hal.mid.gamma, _leg.hal.trim.gamma, _leg.hal.ratio.gamma, _leg.inverse.gamma))));

//  Serial.print("Servo on leg ");
//  Serial.println(_leg.id.id);
//  Serial.print("Servo ID ");
//  Serial.println(esp32pwmServos[l+0]);
//  Serial.println(esp32pwmServos[l+1]);
//  Serial.println(esp32pwmServos[l+2]);
//
//  Serial.print("   ");
//  Serial.print(angleToPulse(limitServoAngle(getHALAngle(_leg.angle.alpha, _leg.hal.mid.alpha, _leg.hal.trim.alpha, _leg.hal.ratio.alpha, _leg.inverse.alpha))));
//  Serial.print(" ");
//  Serial.println(radToDeg(limitServoAngle(getHALAngle(_leg.angle.alpha, _leg.hal.mid.alpha, _leg.hal.trim.alpha, _leg.hal.ratio.alpha, _leg.inverse.alpha))));
//
//  Serial.print("   ");
//  Serial.print(angleToPulse(limitServoAngle(getHALAngle(_leg.angle.beta,  _leg.hal.mid.beta,  _leg.hal.trim.beta,  _leg.hal.ratio.beta,  _leg.inverse.beta ))));
//  Serial.print(" ");
//  Serial.println(radToDeg(limitServoAngle(getHALAngle(_leg.angle.beta,  _leg.hal.mid.beta,  _leg.hal.trim.beta,  _leg.hal.ratio.beta,  _leg.inverse.beta ))));
//
//  Serial.print("   ");
//  Serial.print(angleToPulse(limitServoAngle(getHALAngle(_leg.angle.gamma, _leg.hal.mid.gamma, _leg.hal.trim.gamma, _leg.hal.ratio.gamma, _leg.inverse.gamma))));
//  Serial.print(" ");
//  Serial.println(radToDeg(limitServoAngle(getHALAngle(_leg.angle.gamma, _leg.hal.mid.gamma, _leg.hal.trim.gamma, _leg.hal.ratio.gamma, _leg.inverse.gamma))));
}

void runServoCalibrate(leg &_leg)
{
  uint8_t l = _leg.id.id*3;
  ESP32_ISR_Servos.setPulseWidth(esp32pwmServos[l+0], servoMainProfile.deg90);
  ESP32_ISR_Servos.setPulseWidth(esp32pwmServos[l+1], servoMainProfile.deg90);
  ESP32_ISR_Servos.setPulseWidth(esp32pwmServos[l+2], servoMainProfile.deg90);
}

#endif
