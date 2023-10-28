/*

   i_mBODY LAB
   Vibran v: 1.0
   by: Joaku De Sotavento

*/

#include "WiFi.h"
#include <SPI.h>
#include <OSCMessage.h>

WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
// Options
int update_rate = 16;

#include "vibrant_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h and don't publish it in any place
////char ssid[] = SECRET_SSID;        // your network SSID (name)
//char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
const char* ssid     = "joakinator1";
const char* pass = "joakinator1";

//IPAddress local_IP(192, 168, 1, 120);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 255, 0);
//IPAddress primaryDNS(0, 0, 0, 0); //optional
//IPAddress secondaryDNS(0, 0, 0, 0); //optional

unsigned int localPort = 8888; // local port to listen for OSC packets

///// send OSC to MAX
//destination IP
IPAddress outIp(192, 168, 1, 100);
const unsigned int outPort = 8000;

const int pot = A2;

const int drive1 = 13;
const int drive2 = 12;
const int drive3 = 27;
const int drive4 = 33;
const int drive5 = 32;

const int stopVib = 0;

const int periodOn = 130;
const int periodOff = 50;

const int periodOn2 = 60;
const int periodOff2 = 49;

const int longDelay = 10000;

// FOR THE INTERFACE
const int ledWIFI = 14;

// intensities

int intensity = 0;
int intensity1 = 0;
int intensity2 = 0;
int intensity3 = 0;


// use first channel of 16 channels (started from zero)
// tjose channels are for the PWM on each motor
#define LEDC_CHANNEL_0     0
#define LEDC_CHANNEL_1     1
#define LEDC_CHANNEL_2     2
#define LEDC_CHANNEL_3     3
#define LEDC_CHANNEL_4     4


// use 13 bit precission for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     8000

int brightness = 0;    // how bright the LED is


// Arduino like analogWrite
// value has to be between 0 and valueMax
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}


///////////////////// delay sin delay

int period = 60;
unsigned long time_now = 0;


//////// cosas de los potenciometros

//int intensity = 13;

// the setup function runs once when you press reset or power the board
void setup()
{
  // FOR THE INTERFACE
  pinMode(ledWIFI, OUTPUT);

  // SERIAL COMMUNICATION
  Serial.begin(115200);
  delay(10);
  //  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //    Serial.println("STA Failed to configure");
  //  }

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP Mac Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());

  digitalWrite(ledWIFI, HIGH);

  Udp.begin(localPort);

  /////////////////////////// Yhis part is also for the PWM
  // Setup timer and attach timer to a led pin
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LEDC_CHANNEL_3, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcSetup(LEDC_CHANNEL_4, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);

  ledcAttachPin(drive1, LEDC_CHANNEL_0);
  ledcAttachPin(drive2, LEDC_CHANNEL_1);
  ledcAttachPin(drive3, LEDC_CHANNEL_2);
  ledcAttachPin(drive4, LEDC_CHANNEL_3);
  ledcAttachPin(drive5, LEDC_CHANNEL_4);
}

void ledtoggle(OSCMessage &msg) {

  //// In the MaxPatch we just use state 1 and 2
  switch (msg.getInt(0)) {
    case 0:
      // Serial.println("Apagado");
      break;
    case 1:
      // Serial.println("Up Overlaping");
      vibrantUpOl();
      break;
    case 2:
      // Serial.println("Down Overlaping");
      vibrantDownOl();
      break;
    case 3:
      // Serial.println("Up Hue");
      vibrantUpHue();
      break;
    case 4:
      // Serial.println("Down Hue");
      vibrantDownHue();
      break;
    case 5:
      // Serial.println("Up Paused");
      vibrantPausedUp();
      break;
    case 6:
      // Serial.println("Down Paused");
      vibrantPausedDown();
      break;
  }
}

void receiveMessage() {
  OSCMessage inmsg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      inmsg.fill(Udp.read());
    }
    if (!inmsg.hasError()) {
      inmsg.dispatch("/led", ledtoggle);
    }
    //else { auto error = inmsg.getError(); }
  }
}

void loop() {
  receiveMessage();
  delay(update_rate);
}


void vibrantUpOl() {
  intensity = map(analogRead(pot), 0, 4095, 0, 255);
  intensity3 = intensity;
  intensity2 = intensity-20;
  intensity1 = intensity-40;
  //Serial.print("El potenciometro da: ");
  //Serial.println(intensity);
  sendOsc2Max(intensity);

  ledcAnalogWrite(LEDC_CHANNEL_0, intensity1);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_0, intensity1);
  ledcAnalogWrite(LEDC_CHANNEL_1, intensity1);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_0, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_1, intensity1);
  ledcAnalogWrite(LEDC_CHANNEL_2, intensity2);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_1, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_2, intensity2);
  ledcAnalogWrite(LEDC_CHANNEL_3, intensity2);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_2, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_3, intensity2);
  ledcAnalogWrite(LEDC_CHANNEL_4, intensity3);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_3, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_4, intensity3);
  delay(periodOn * 4);

  ledcAnalogWrite(LEDC_CHANNEL_4, stopVib);
  delay(periodOn);
}

void vibrantDownOl() {
  intensity = map(analogRead(pot), 0, 4095, 0, 255);
  intensity3 = intensity;
  intensity2 = intensity-20;
  intensity1 = intensity-40;
  //Serial.print("El potenciometro da: ");
  //Serial.println(intensity);
  sendOsc2Max(intensity);

  ledcAnalogWrite(LEDC_CHANNEL_4, intensity1);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_4, intensity1);
  ledcAnalogWrite(LEDC_CHANNEL_3, intensity1);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_4, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_3, intensity1);
  ledcAnalogWrite(LEDC_CHANNEL_2, intensity2);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_3, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_2, intensity2);
  ledcAnalogWrite(LEDC_CHANNEL_1, intensity2);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_2, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_1, intensity2);
  ledcAnalogWrite(LEDC_CHANNEL_0, intensity3);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_1, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_0, intensity3);
  delay(periodOn * 4);

  ledcAnalogWrite(LEDC_CHANNEL_0, stopVib);
  delay(periodOn);
}

void vibrantUpHue() {
  intensity = map(analogRead(pot), 0, 4095, 0, 255);
  // Serial.print("El potenciometro da: ");
  // Serial.println(intensity);
  sendOsc2Max(intensity);

  ledcAnalogWrite(LEDC_CHANNEL_0, intensity - 120);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_0, intensity - 120);
  ledcAnalogWrite(LEDC_CHANNEL_1, intensity - 90);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_0, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_1, intensity - 90);
  ledcAnalogWrite(LEDC_CHANNEL_2, intensity - 60);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_1, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_2, intensity - 60);
  ledcAnalogWrite(LEDC_CHANNEL_3, intensity -30);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_2, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_3, intensity - 30);
  ledcAnalogWrite(LEDC_CHANNEL_4, intensity);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_3, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_4, intensity);
  delay(periodOn * 4);

  ledcAnalogWrite(LEDC_CHANNEL_4, stopVib);
  delay(periodOn);
}

void vibrantDownHue() {
  intensity = map(analogRead(pot), 0, 4095, 0, 255);
  // Serial.print("El potenciometro da: ");
  // Serial.println(intensity);
  sendOsc2Max(intensity);

  ledcAnalogWrite(LEDC_CHANNEL_4, intensity - 120);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_4, intensity - 120);
  ledcAnalogWrite(LEDC_CHANNEL_3, intensity - 90);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_4, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_3, intensity - 90);
  ledcAnalogWrite(LEDC_CHANNEL_2, intensity - 60);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_3, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_2, intensity - 60);
  ledcAnalogWrite(LEDC_CHANNEL_1, intensity - 30);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_2, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_1, intensity - 30);
  ledcAnalogWrite(LEDC_CHANNEL_0, intensity);
  delay(periodOn);

  ledcAnalogWrite(LEDC_CHANNEL_1, stopVib);
  ledcAnalogWrite(LEDC_CHANNEL_0, intensity);
  delay(periodOn * 4);

  ledcAnalogWrite(LEDC_CHANNEL_0, stopVib);
  delay(periodOn);
}

void vibrantPausedUp() {
  intensity = map(analogRead(pot), 0, 4095, 0, 255);
  //  Serial.print("El potenciometro da: ");
  //  Serial.println(intensity);
  sendOsc2Max(intensity);

  ledcAnalogWrite(LEDC_CHANNEL_0, intensity);
  delay(periodOn2);
  ledcAnalogWrite(LEDC_CHANNEL_0, stopVib);
  delay(periodOff2);

  ledcAnalogWrite(LEDC_CHANNEL_1, intensity);
  delay(periodOn2);
  ledcAnalogWrite(LEDC_CHANNEL_1, stopVib);
  delay(periodOff2);

  ledcAnalogWrite(LEDC_CHANNEL_2, intensity);
  delay(periodOn2);
  ledcAnalogWrite(LEDC_CHANNEL_2, stopVib);
  delay(periodOff2);

  ledcAnalogWrite(LEDC_CHANNEL_3, intensity);
  delay(periodOn2);
  ledcAnalogWrite(LEDC_CHANNEL_3, stopVib);
  delay(periodOff2);

  ledcAnalogWrite(LEDC_CHANNEL_4, intensity);
  delay(periodOn2);
  ledcAnalogWrite(LEDC_CHANNEL_4, stopVib);
  delay(periodOff2);

}

void vibrantPausedDown() {
  intensity = map(analogRead(pot), 0, 4095, 0, 255);
  //  Serial.print("El potenciometro da: ");
  //  Serial.println(intensity);
  sendOsc2Max(intensity);

  ledcAnalogWrite(LEDC_CHANNEL_4, intensity);
  delay(periodOn2);
  ledcAnalogWrite(LEDC_CHANNEL_4, stopVib);
  delay(periodOff2);

  ledcAnalogWrite(LEDC_CHANNEL_3, intensity);
  delay(periodOn2);
  ledcAnalogWrite(LEDC_CHANNEL_3, stopVib);
  delay(periodOff2);

  ledcAnalogWrite(LEDC_CHANNEL_2, intensity);
  delay(periodOn2);
  ledcAnalogWrite(LEDC_CHANNEL_2, stopVib);
  delay(periodOff2);

  ledcAnalogWrite(LEDC_CHANNEL_1, intensity);
  delay(periodOn2);
  ledcAnalogWrite(LEDC_CHANNEL_1, stopVib);
  delay(periodOff2);

  ledcAnalogWrite(LEDC_CHANNEL_0, intensity);
  delay(periodOn2);
  ledcAnalogWrite(LEDC_CHANNEL_0, stopVib);
  delay(periodOff2);

}

void sendOsc2Max(int _pot) {
  //the message wants an OSC address as first argument
  OSCMessage msg("/max");
  msg.add((int32_t)_pot);

  Udp.beginPacket(outIp, outPort);
  msg.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message

}
