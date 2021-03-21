double runI2CScan(double id) {
  // We need to run that function on diferent cores
  cliFunctionFAST = runI2CScanFAST;
  runCommandFASTCore = true;

  return 1;
}

double runI2CScanFAST(double id)
{
  cliSerial->println("i2c scan FAST");
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
