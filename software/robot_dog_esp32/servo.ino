float testAngle = 0;
float testAngleInc = 0.5;
bool testAngleWay = true;


void initServo()
{
  Serial.print("Servo ");

  initServoHAL();

  Serial.println();
}

void setServoToMiddle()
{
  for (uint8_t i = 0; i < LEG_NUM; i++) {
    runServoCalibrate(legs[i]);
  }
}


void servoSet()
{
  for (uint8_t i = 0; i < LEG_NUM; i++) {
    setLegPWM(legs[i]);
  }
}

bool setAngleDeg(leg &_leg, int angleId, double deg)
{
  // TODO limits?
  double rad = degToRad(deg);
  switch (angleId) {
    case ALPHA:
      _leg.angle.alpha = rad;
      break;
    case BETA:
      _leg.angle.beta  = rad;
      break;
    case GAMMA:
      _leg.angle.gamma = rad;
      break;
  }

  return true;
}

double getAngleDeg(leg &_leg, int angleId)
{
  switch (angleId) {
    case ALPHA:
      return radToDeg(_leg.angle.alpha);
      break;
    case BETA:
      return radToDeg(_leg.angle.beta);
      break;
    case GAMMA:
      return radToDeg(_leg.angle.gamma);
      break;
  }

  return 0.0;
}
