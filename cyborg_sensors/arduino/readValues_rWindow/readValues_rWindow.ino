/*
  In this skectch im mixing the circular buffer librari with the wekinator arduino program

  two task are in que:

  make the mean of the sensors and make the median in order to brake the noise

*/

/*
   Arduino2Max
   Send pin values from Arduino to MAX/MSP

   Arduino2Max.pde
   ------------
   This version: .5, November 29, 2010
   ------------
   Copyleft: use as you like
   by Daniel Jolliffe
   Based on a sketch and patch by Thomas Ouellet Fredericks  tof.danslchamp.org

*/

#include <CircularBuffer.h>
#define numSen 4
#define numBuffers 4
#define ledpin 13

CircularBuffer<byte, 255> buffer[numBuffers];
//unsigned long time = 0;
int x = 0;                              // a place to hold pin values
float mean = 0.0;

void setup()
{
  Serial.begin(115200);               // 115200 is the default Arduino Bluetooth speed
  digitalWrite(13, HIGH);             ///startup blink
  delay(600);
  digitalWrite(13, LOW);
  pinMode(13, INPUT);
  //time = millis();
}
void loop()
{
  //if (Serial.available() > 0){         // Check serial buffer for characters
  // if (Serial.read() == 'r') {       // If an 'r' is received then read the pins
  for (byte pin = 0; pin <= numSen; pin++) {  // Read and send analog pins 0-5
    x = analogRead(pin);
    //sendValue (x);
    if (pin <= numBuffers - 1) buffer[pin].push(x);
  }
  if (buffer[numBuffers].isFull()) {
    //time = millis();
    for (byte actualBu = 0; actualBu <= numBuffers; actualBu++) {
      // the following ensures using the right type for the index variable
      mean = 0.0;
      //    using index_t = decltype(buffer[actualBu])::index_t;
      for (byte i = 0; i < buffer[actualBu].size(); i++) {
        mean += buffer[actualBu][i] / buffer[actualBu].size();
      }
      //Serial.println(mean);
      sendValue (mean);
      //Serial.print("Average is ");
      //Serial.println(avg);
    }
  }
  Serial.println();                 // Send a carriage returnt to mark end of pin data.
  delay (5);                        // add a delay to prevent crashing/overloading of the serial port
  // }
  // }
}

void sendValue (int x) {             // function to send the pin value followed by a "space".
  Serial.print(x);
  Serial.write(32);
}
