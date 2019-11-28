#include <ArduinoOSC.h>
OscWiFi osc;

void setup()
{
  WiFi.begin(ssid, pwd);
  WiFi.config(ip, gateway, subnet);
  osc.begin(recv_port);

  // add callbacks...
  osc.subscribe("/lambda", [](OscMessage & m)
  {
    // do something with osc message
    Serial.print(m.arg<int>(0));    Serial.print(" ");
    Serial.print(m.arg<float>(1));  Serial.print(" ");
    Serial.print(m.arg<String>(2)); Serial.println();
  });
}

void loop()
{
  osc.parse(); // should be called
  osc.send(host, send_port, "/send", 1, 2.2F, 3.3, "string"); // send osc packet in one line
}
