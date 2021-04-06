void initIMU()
{
  Serial.print("IMU ");
  setupIMU();
  Serial.println();
}

void setupIMU()
{
  IMU = MPU9250_WE(IMU_ADDRESS);
  
  if(!IMU.init()){
    Serial.println("IMU does not respond");
  }

  IMU.setAccRange(MPU9250_ACC_RANGE_2G);
  IMU.enableAccDLPF(true);
  IMU.setAccDLPF(MPU9250_DLPF_6);
}

double calibrateIMU(double id)
{
  Serial.println("Calibrating ACC and GYRO in 5 seconds. Put device on flat leveled surface.");
  delay(5000);
  Serial.print("Calibration...");
  IMU.autoOffsets();
  Serial.println("Done.");

  return 1;
}

void updateIMU()
{
  xyzFloat angle = IMU.getAngles();

  // TODO not sure about correct mapping!!!
  IMU_DATA[ROLL]  = angle.x;
  IMU_DATA[PITCH] = angle.y;
  IMU_DATA[YAW]   = angle.z;
  
}
