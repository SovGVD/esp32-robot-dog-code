// HAL

// HAL - trim
double _cliGetHALTrim(leg &_leg, int angleId) {
  return radToDeg(getHALTrim(_leg, angleId));
}

double cliGetHALTrimLFAlpha(double id) {
  return _cliGetHALTrim(legs[LEGLF], ALPHA);
}
double cliGetHALTrimLFBeta(double id) {
  return _cliGetHALTrim(legs[LEGLF], BETA);
}
double cliGetHALTrimLFGamma(double id) {
  return _cliGetHALTrim(legs[LEGLF], GAMMA);
}
double cliGetHALTrimRFAlpha(double id) {
  return _cliGetHALTrim(legs[LEGRF], ALPHA);
}
double cliGetHALTrimRFBeta(double id) {
  return _cliGetHALTrim(legs[LEGRF], BETA);
}
double cliGetHALTrimRFGamma(double id) {
  return _cliGetHALTrim(legs[LEGRF], GAMMA);
}
double cliGetHALTrimLHAlpha(double id) {
  return _cliGetHALTrim(legs[LEGLH], ALPHA);
}
double cliGetHALTrimLHBeta(double id) {
  return _cliGetHALTrim(legs[LEGLH], BETA);
}
double cliGetHALTrimLHGamma(double id) {
  return _cliGetHALTrim(legs[LEGLH], GAMMA);
}
double cliGetHALTrimRHAlpha(double id) {
  return _cliGetHALTrim(legs[LEGRH], ALPHA);
}
double cliGetHALTrimRHBeta(double id) {
  return _cliGetHALTrim(legs[LEGRH], BETA);
}
double cliGetHALTrimRHGamma(double id) {
  return _cliGetHALTrim(legs[LEGRH], GAMMA);
}

// HAL - state

double cliGetHALState(double id)
{
  return isHALEnabled() ? 1.0 : 0.0;
}

// Leg angle

double _cliGetAngle(leg &_leg, int angleId)
{
  return getAngleDeg(_leg, angleId);
}

double cliGetAngleLFAlpha(double id) { return _cliGetAngle(legs[LEGLF], ALPHA); }
double cliGetAngleLFBeta(double id)  { return _cliGetAngle(legs[LEGLF], BETA);  }
double cliGetAngleLFGamma(double id) { return _cliGetAngle(legs[LEGLF], GAMMA); }

double cliGetAngleRFAlpha(double id) { return _cliGetAngle(legs[LEGRF], ALPHA); }
double cliGetAngleRFBeta(double id)  { return _cliGetAngle(legs[LEGRF], BETA);  }
double cliGetAngleRFGamma(double id) { return _cliGetAngle(legs[LEGRF], GAMMA); }

double cliGetAngleLHAlpha(double id) { return _cliGetAngle(legs[LEGLH], ALPHA); }
double cliGetAngleLHBeta(double id)  { return _cliGetAngle(legs[LEGLH], BETA);  }
double cliGetAngleLHGamma(double id) { return _cliGetAngle(legs[LEGLH], GAMMA); }

double cliGetAngleRHAlpha(double id) { return _cliGetAngle(legs[LEGRH], ALPHA); }
double cliGetAngleRHBeta(double id)  { return _cliGetAngle(legs[LEGRH], BETA);  }
double cliGetAngleRHGamma(double id) { return _cliGetAngle(legs[LEGRH], GAMMA); }

void _cliGetAngles(leg &_leg)
{
    cliSerial->print(radToDeg(_leg.angle.alpha), CLI_DP); cliSerial->print(" -> ");
    cliSerial->println(radToDeg(getHALAngle(_leg.angle.alpha, _leg.hal.mid.alpha, _leg.hal.trim.alpha, _leg.hal.ratio.alpha, _leg.inverse.alpha)), CLI_DP);

    cliSerial->print(radToDeg(_leg.angle.beta), CLI_DP); cliSerial->print(" -> ");
    cliSerial->println(radToDeg(getHALAngle(_leg.angle.beta,  _leg.hal.mid.beta,  _leg.hal.trim.beta,  _leg.hal.ratio.beta,  _leg.inverse.beta )), CLI_DP);

    cliSerial->print(radToDeg(_leg.angle.gamma), CLI_DP); cliSerial->print(" -> ");
    cliSerial->println(radToDeg(getHALAngle(_leg.angle.gamma, _leg.hal.mid.gamma, _leg.hal.trim.gamma, _leg.hal.ratio.gamma, _leg.inverse.gamma)), CLI_DP);
}

double cliGetAngles(double id)
{
  cliHideReturn = true;

   cliSerial->println("LF");
   _cliGetAngles(legs[LEGLF]);

   cliSerial->println("RF");
   _cliGetAngles(legs[LEGRF]);

   cliSerial->println("LH");
   _cliGetAngles(legs[LEGLH]);

   cliSerial->println("RH");
   _cliGetAngles(legs[LEGRH]);

   cliHideReturn = true;
   return 0.0;

}

double cliGetPower(double id)
{
  cliSerial->print(getPowerSensorVoltage(), 6);
  cliSerial->println(" V");
  cliSerial->print(getPowerSensorCurrent(), 6);
  cliSerial->println(" A");

  cliHideReturn = true;
  return 0.0;
}

double cliGetIMU(double id)
{
  cliSerial->print("Pitch: ");
  cliSerial->println(IMU_DATA[PITCH], 6);
  cliSerial->print("Roll: ");
  cliSerial->println(IMU_DATA[ROLL], 6);
  cliSerial->print("Yaw: ");
  cliSerial->println(IMU_DATA[YAW], 6);

  cliHideReturn = true;
  return 0.0;
}

double cliGetDebug(double id)
{

  switch((int)id) {
    case 1:
      cliSerial->print("Main loop: ");
      cliSerial->println(loopTime);

      cliSerial->print("Service fast loop: ");
      cliSerial->println(serviceFastLoopTime);

      cliSerial->print("Service loop: ");
      cliSerial->println(serviceLoopTime);

      break;

    case 2:
      cliSerial->print("Body   ");
      cliPrintPoint(body.position, 3);
      cliPrintAngle(body.orientation, 4);
      cliSerial->println();

      for (int i = 0; i < LEG_NUM; i++) {
        cliSerial->print("Leg ");
        cliSerial->print(i);
        cliSerial->print("  ");

        cliPrintPoint(legs[i].foot, 2);
        cliSerial->println();
      }
      break;

    default:
      cliSerial->println("Unknown debug ID");
  }

  cliHideReturn = true;
  return 0.0;
}


void cliPrintPoint(point p, int n)
{
  cliSerial->print("{");
  cliSerial->print(p.x, n);
  cliSerial->print(", ");
  cliSerial->print(p.y, n);
  cliSerial->print(", ");
  cliSerial->print(p.z, n);
  cliSerial->print("} ");
}

void cliPrintAngle(angle p, int n)
{
  cliSerial->print("{");
  cliSerial->print(degToRad(p.pitch), n);
  cliSerial->print(", ");
  cliSerial->print(degToRad(p.roll), n);
  cliSerial->print(", ");
  cliSerial->print(degToRad(p.yaw), n);
  cliSerial->print("} ");
}
