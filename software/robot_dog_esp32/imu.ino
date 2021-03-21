MPU9250 mpu;

#define ROLL  0
#define PITCH 1
#define YAW   2

float IMU_DATA[3] = {0, 0, 0};

void initIMU()
{
  Serial.print("IMU ");
  mpu.setup();
  Serial.println();
}

double calibrateIMU(double id)
{
  Serial.println("Calibrating ACC and GYRO in 5 seconds. Put device on flat leveled surface.");
  delay(5000);
  Serial.print("Calibration...");
  mpu.calibrateAccelGyro();
  Serial.println("Done.");

  return 1;
}

void updateIMU()
{
  mpu.update();

  IMU_DATA[ROLL]  = mpu.getRoll();
  IMU_DATA[PITCH] = mpu.getPitch();
  IMU_DATA[YAW]   = mpu.getYaw();
  
}

void displayIMU(int id)
{
//  display.print("Roll  |X| ");
//  display.println(IMU_DATA[ROLL], DISPLAY_DIGITS);
//  display.print("Pitch |Y| ");
//  display.println(IMU_DATA[PITCH], DISPLAY_DIGITS);
//  display.print("Yaw   |Z| ");
//  display.println(IMU_DATA[YAW], DISPLAY_DIGITS);
//  display.println("   X ^        +---+");
//  display.println("     |        |IMU|");
//  display.println(" Y <-Z        0---+");

}
