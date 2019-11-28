/*
    Make an OSC message and send it over UDP

    Adrian Freed
*/



//#include <Ethernet.h>
//#include <EthernetUdp.h>
#include <SPI.h>
#include <OSCMessage.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

unsigned int localPort = 8888;      // local port to listen on

WiFiUDP Udp;

//EthernetUDP Udp;

//the Arduino's IP
IPAddress ip(128, 32, 122, 252);
//destination IP
IPAddress outIp(192, 68, 3, 69);
const unsigned int outPort = 12000;
const unsigned int outPort2 = 13000;

//byte mac[] = {
//  80,7D,3A,86,F8,84
//  }; // you can find this written on the board of some Arduino Ethernets or shields


void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // you can override it with the following:
  WiFi.config(ip);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);

  }
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
  //    Ethernet.begin(mac, ip);
  Udp.begin(localPort);
 // Udp2.begin(localPort);
}


void loop() {


  //the message wants an OSC address as first argument
  OSCMessage msg("/analog/0");
  OSCMessage msg2("/analog/0");

  msg.add((int32_t)analogRead(0));
  msg2.add((int32_t)analogRead(0));
  //Serial.println((int32_t)analogRead(0));

///////////////////////////// Need to put the IP directly VERY IMPORTANT  ////////////////////////////////////////
  Udp.beginPacket("192.168.3.69", outPort);
  msg.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message

  Udp.beginPacket("192.168.3.69", outPort2);
  msg2.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg2.empty(); // free space occupied by message

  delay(20);
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
