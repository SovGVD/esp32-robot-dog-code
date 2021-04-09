#include <math.h>

#include "libs/IK/geometry.h"
#include "libs/IK/leg.h"
#include "libs/IK/IK_simple.h"  // TODO this is for small dog only!!!
#include "libs/planner/planner.h"
#include "libs/balance/balance.h"
#include "libs/gait/gait.h"

#include "def.h"
#include "config.h"
#include "config_small.h"
#include "config_wifi.h"
#include <EEPROM.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "web/index.html.gz.h"
#include "cli.h"
#include "subscription.h"

#include <MPU9250_WE.h>
#include <Wire.h>

#include "libs/IK/IK_simple.cpp"  // TODO this is for small dog only!!!
#include "libs/planner/planner.cpp"
#include "libs/balance/balance.cpp"
#include "libs/gait/gait.cpp"

/**
 * Hardware libraries
 */
#if PWM_CONTROLLER_TYPE == PCA9685
  #include <Adafruit_PWMServoDriver.h>
#endif

#if PWM_CONTROLLER_TYPE == ESP32PWM
  #define USE_ESP32_TIMER_NO 3
  #include "ESP32_ISR_Servo.h"
#endif

#ifdef POWER_SENSOR
  float voltage_V = 0.0;
  float current_A = 0.0;

  #if POWER_SENSOR == INA219
    #include <INA219_WE.h>
    INA219_WE ina219;
  #endif
#endif

float IMU_DATA[3] = {0, 0, 0};
MPU9250_WE IMU;

// run commands on diferent cores (FAST for main, SLOW for services)
bool runCommandFASTCore = false;
bool runCommandSLOWCore = false;
cliFunction cliFunctionFAST;
cliFunction cliFunctionSLOW;
double cliFunctionFASTVar = 0.0;
double cliFunctionSLOWVar = 0.0;

TaskHandle_t ServicesTask;

#if PWM_CONTROLLER_TYPE == PCA9685
  Adafruit_PWMServoDriver pwm;
#endif

unsigned long currentTime;
unsigned long previousTime;
unsigned long loopTime;

unsigned long serviceCurrentTime;

unsigned long servicePreviousTime;
unsigned long serviceLoopTime;

unsigned long serviceFastPreviousTime;
unsigned long serviceFastLoopTime;

bool HALEnabled = true;

// Gait
uint16_t ticksPerGaitItem    = 0;
uint16_t ticksToNextGaitItem = 0;
uint8_t  currentGait         = 0;
uint8_t  nextGait            = 0;
double   gaitItemProgress    = 0;
double   gaitProgress[]      = {0, 0, 0, 0};

transition bodyTransition;
transitionParameters bodyTransitionParams = {{0,0,0}, {0,0,0}, 0};

//Move
moveVector vector = {
  {0, 0, 0},
  {0, 0, 0}
};

//Failsafe
bool FS_FAIL = false;
uint8_t FS_WS_count = 0;

// HAL
figure body = {
  {  0,  0,  0},
  {  0,  0,  0}
};

IK ikLegLF(legs[LEGLF], body);
IK ikLegRF(legs[LEGRF], body);
IK ikLegLH(legs[LEGLH], body);
IK ikLegRH(legs[LEGRH], body);

/* We need predict future position of legs/body */
planner walkPlanner(
  vector,
  body,
  legs[LEGLF],
  legs[LEGRF],
  legs[LEGLH],
  legs[LEGRH]
);

/* and balance it someway */
balance bodyBalance(
  balanceOffset,
  body,
  legs[LEGLF],
  legs[LEGRF],
  legs[LEGLH],
  legs[LEGRH]
);

// WebServer
bool clientOnline = false;
int WiFiMode = AP_MODE;
IPAddress WiFiIP;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
uint8_t telemetryPackage[P_TELEMETRY_LEN];

// CLI
Stream *cliSerial;

// Subscriptions
bool subscriptionEnabled = false;
bool subscriptionBinary = false;


void setup()
{
  Serial.begin(SERIAL_BAUD);
  delay(100);

  initSettings();
  delay(100);
  
  initHAL();
  delay(100);
  
  initGait();
  delay(100);
  
  initWiFi();
  delay(100);
  
  initWebServer();
  delay(100);
  
  xTaskCreatePinnedToCore(
    servicesLoop,   /* Task function. */
    "Services",     /* name of task. */
    100000,         /* Stack size of task */
    NULL,           /* parameter of the task */
    1,              /* priority of the task */
    &ServicesTask,  /* Task handle to keep track of created task */
    0);             /* pin task to core 0 */
}

/**
   Main loop for all major things
   Core 1
*/
void loop()
{
  currentTime = micros();
  if (currentTime - previousTime >= LOOP_TIME) {
    previousTime = currentTime;

    updateFailsafe();
    updateGait();
    updateHAL();
    doHAL();

    updateWiFi();
    

    FS_WS_count++;

    loopTime = micros() - currentTime;
    if (loopTime > LOOP_TIME) {
      Serial.print("WARNING! Increase LOOP_TIME: ");
      Serial.println(loopTime);
    }
  }
}

/**
   Loop for service things, like CLI
   Core 0
*/
void servicesSetup() {
  cliSerial = &Serial;
  initCLI();
  initSubscription();
  Wire.begin();
  Wire.setClock(400000);
  delay(100);

  initIMU();
  delay(100);
  initPowerSensor();
  delay(100);
}

void servicesLoop(void * pvParameters) {
  servicesSetup();

  for (;;) {
    serviceCurrentTime = micros();

    if (serviceCurrentTime - serviceFastPreviousTime >= SERVICE_FAST_LOOP_TIME) {
      serviceFastPreviousTime = serviceCurrentTime;

      updateIMU();
      
      runFASTCommand();
      doFASTSubscription();

      serviceFastLoopTime = micros() - serviceCurrentTime;
      if (serviceFastLoopTime > LOOP_TIME) {
        Serial.print("WARNING! Increase SERVICE_FAST_LOOP_TIME: ");
        Serial.println(serviceFastLoopTime);
      }      
    }

    if (serviceCurrentTime - servicePreviousTime >= SERVICE_LOOP_TIME) {
      servicePreviousTime = serviceCurrentTime;

      updatePower();
      runSLOWCommand();
      updateCLI();
      doSLOWSubscription();

      serviceLoopTime = micros() - serviceCurrentTime;  // this loop + service fast loop
      if (serviceLoopTime > LOOP_TIME) {
        Serial.print("WARNING! Increase SERVICE_LOOP_TIME: ");
        Serial.println(serviceLoopTime);
      }

    }
    vTaskDelay(1);  // https://github.com/espressif/arduino-esp32/issues/595
  }
}

void runFASTCommand()
{
  if (runCommandFASTCore) {
    runCommandFASTCore = false;
    cliFunctionFAST(cliFunctionFASTVar);
  }
}

void runSLOWCommand()
{
  if (runCommandSLOWCore) {
    runCommandSLOWCore = false;
    cliFunctionSLOW(cliFunctionSLOWVar);
  }
}

/**
   TODO
    - calculate center of mass and use it for balance
    - make the queue of tasks by core, e.g. not just   cliFunctionFAST = runI2CScanFAST; but array/list with commands
    - i2c pwm broken, i2c in service loop

*/
