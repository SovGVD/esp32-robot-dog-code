#define CLI_MENU_COMMANDS_GET 30
#define CLI_MENU_COMMANDS_SET 28
#define CLI_MENU_COMMANDS_RUN 5

char cliChar;
bool cliErrorState = false;
bool cliHideReturn = false;
char cliValues[CLI_VALUES][CLI_VALUE_LEN];
unsigned int cliPosCommand = 0;
unsigned int cliPosState = 0;

const cliCommand cliCommandsGet[CLI_MENU_COMMANDS_GET] = {
  { "help",              cliGetHelp           },
  { "LF_HAL_trim_alpha", cliGetHALTrimLFAlpha },
  { "LF_HAL_trim_beta",  cliGetHALTrimLFBeta  },
  { "LF_HAL_trim_gamma", cliGetHALTrimLFGamma },
  { "RF_HAL_trim_alpha", cliGetHALTrimRFAlpha },
  { "RF_HAL_trim_beta",  cliGetHALTrimRFBeta  },
  { "RF_HAL_trim_gamma", cliGetHALTrimRFGamma },
  { "LH_HAL_trim_alpha", cliGetHALTrimLHAlpha },
  { "LH_HAL_trim_beta",  cliGetHALTrimLHBeta  },
  { "LH_HAL_trim_gamma", cliGetHALTrimLHGamma },
  { "RH_HAL_trim_alpha", cliGetHALTrimRHAlpha },
  { "RH_HAL_trim_beta",  cliGetHALTrimRHBeta  },
  { "RH_HAL_trim_gamma", cliGetHALTrimRHGamma },
  { "HAL",               cliGetHALState       },
  { "LF_angle_alpha",    cliGetAngleLFAlpha   },
  { "LF_angle_beta",     cliGetAngleLFBeta    },
  { "LF_angle_gamma",    cliGetAngleLFGamma   },
  { "RF_angle_alpha",    cliGetAngleRFAlpha   },
  { "RF_angle_beta",     cliGetAngleRFBeta    },
  { "RF_angle_gamma",    cliGetAngleRFGamma   },
  { "LH_angle_alpha",    cliGetAngleLHAlpha   },
  { "LH_angle_beta",     cliGetAngleLHBeta    },
  { "LH_angle_gamma",    cliGetAngleLHGamma   },
  { "RH_angle_alpha",    cliGetAngleRHAlpha   },
  { "RH_angle_beta",     cliGetAngleRHBeta    },
  { "RH_angle_gamma",    cliGetAngleRHGamma   },
  { "angles",            cliGetAngles         },
  { "power",             cliGetPower          },
  { "imu",               cliGetIMU            },
  { "debug",             cliGetDebug          }
};

const cliCommand cliCommandsSet[CLI_MENU_COMMANDS_SET] = {
  { "help",              cliSetHelp           },
  { "LF_HAL_trim_alpha", cliSetHALTrimLFAlpha },
  { "LF_HAL_trim_beta",  cliSetHALTrimLFBeta  },
  { "LF_HAL_trim_gamma", cliSetHALTrimLFGamma },
  { "RF_HAL_trim_alpha", cliSetHALTrimRFAlpha },
  { "RF_HAL_trim_beta",  cliSetHALTrimRFBeta  },
  { "RF_HAL_trim_gamma", cliSetHALTrimRFGamma },
  { "LH_HAL_trim_alpha", cliSetHALTrimLHAlpha },
  { "LH_HAL_trim_beta",  cliSetHALTrimLHBeta  },
  { "LH_HAL_trim_gamma", cliSetHALTrimLHGamma },
  { "RH_HAL_trim_alpha", cliSetHALTrimRHAlpha },
  { "RH_HAL_trim_beta",  cliSetHALTrimRHBeta  },
  { "RH_HAL_trim_gamma", cliSetHALTrimRHGamma },
  { "HAL",               cliSetHALState       },
  { "LF_angle_alpha",    cliSetAngleLFAlpha   },
  { "LF_angle_beta",     cliSetAngleLFBeta    },
  { "LF_angle_gamma",    cliSetAngleLFGamma   },
  { "RF_angle_alpha",    cliSetAngleRFAlpha   },
  { "RF_angle_beta",     cliSetAngleRFBeta    },
  { "RF_angle_gamma",    cliSetAngleRFGamma   },
  { "LH_angle_alpha",    cliSetAngleLHAlpha   },
  { "LH_angle_beta",     cliSetAngleLHBeta    },
  { "LH_angle_gamma",    cliSetAngleLHGamma   },
  { "RH_angle_alpha",    cliSetAngleRHAlpha   },
  { "RH_angle_beta",     cliSetAngleRHBeta    },
  { "RH_angle_gamma",    cliSetAngleRHGamma   },
  { "servo_calib",       cliSetServoCalib     },
  { "servo_to_init",     cliSetServoToInit    }
};

const cliCommand cliCommandsRun[CLI_MENU_COMMANDS_RUN] = {
  { "help",         cliRunHelp        },
  { "i2cscan",      runI2CScan        },
  { "calibrateimu", calibrateIMU      },
  { "wifiinfo",     WiFiInfo          },
  { "sbscr",        subscriptionState }
};

void initCLI() {
  cliInitHelp();
}

void cliInitHelp()
{
  cliSerial->println();
  cliSerial->println(" CLI: type `get|set|run|sbs help` to see available methods.");
  cliSerial->println();

}

void updateCLI()
{
  if (cliSerial->available()) {
    cliChar = cliSerial->read();
    
    if (cliPosCommand >= CLI_VALUES || cliPosState >= CLI_VALUE_LEN) {
      cliFailCommand();
    }
    
    switch (cliChar) {
      case 10:
      case 13:
        cliRunCommand();
        break;
        
      case 32:
        cliPosState = 0;
        cliPosCommand++;
        break;
        
      default:
        cliValues[cliPosCommand][cliPosState] = cliChar;
        cliPosState++;
        break;
    }
  }
}

void cliRunCommand()
{
  cliSerial->print(CLI_TERMINAL_SIGN);
  if (strcmp(cliValues[0], "get") == 0) {
    cliRunCommandGet();
  } else if (strcmp(cliValues[0], "set") == 0) {
    cliRunCommandSet();
  } else if (strcmp(cliValues[0], "run") == 0) {
    cliRunCommandRun();
  } else if (strcmp(cliValues[0], "sbs") == 0) {
    cliRunCommandSubscription();
  } else {
    //cliUnknownCommand();
  }

  cliClearError();
  cliClearCommand();
}

void cliUnknownCommand()
{
  cliSerial->println("UNKNOWN");
  cliInitHelp();
}

void cliRunCommandGet() {
  cliSerial->print("GET ");
  cliSerial->println(cliValues[1]);
  for (int i = 0; i < CLI_MENU_COMMANDS_GET; i++) {
    if (strcmp(cliValues[1], cliCommandsGet[i].commandName) == 0) {
      double r = cliCommandsGet[i].func(atof(cliValues[2]));
      if (!cliErrorState && !cliHideReturn) {
        cliSerial->println(r, CLI_DP);
      }
    }
  }
  cliHideReturn = false;
}

void cliRunCommandSubscription() {
  cliSerial->print("SBS ");
  cliSerial->println(cliValues[1]);
  cliSerial->println(cliValues[2]);
  cliSerial->println(cliValues[3]);
  for (int i = 1; i < CLI_MENU_COMMANDS_GET; i++) { // ignore `help`
    if (strcmp(cliValues[1], cliCommandsGet[i].commandName) == 0) {
      subscribe(i, atoi(cliValues[2]) == 1, atoi(cliValues[3]) == 1);
    }
  }
  cliHideReturn = false;
}


void cliRunCommandSet() {
  cliSerial->print("SET ");
  cliSerial->print(cliValues[1]);
  cliSerial->print(" ");
  cliSerial->println(atof(cliValues[2]), CLI_DP);
  for (int i = 0; i < CLI_MENU_COMMANDS_SET; i++) {
    if (strcmp(cliValues[1], cliCommandsSet[i].commandName) == 0) {
      double r = cliCommandsSet[i].func(atof(cliValues[2]));
      if (!cliErrorState && !cliHideReturn) {
        cliSerial->println(r, CLI_DP);
      }
    }
  }
  cliHideReturn = false;
}

void cliRunCommandRun() {
  cliSerial->print("RUN ");
  cliSerial->println(cliValues[1]);
  for (int i = 0; i < CLI_MENU_COMMANDS_RUN; i++) {
    if (strcmp(cliValues[1], cliCommandsRun[i].commandName) == 0) {
      cliCommandsRun[i].func(atof(cliValues[2]));
    }
  }
  cliHideReturn = false;
}

void cliDisplayHelp(cliCommand *_commands, int num)
{
  cliHideReturn = true;
  for (int i = 1; i < num; i++) { // first command is `help`, we don't need to print it
    cliSerial->print(i);
    cliSerial->print(CLI_DELIMITER);
    cliSerial->println(_commands[i].commandName);
  }
  
}

double cliGetHelp(double id) {
  int num = CLI_MENU_COMMANDS_GET;
  cliDisplayHelp(cliCommandsGet, num);
  return num;
}

double cliSetHelp(double id) {
  int num = CLI_MENU_COMMANDS_SET;
  cliDisplayHelp(cliCommandsSet, num);
  return num;
}

double cliRunHelp(double id) {
  int num = CLI_MENU_COMMANDS_RUN;
  cliDisplayHelp(cliCommandsRun, num);
  return num;
}

void cliFailCommand() {
  cliError("Command too long");
}

void cliError(String msg) {
  cliClearCommand();
  cliWasError();
  cliSerial->print("ERROR: ");
  cliSerial->println(msg);
}

void cliWasError()
{
  cliErrorState = true;
}

void cliClearError()
{
  cliErrorState = false;
}

void cliClearCommand() {
  for (cliPosCommand = 0; cliPosCommand < CLI_VALUES; cliPosCommand++) {
    for (cliPosState = 0; cliPosState < CLI_VALUE_LEN; cliPosState++) {
      cliValues[cliPosCommand][cliPosState] = 0;
    }
  }
  
  cliPosCommand = 0;
  cliPosState = 0;
}
