void initFailsafe () {
  
}

void updateFailsafe() {
  if (FS_WS_count >= FS_WS_THR) {
    setFailsafe();
    return;
  }
  
  FS_FAIL = false;
}

void setFailsafe() {
  vector.move.x     = 0;
  vector.move.y     = 0;
  vector.move.z     = 0;
  vector.rotate.yaw = 0;

  FS_WS_count = FS_WS_THR;  
  FS_FAIL     = true;
}
