#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <CircularBuffer.hpp>
#include <Adafruit_NeoPixel.h>

// Configuración de red y OSC
char ssid[] = "Invisible";           // tu SSID
char pass[] = "Invisible";           // tu contraseña
IPAddress ip(192, 168, 1, 11);       // IP del Arduino
IPAddress outIp(192, 168, 1, 100);   // IP del destino
const unsigned int outPort = 12000;  // Puerto del destino
WiFiUDP Udp;

// setup circular buffer (rolling window)
CircularBuffer<int, 10> buffer;   // la capacidad del buffer es 10
CircularBuffer<int, 10> buffer2;  // la capacidad del buffer es 10

// Variables para los sensores del EMG MYO
#define emgPin A1

// Variables para los potenciómetros y botón
#define potPin1 A4
#define potPin2 A2
#define fsrPin A0
#define buttonPin 7

// Variables para las luces
// Configuración de los Neopixels
#define PIN 6        // Pin donde están conectados los Neopixels
#define NUMPIXELS 1  // Número de Neopixels

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const int neoP1 = 0;

// el pin para el led de conectado
const int wifiLed = 10;
const int pwmFreq = 5000;
const int pwmResolution = 8;

void setup() {
  Serial.begin(115200);

  Serial.println("Joakinator Mini: EMG + Potentiometers + FSR + Button + Beetle ESP32 C3");
  ledcAttach(wifiLed, pwmFreq, pwmResolution);

  // Conexión WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected to LA INVISIBLE");

  // si se ha conectado el wifi se prende el led 10
  ledcWrite(wifiLed, 500);
  Udp.begin(outPort);

  // Inicialización de los Neopixels
  pixels.begin();
  pixels.show();  // Inicializar todos los píxeles a 'apagado'

  // Configuración de los pines
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Obtener datos del sensor EMG
  int emgValue = analogRead(emgPin);
  buffer.unshift(emgValue);

  int emgAvg = 0;
  for (int i = 0; i < buffer.size(); i++) {
    emgAvg += buffer[i];
  }
  emgAvg = emgAvg / buffer.size();

  sendEMGData(emgAvg);

  // Obtener datos de los potenciómetros y el botón
  int potValue1 = analogRead(potPin1);
  int potValue2 = analogRead(potPin2);
  int fsrValue = analogRead(fsrPin);
  bool buttonState = digitalRead(buttonPin) == LOW;

  sendSensorData(potValue1, potValue2, fsrValue, buttonState);

  // Controlar el brillo del color rojo de los Neopixels según el valor del EMG
  int redBrightness = map(emgAvg, 0, 4095, 0, 255);  // Ajustar el valor EMG al rango de brillo
  setNeopixelColor(neoP1, redBrightness, 0, 0);      // Ajustar el color rojo

  delay(1);  // Esperar un tiempo antes de la siguiente lectura
}

void sendEMGData(int emg) {
  // OSC Messages
  OSCMessage msg("/emg");
  msg.add((int32_t)emg);

  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);    // send the bytes to the SLIP stream
  Udp.endPacket();  // mark the end of the OSC Packet
  msg.empty();      // free space occupied by message
}

void sendSensorData(int pot1, int pot2, int fsr, bool button) {
  // OSC Messages
  OSCMessage msg("/sensors");
  msg.add((int32_t)pot1).add((int32_t)pot2).add((int32_t)fsr).add(button);

  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);    // send the bytes to the SLIP stream
  Udp.endPacket();  // mark the end of the OSC Packet
  msg.empty();      // free space occupied by message
}

// Función para configurar el color de los Neopixels
void setNeopixelColor(int neoPixel, int red, int green, int blue) {
  pixels.setPixelColor(neoPixel, pixels.Color(red, green, blue));
  pixels.show();  // Actualizar los Neopixels con el nuevo color
}
