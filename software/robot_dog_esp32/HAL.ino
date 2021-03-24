void initHAL()
{
  Serial.print("HAL ");
  initServo();
  initLegs();
  Serial.println();
}

void initLegs() {

}

void updateHAL() {
  updateLegs();
}

void doHAL() {
  //if (!isHALEnabled()) return;
  servoSet();
}

void readLegsSensors() {
  // TODO that will be i2c ADC, not analog read
  // TODO enable if sensors available
  // TODO don't set to legs, this should be part of gait
  for (int i = 0; i < LEG_NUM; i++) {
    //legs[i].sensor.onGround = analogRead(legs[i].sensor.pin) > legs[i].sensor.threshold;
  }
}

void updateLegs() {
  if (!isHALEnabled()) return;

  readLegsSensors();
  // TODO check `isSolved`
  #ifdef DEBUG_HAL_LEG
    Serial.println("LEGLF");
  #endif
  legs[LEGLF].angle = ikLegLF.solve().angle;
  
  #ifdef DEBUG_HAL_LEG
    Serial.println("LEGRF");
  #endif
  legs[LEGRF].angle = ikLegRF.solve().angle;
  
  #ifdef DEBUG_HAL_LEG
    Serial.println("LEGLH");
  #endif
  legs[LEGLH].angle = ikLegLH.solve().angle;

  #ifdef DEBUG_HAL_LEG
    Serial.println("LEGRH");
  #endif
  legs[LEGRH].angle = ikLegRH.solve().angle;
}

double limitServoAngle(double angle)
{
  // TODO this is not right solution, we should restart calc to solve the angle instead of just limit if something wrong
  //      also this is nothing about real mechanical limits, so need TODO
  
  // default servo can handle angles from 0 to 180 (PI) degrees only
  if (angle < 0) angle = 0;
  if (angle > PI) angle = PI;

  return angle;
}

double getHALAngle(double angle, double mid, double trimAngle, double gearRatio, bool inverse) {
  angle = (angle - mid) * gearRatio + M_PI_2;
  if (inverse) angle = M_PI - angle;
  angle = angle + trimAngle;
  return angle;
}

double getHALTrim (leg &_leg, int angleId)
{
  switch (angleId) {
    case ALPHA:
      return _leg.hal.trim.alpha;
      break;
    case BETA:
      return _leg.hal.trim.beta;
      break;
    case GAMMA:
      return _leg.hal.trim.gamma;
      break;
  }

  return 0;
}

bool setHALTrim (leg &_leg, int angleId, double deg)
{
  double rad = degToRad(deg);
  if (rad >= LEG_TRIM_LIMIT || rad <= -LEG_TRIM_LIMIT) {
    return false;
  }
  
  switch (angleId) {
    case ALPHA:
      _leg.hal.trim.alpha = rad;
      break;
    case BETA:
      _leg.hal.trim.beta  = rad;
      break;
    case GAMMA:
      _leg.hal.trim.gamma = rad;
      break;
  }

  settingsSaveTrimLeg(_leg);
  _leg.hal.trim = settingsLoadTrimLeg(_leg);

  return true;
}

bool isHALEnabled()
{
  // This not correct, we are not enbale/disable HAL, but just disable servo position calculations
  return HALEnabled;
}
