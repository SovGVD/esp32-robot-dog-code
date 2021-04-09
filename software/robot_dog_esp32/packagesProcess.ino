int bytesToInt16(uint8_t byte1, uint8_t byte2) {
  return (byte1 << 8) | byte2;
}

void pMove(uint8_t* data) {
  if (data[1] == 0) setFailsafe();
  
  vector.move.x     = float(bytesToInt16( data[2],  data[3]))/10000-1;
  vector.move.y     = float(bytesToInt16( data[4],  data[5]))/10000-1;
  vector.move.z     = float(bytesToInt16( data[6],  data[7]))/10000-1;
  vector.rotate.yaw = float(bytesToInt16(data[12], data[13]))/10000-1;
}

void pTelemetry() {
  /**
   * Telemetry package map
   * offset | type | length | description
   * -------|------|--------|-------------
   *      0 | char |      1 | 'T' (P_TELEMETRY)
   *      1 |      |      1 | add something
   *      2 | bin  |      2 | 16 statuses
   *      4 | int  |      2 | voltage*1000
   *      6 | int  |      2 | current*1000
   *      8 | int  |      2 | loop time in microseconds
   */
  telemetryPackage[0] = P_TELEMETRY;
  // TODO statuses
  int16ToBytes(getPowerSensorVoltage()*1000, 4);
  int16ToBytes(getPowerSensorCurrent()*1000, 6);
  int16ToBytes(loopTime, 8);
}

void int16ToBytes(int value, uint8_t _offset)
{
  telemetryPackage[_offset]   = (value >> 8) & 0xFF;
  telemetryPackage[_offset+1] =       value  & 0xFF;
}
