// TODO binarySubscription

subscription subscriptions[CLI_MENU_COMMANDS_GET] = {};

void initSubscription()
{
  // Copy all get methods to use is for subscriptions
  for (int i = 0; i < CLI_MENU_COMMANDS_GET; i++) {
    subscriptions[i].active = false;
    subscriptions[i].fast   = false;
  }
}

double subscriptionState(double state)
{
  subscriptionEnabled = state > 0;

  Serial.println(subscriptionEnabled);
  
  return subscriptionEnabled ? 1.0 : 0.0;
}

void subscribe(int commandId, bool subscribe, bool fast)
{
  Serial.println(commandId);
  Serial.println(subscribe);
  Serial.println(fast);
    subscriptions[commandId].active = subscribe;
    subscriptions[commandId].fast   = fast;
}

void doFASTSubscription()
{
  doSubscription(true);
}

void doSLOWSubscription()
{
  doSubscription(false);
}

void doSubscription(bool fast)
{
  if (!subscriptionEnabled) return;
  
  for (int i = 0; i < CLI_MENU_COMMANDS_GET; i++) {
    if (subscriptions[i].active && subscriptions[i].fast) {
      double r = cliCommandsGet[i].func(0.0);
      cliSerial->print(cliCommandsGet[i].commandName);
      cliSerial->print(CLI_DELIMITER);
      cliSerial->println(r, CLI_DP);
    }
  }
}
