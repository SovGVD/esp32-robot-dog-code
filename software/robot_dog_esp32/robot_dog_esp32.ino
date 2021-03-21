#include <math.h>

#include "libs/IK/geometry.h"
#include "libs/IK/leg.h"
#include "libs/IK/IK.h"
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

#include <Wire.h>

#include "libs/IK/IK.cpp"
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

#include "libs/MPU9250/MPU9250.h"

// run commands on diferent cores (FAST for main, SLOW for services)
bool runCommandFASTCore = false;
bool runCommandSLOWCore = false;
cliFunction cliFunctionFAST;
cliFunction cliFunctionSLOW;
double cliFunctionFASTVar = 0.0;
double cliFunctionSLOWVar = 0.0;

TaskHandle_t ServicesTask;

#if PWM_CONTROLLER_TYPE == PCA9685
  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#endif

unsigned long currentTime;
unsigned long previousTime;
unsigned long loopTime;

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

// CLI
Stream *cliSerial;

// Subscriptions
bool subscriptionEnabled = false;
bool subscriptionBinary = false;

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Wire.begin();

  delay(500);

  initSettings();
  initIMU();
  initHAL();
  initGait();
  initWiFi();
  initWebServer();
  
  xTaskCreatePinnedToCore(
    servicesLoop,   /* Task function. */
    "Services",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &ServicesTask,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
}

/**
   Main loop for all major things
   Core 1
*/
void loop()
{
  currentTime = millis();
  if (currentTime - previousTime >= LOOP_TIME) {
    previousTime = currentTime;

    updateFailsafe();
    updateIMU();
    updateGait();
    updateHAL();
    doHAL();

    updateWiFi();
    runFASTCommand();
    

    FS_WS_count++;

    loopTime = millis() - currentTime;
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
}

void servicesLoop(void * pvParameters) {
  servicesSetup();

  for (;;) {
    runSLOWCommand();
    updateCLI();
    doFASTSubscription();
    doSLOWSubscription();
    delay(100); // TODO use timer to implement do-FAST/SLOW-Subscription
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

*/
