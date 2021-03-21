int bytesToInt16(uint8_t byte1, uint8_t byte2) {
  return (byte1 << 8) | byte2;
}

void pMove(uint8_t* data) {
  if (data[1] == 0) setFailsafe();
  
  vector.move.x     = float(bytesToInt16(data[2], data[3]))/10000-1;
  vector.move.y     = float(bytesToInt16(data[4], data[5]))/10000-1;
  vector.move.z     = float(bytesToInt16(data[6], data[7]))/10000-1;
  vector.rotate.yaw = float(bytesToInt16(data[8], data[9]))/10000-1;
}
