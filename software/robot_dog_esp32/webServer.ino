void initWebServer() {
  Serial.print("WebServer ");
  initWebServerRoutes();
  Serial.println();
}

void initWebServerRoutes() {  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html",  index_html_gz, index_html_gz_len);
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);  
  });
    
  // Dinamic config
  server.on("/c.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/x-javascript", "var c={w:'ws://" + WiFiIP.toString() + "/ws'}");
  });

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
 
  server.begin();
}

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
 
  if(type == WS_EVT_CONNECT){ 
    client->text("Ok");
    clientOnline = true;
  } else if (clientOnline && type == WS_EVT_DATA) {
    FS_WS_count = 0;  // zero FS counter

    switch(data[0]) {
      case P_MOVE:
        pMove(data);
        break;
      case P_TELEMETRY:
        pTelemetry();
        client->binary(telemetryPackage, P_TELEMETRY_LEN);
        break;
      default:
        Serial.print("UNKNOWN PACKAGE ID: ");
        Serial.println(data[0], DEC);
    }

  } else if(type == WS_EVT_DISCONNECT){
    clientOnline = false;
  }
}
