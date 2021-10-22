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
#include <CapacitiveSensor.h>


CapacitiveSensor   cs_11_10 = CapacitiveSensor(11, 10);
CapacitiveSensor   cs_8_7 = CapacitiveSensor(8, 7);

const int ON_11_10 = 1000;
const int ON_8_7 = 1000;

int retard = 200;

int bot_11_10 = 0;
int bot_8_7 = 0;


Tone tone1;

int notes[] = { 0,
                NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
                NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
                NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
                NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
              };

int kumoy[] = {
  NOTE_F4, NOTE_G4, NOTE_GS4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5
};




// constants won't change. Used here to set a pin number:
const int ledPin =  LED_BUILTIN;// the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 500;           // interval at which to blink (milliseconds)

const unsigned int fraqTemp[] = { 1, 2, 4, 8, 16, 32, 64 };

#define sound 9

#define muscle A0

unsigned int tempo = 16;

unsigned int ritmo[] = {1, 1, 1, 1,
                        1, 1, 1, 1,
                        1, 1, 1, 1,
                        1, 1, 1, 1
                       };

//int ritmo2[16][3] = {
//  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
//};


unsigned int MIDINotes[] = {33, 34, 43, 35, 36, 66, 84, 86, 87, 88, 90, 91,
                            25, 26, 27, 28, 29, 30, 39, 41, 43, 45, 47, 48, 50, 60, 61, 63, 62, 76, 77, 79, 89, 95, 96,
                            37, 42, 44, 46, 49, 51, 52, 53, 54, 55, 56, 57, 58, 59, 68, 67, 69, 70, 71, 72, 73, 74, 76, 75, 80, 81, 82, 83, 94
                           };


unsigned int contRitmo = 0;

void setup() {

  cs_11_10.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_8_7.set_CS_AutocaL_Millis(0xFFFFFFFF);

  // Set MIDI baud rate:
  Serial.begin(31250);
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);

  tone1.begin(sound);

  //pinMode(sound, OUTPUT);
}

void loop() {
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  //tone1.play();
  unsigned long currentMillis = millis();

  long total_11_10  =  cs_11_10.capacitiveSensor(12);
  long total_8_7  =  cs_8_7.capacitiveSensor(12);

  int note = 0x5F;

  int soundSin = random(0, 8);

  //int noteRan = random(0x1E, 0x5A);

  int noteRan = 0x3E;

  int muscle = analogRead(muscle);
  int muscleSen = map (muscle, 30, 500, 0, 6);
  int muscleNote = map (muscle, 30, 500, 5, 58);

  MIDINotes[muscleNote + random(-5, 5)];


  clockMIDI(currentMillis, MIDINotes[muscleNote + random(-5, 5)], soundSin, muscleSen);


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


        //  tone1.play(muscle);
        tone1.play(kumoy[_soundSin]);
        //tone(sound, _soundSin);
        noteOn(0x99, 60, 0x44);
      }

      contRitmo++;
      ledState = HIGH;

    } else {

      noteOn(0x99, 60, 0x00);
      //noTone(sound);
      tone1.stop();
      ledState = LOW;

    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);

  }


}

// plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that
// data values are less than 127:
void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}
