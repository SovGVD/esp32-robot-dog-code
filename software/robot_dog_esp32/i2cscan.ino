double runI2CScan(double id) {
  // We need to run that function on different cores
  cliFunctionFAST = runI2CScanService;
  runCommandFASTCore = true;

  return 1;
}

double runI2CScanService(double id)
{
  cliSerial->println("i2c scan");
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);        // Begin I2C transmission Address (i)
    if (Wire.endTransmission () == 0)  // Receive 0 = success (ACK response) 
    {
      cliSerial->print(i, DEC);
      cliSerial->print(" (0x");
      cliSerial->print(i, HEX);
      cliSerial->print("), ");
    }
  }

  return 1;
}
