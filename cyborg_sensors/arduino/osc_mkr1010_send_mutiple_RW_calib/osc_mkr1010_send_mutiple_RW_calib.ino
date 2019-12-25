/*
  WiFi Access Point for OSC Communication
  A simple web server that lets you send OSC messages over a closed network.
  This sketch will create a new access point with password.
  It will then launch a new server, print out the IP address
  to the Serial monitor, and then send an OSC message over UDP to a specific IP address.
  created 24 Nov 2018
  by Federico Peliti
  based on
  WiFi Simple Web Server by Tom Igoe
  WiFi UDP Send and Receive String by dlf
  UDP Send Message by Adrian Freed
  Tested with
  Arduino MKR1010
  NINA Firmware 1.2.1
  Requires the following libraries:
  WiFi by Arduino
  WiFiNINA by Arduino
  OSC by Adrian Freed, Yotam Mann
*/
/*
    Make an OSC message and send it over UDP

    Adrian Freed
*/

/*

  This example connects to an unencrypted Wifi network.
  Then it prints the  MAC address of the Wifi module,
  the IP address obtained, and other network details.

  created 13 July 2010
  by dlf (Metodo2 srl)
  modified 31 May 2012
  by Tom Igoe
*/


//#include <Ethernet.h>
//#include <EthernetUdp.h>
#include <SPI.h>
//#include <OSCMessage.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <CircularBuffer.h>

#include <OSCBundle.h>

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

unsigned int localPort = 8888;      // local port to listen on

// setup circular buffer (rolling window)
CircularBuffer<int, 10> buffer; //la capacidad del buffer es numbersamples, cuyo valor previamente hemos fijado
CircularBuffer<int, 10> buffer1;
CircularBuffer<int, 10> buffer2;
CircularBuffer<int, 10> buffer3;
CircularBuffer<int, 10> buffer4;
CircularBuffer<int, 10> buffer5;
CircularBuffer<int, 10> buffer6;
CircularBuffer<int, 10> buffer7;

WiFiUDP Udp;

//EthernetUDP Udp;

//the Arduino's IP
IPAddress ip(192, 168, 43, 250);
//destination IP
IPAddress outIp(192, 168, 43, 154);
const unsigned int outPort = 12000;
//const unsigned int outPort2 = 13000;

//byte mac[] = {
//  80,7D,3A,86,F8,84
//  }; // you can find this written on the board of some Arduino Ethernets or shields


/////////////*CALIBRATION ROUTINE VARIABLES*/////////////////

// variables:

//const int sensorPin = A0;    // pin that the sensor is attached to

int senVal[] = {0, 0, 0, 0, 0, 0, 0, 0};
int senMin[] = {1023, 1023, 1023, 1023, 1023, 1023, 1023};
int senMax[] = {0, 0, 0, 0, 0, 0, 0};

const int senPin[] = {A0, A1, A2, A3, A4, A5, A6};


void setup() {

  Serial.begin(9600);

  // !!!!!!!!!!!!! estos comemntarios son para que se conecte automanticamente y envie
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for native USB port only
  //  }

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

  if (status == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
  //    Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  // Udp2.begin(localPort);

  //calibRoutine();
}


void loop() {

  for (int j = 0; j < 6; j++) {
    senVal[j] = analogRead(senPin[j]);
    senVal[j] = map(senVal[j], senMin[j], senMax[j], 0, 1023);
  }

  int sensorValue = analogRead(A0);
  int sensorValue1 = analogRead(A1);
  int sensorValue2 = analogRead(A2);
  int sensorValue3 = analogRead(A3);
  int sensorValue4 = analogRead(A4);
  int sensorValue5 = analogRead(A5);
  int sensorValue6 = analogRead(A6);

  ///// building a bundle
  buffer.unshift(sensorValue);
  buffer1.unshift(sensorValue1);
  buffer2.unshift(sensorValue2);
  buffer3.unshift(sensorValue3);
  buffer4.unshift(sensorValue4);
  buffer5.unshift(sensorValue5);
  buffer6.unshift(sensorValue6);

  int s = 0;
  int s1 = 0;
  int s2 = 0;
  int s3 = 0;
  int s4 = 0;
  int s5 = 0;
  int s6 = 0;

  for (int i = 0; i <= buffer.size(); i++) {

    s += buffer[i];
    s1 += buffer1[i];
    s2 += buffer2[i];
    s3 += buffer3[i];
    s4 += buffer4[i];
    s5 += buffer5[i];
    s6 += buffer6[i];
  }

  s = s / buffer.size();
  s1 = s1 / buffer1.size();
  s2 = s2 / buffer2.size();
  s3 = s3 / buffer3.size();
  s4 = s4 / buffer4.size();
  s5 = s5 / buffer5.size();
  s6 = s6 / buffer6.size();

  OSCBundle bndl;
  bndl.add("/analog/0").add((int32_t)s);
  bndl.add("/analog/1").add((int32_t)s1);
  bndl.add("/analog/2").add((int32_t)s2);
  bndl.add("/analog/3").add((int32_t)s3);
  bndl.add("/analog/4").add((int32_t)s4);
  bndl.add("/analog/5").add((int32_t)s5);
  bndl.add("/analog/6").add((int32_t)s6);

  ///////////////////////////// Need to put the IP directly VERY IMPORTANT  ////////////////////////////////////////
  Udp.beginPacket("192.168.43.154", outPort);
  bndl.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  bndl.empty(); // free space occupied by message

  //  OSCMessage msg("/analog/0");
  //  msg.add(String((int32_t)analogRead(A0), DEC));
  //the message wants an OSC address as first argument
  //msg.add((int32_t)analogRead(A0));
  //Serial.println((int32_t)analogRead(0));

  //  Udp.beginPacket("192.168.3.69", outPort2);
  //  msg.send(Udp); // send the bytes to the SLIP stream
  //  Udp.endPacket(); // mark the end of the OSC Packet
  //  msg.empty(); // free space occupied by message

  //* For the arduino to initiate the calibration process*//
  /* aquí estoy tratabdo de meter la calibracion via processing
    OSCBundle bundleIN;
    int size;

    if ( (size = Udp.parsePacket()) > 0)
    {
      Serial.println("llego un mesaje de processing");
      while (size--)
        bundleIN.fill(Udp.read());

      if (!bundleIN.hasError())
        bundleIN.route("/calib", 0);


    }
  */
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

void calibRoutine() {

  digitalWrite(LED_BUILTIN, LOW);
  // calibrate during the first five seconds
  while (millis() < 5000) {

    for (int i = 0; i < 6; i++) {
      senVal[i] = analogRead(senPin[i]);
      if (senVal[i] > senMax[i]) {
        senMax[i] = senVal[i]
      }
      if (senVal[i] > senMin[i]) {
        senMin[i] = senVal[i]
      }
    }
  }
  // signal the end of the calibration period
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println("Calibration Donde, lets make some noise");
}
