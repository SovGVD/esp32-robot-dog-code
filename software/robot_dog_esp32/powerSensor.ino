bool powerSensorReady = false;
void initPowerSensor() {
  Serial.print("Power sensor ");
  #if POWER_SENSOR == INA219
    Serial.print("INA219");
  #else
    Serial.print("<NO>");
  #endif
  setupPowerSensor();
  Serial.println();
}

// TODO this dies not work fine for some reason, ESP just reboot
//      a lot of other stuff does not work, e.g. Captive portal, Servos randomly moving, probablt PWM lib does not work fine
void setupPowerSensor()
{
  #if POWER_SENSOR == INA219
    if (ina219.init()) {
      powerSensorReady = true;
    } else {
      Serial.println("Failed to find INA219 chip");
    }
    ina219.setMeasureMode(TRIGGERED);
//    ina219.setADCMode(BIT_MODE_9);
//    ina219.setPGain(PG_320);
//    ina219.setBusRange(BRNG_32);
  #endif
}

// return Volts
double getPowerSensorVoltage() {
  #if POWER_SENSOR == INA219
    ina219.startSingleMeasurement();
    return powerSensorReady ? ina219.getBusVoltage_V() : 0.0;
  #else
    return 0.0;
  #endif
}


// return mA
double getPowerSensorCurrent() {
  #if POWER_SENSOR == INA219
    ina219.startSingleMeasurement();  // TODO not great... need to fetch it together or cache!!!
    return powerSensorReady ? ina219.getCurrent_mA() : 0.0;
  #else
    return 0.0;
  #endif
}
