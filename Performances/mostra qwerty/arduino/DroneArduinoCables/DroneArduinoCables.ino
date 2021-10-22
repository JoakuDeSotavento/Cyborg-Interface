/*  Example playing a sinewave at a set frequency,
    using Mozzi sonification library.
  
    Demonstrates the use of Oscil to play a wavetable.
  
    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or 
    check the README or http://sensorium.github.com/Mozzi/
  
    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users
  
    Tim Barrass 2012, CC by-nc-sa.
*/

//#include <ADC.h>  // Teensy 3.1 uncomment this line and install http://github.com/pedvide/ADC
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // powers of 2 please

 unsigned long timeNow = 0;
  unsigned long timeMark = 0;
  unsigned long timeBase = 4000;
  int durationSelect = 2;
  int durationFactor = 4;
  int baseFreq = 440;
  
void setup(){
  startMozzi(CONTROL_RATE); // set a control rate of 64 (powers of 2 please)
  
  aSin.setFreq(baseFreq); // set the frequency
 
}


void updateControl(){
  // put changing controls in here
  timeNow = audioTicks();
  if ((timeNow - timeMark) > timeBase * durationFactor){     
    durationSelect = random(4);
    durationFactor = pow (2, durationSelect);
    float toneFreq = baseFreq;
    switch (durationSelect){
      case 0:
         toneFreq = baseFreq * 0.76;
      break;
      case 1:
         toneFreq = baseFreq * 0.375;
      break;
      case 2:
         toneFreq = baseFreq * 0.5;
      break;
      case 3:
         toneFreq = baseFreq * 0.25;
      break;
      default:
        toneFreq = baseFreq * 1;
      break;
      }
      
    aSin.setFreq(toneFreq);
    timeMark = timeNow;
    }
}


int updateAudio(){
  return aSin.next(); // return an int signal centred around 0
}


void loop(){
  audioHook(); // required here
}



