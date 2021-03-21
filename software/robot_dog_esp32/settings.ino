/**
 * Save and restore settings using EEPROM
 * 
 * EEPROM map
 *        0: version
 *        1: version revision
 *    2,3,4: LF alpha, beta, gamma
 *    5,6,7: RF alpha, beta, gamma
 *   8,9,10: LH alpha, beta, gamma
 * 11,12,13: RH alpha, beta, gamma
 *       14: WiFi mode (AP = 0 (default), 1...255 number of default WiFi AP to connect)
 */

#define EEPROM_VERSION 0x01
#define EEPROM_REV     0x01
#define EEPROM_OFFSET     2 // skip first value as it is version number
#define EEPROM_SIZE      32

// TODO
// - check EEPROM data structure version
// - print setting to Serial


void initSettings() {
  Serial.print("Settings ");

  EEPROM.begin(EEPROM_SIZE);

  settingsCheck();

  settingsLoad();

  Serial.println();
}

void settingsCheck() {
  if (EEPROM.read(0) == EEPROM_VERSION && EEPROM.read(1) == EEPROM_REV ) return;
  // TODO ask??
  settingsInitEEPROM();
}

void settingsInitEEPROM() {
  // In some cases that can be moving data from one version to another
  Serial.println("Re-init EEPROM");
  delay(5000);
  EEPROM.write(0, EEPROM_VERSION);
  EEPROM.write(1, EEPROM_REV);
  for (int i = 2; i < 14; i++) {
    EEPROM.write(i,128);  // Zeroes for trim
  }
  
  settingsSaveWiFi(AP_MODE); // Set AP mode
  
  for (int i = 15; i < EEPROM_SIZE; i++) {
    EEPROM.write(i,0);
  }
  settingsCommit();
}

void settingsCommit() {
  // TODO confirm save?
  EEPROM.commit();
}

double settingsUint8ToDouble(uint8_t value) {
  // This is for servo trimming, so the value of uint8_t is 0 to 255
  // can be converted to -0.25...+0.25 rad that should be enough for 
  // servo trimming, in other cases you are doing hardware wrong
  // (256 values with sign: 0 to 255 -> -127...126 -> /500 and rad->deg -> -14.6...14.5 deg)
  return ((double)value-128)/500;
}

uint8_t settingsDoubleToUint8(double value) {
  // see settingsUint8ToDouble();
  if (value >=  LEG_TRIM_LIMIT) value =  LEG_TRIM_LIMIT;
  if (value <= -LEG_TRIM_LIMIT) value = -LEG_TRIM_LIMIT;
  
  return value*500+128;
}


void settingsLoad() {
  settingsLoadTrim();
  WiFiMode = settingsLoadWiFi();
}

// leg trim values
void settingsLoadTrim() {
  //0 leg trim {-0.05, -0.15,  0.20}
  //1 leg trim { 0.12, -0.10, -0.18}
  //2 leg trim {-0.02, -0.21, -0.06}
  //3 leg trim { 0.04, -0.12, -0.06}

  Serial.println();
  for (int i = 0; i < LEG_NUM; i++) {
    legs[i].hal.trim = settingsLoadTrimLeg(legs[i]);
    Serial.print(i);
    Serial.print(" leg trim {");
    Serial.print(radToDeg(legs[i].hal.trim.alpha), CLI_DP);
    Serial.print(", ");
    Serial.print(radToDeg(legs[i].hal.trim.beta),  CLI_DP);
    Serial.print(", ");
    Serial.print(radToDeg(legs[i].hal.trim.gamma), CLI_DP);
    Serial.println("}");
  }
}

legangle settingsLoadTrimLeg(leg &_leg){
  int offset = _leg.id.id*3+EEPROM_OFFSET;
  return {
    settingsUint8ToDouble(EEPROM.read(offset)),
    settingsUint8ToDouble(EEPROM.read(offset+1)),
    settingsUint8ToDouble(EEPROM.read(offset+2))
  };
}

void settingsSaveTrimLeg(leg &_leg) {
  int offset = _leg.id.id*3+EEPROM_OFFSET;
  EEPROM.write(offset,   settingsDoubleToUint8(_leg.hal.trim.alpha));
  EEPROM.write(offset+1, settingsDoubleToUint8(_leg.hal.trim.beta));
  EEPROM.write(offset+2, settingsDoubleToUint8(_leg.hal.trim.gamma));
  settingsCommit();
}

// WiFi
int settingsLoadWiFi() {
  return (int)EEPROM.read(14);
}

int settingsSaveWiFi(int id) {
  EEPROM.write(14, id);
  settingsCommit();
  return id;
}
