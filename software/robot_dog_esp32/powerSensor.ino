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

void updatePower() {
  voltage_V = powerSensorReady ? ina219.getBusVoltage_V() : 0.0;
  current_A = powerSensorReady ? ina219.getCurrent_mA() / 1000 : 0.0;
}

void setupPowerSensor()
{
  #if POWER_SENSOR == INA219
    ina219 = INA219_WE();

    if (ina219.init()) {
      powerSensorReady = true;
    } else {
      Serial.println("Failed to find INA219 chip");
    }

    ina219.setADCMode(BIT_MODE_9);  // just to make it fast
    ina219.setPGain(PG_320);
    ina219.setBusRange(BRNG_16);
  #endif
}

// return V
double getPowerSensorVoltage() {
  return voltage_V;
}


// return A
double getPowerSensorCurrent() {
  return current_A;
}
