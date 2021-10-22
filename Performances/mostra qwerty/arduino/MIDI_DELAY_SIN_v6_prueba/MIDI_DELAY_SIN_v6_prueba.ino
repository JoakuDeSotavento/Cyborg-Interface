/*
  Blink without Delay

  Turns on and off a light emitting diode (LED) connected to a digital pin,
  without using the delay() function. This means that other code can run at the
  same time without being interrupted by the LED code.

  The circuit:
  - Use the onboard LED.
  - Note: Most Arduinos have an on-board LED you can control. On the UNO, MEGA
    and ZERO it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN
    is set to the correct LED pin independent of which board is used.
    If you want to know what pin the on-board LED is connected to on your
    Arduino model, check the Technical Specs of your board at:
    https://www.arduino.cc/en/Main/Products

  created 2005
  by David A. Mellis
  modified 8 Feb 2010
  by Paul Stoffregen
  modified 11 Nov 2013
  by Scott Fitzgerald
  modified 9 Jan 2017
  by Arturo Guadalupi

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
*/

/*
  MIDI note player

  This sketch shows how to use the serial transmit pin (pin 1) to send MIDI note data.
  If this circuit is connected to a MIDI synth, it will play the notes
  F#-0 (0x1E) to F#-5 (0x5A) in sequence.

  The circuit:
  - digital in 1 connected to MIDI jack pin 5
  - MIDI jack pin 2 connected to ground
  - MIDI jack pin 4 connected to +5V through 220 ohm resistor
  - Attach a MIDI cable to the jack, then to a MIDI synth, and play music.

  created 13 Jun 2006
  modified 13 Aug 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Midi
*/


#include <Tone.h>

// These constants won't change:
// const int sensorPin = A0;    // pin that the sensor is attached to
// const int ledPin = 9;        // pin that the LED is attached to

////// capacitive sensor/////
//CapacitiveSensor   cs_11_10 = CapacitiveSensor(11, 10);
//CapacitiveSensor   cs_8_7 = CapacitiveSensor(8, 7);

//const int ON_11_10 = 1000;
//const int ON_8_7 = 1000;

////// Sound  part ///////////////////

Tone tone1, tone2;

int kumoy[] = {
  NOTE_F4, NOTE_G4, NOTE_GS4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5
};


const unsigned int fraqTemp[] = { 1, 2, 4, 8, 16, 32, 64 };

////// calibraciion del sensor
#define muscle A0

// variables:
int sensorValue = 0;         // the sensor value
int sensorMin = 1023;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value


unsigned int tempo = 16;

unsigned int ritmo[] = {1, 1, 1, 1,
                        1, 1, 1, 1,
                        1, 1, 1, 1,
                        1, 1, 1, 1
                       };

//int ritmo2[16][3] = {
//  {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
//  {1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
//};

////////

////// MIDI part //////////////////////

unsigned int MIDINotes[] = {21, 28, 30, 31, 35, 36, 40, 42, 43, 47, 48, 52, 54, 55, 59, 60, 64, 66, 67, 71, 72

                           };


unsigned int contRitmo = 0;

int cmd = 0x99;

//int cmd = 10;

int pitch = 0x3c;

int velocity = 0x64;

/////////// no delay clock

// constants won't change. Used here to set a pin number:
const int ledPin =  LED_BUILTIN;// the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 500;           // interval at which to blink (milliseconds)


void setup() {


  // calibrate during the first five seconds
  while (millis() < 5000) {
    sensorValue = analogRead(muscle);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }
  /*/////////////////////////////////////////////////////////////////////////////////////*/

  // Set MIDI baud rate:
  Serial.begin(31250);
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);

}

void loop() {
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  //tone1.play();
  unsigned long currentMillis = millis();
  //int note = 0x5F;

  int soundSin = random(0, 8);

  //int noteRan = random(0x1E, 0x5A);

  //int noteRan = 0x3E;

  sensorValue = analogRead(muscle);
  // this define the index for the array with the division of time
  int muscleSen = map (sensorValue, sensorMin, sensorMax, 0, 5);

  // this define the index for the array of notes
  int muscleNote = map (sensorValue, sensorMin, sensorMax, 0, 20);

  // filter that just makes the sensor produce sound when it is bigger in numbers

  if (muscleSen > 1) {
    clockMIDI(currentMillis, MIDINotes[muscleNote + random(-5, 5)], soundSin, muscleSen);
  }
 
}

void clockMIDI(unsigned long _currentMillis, int _noteRan, int _soundSin, int _muscleSen) {

  if (_currentMillis - previousMillis >= interval / fraqTemp[_muscleSen]) {

    if (contRitmo >= tempo - 1) {
      contRitmo = 0;
    }
    // save the last time you blinked the LED
    previousMillis = _currentMillis;
    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      if (ritmo[contRitmo] == 1) {

        noteOn(cmd, _noteRan, velocity);
      }

      contRitmo++;
      ledState = HIGH;

    } else {

      noteOn(cmd, _noteRan, 0x00);
      ledState = LOW;

    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);

  }


}

// plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that
// data values are less than 127:
void noteOn(int _cmd, int _pitch, int _velocity) {
  Serial.write(_cmd);
  Serial.write(_pitch);
  Serial.write(_velocity);
}
