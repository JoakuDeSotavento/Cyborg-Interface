/*Receive MIDI and check if note = 60
  By Amanda Ghassaei
  July 2012
  <a href="https://www.instructables.com/id/Send-and-Receive-MIDI-with-Arduino/">

  https://www.instructables.com/id/Send-and-Receive-...>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

*/

/*servo part*/

#include <Servo.h>
Servo servoElbow1, servoElbow2, servoWrist;  // create servo object to control a servo
int val2 = 50;    //initial value for elbow1, 90,  goes from 0 to 90 menores para la izquierda
int val3 = 110;    //initial value for elbow2, 180, goes to 90. Menores va para arriba
int val4 = 70;    //initial value for wrist, goes from 0 to 180. Menores va para abajo


byte commandByte;
byte noteByte;
byte velocityByte;

//

byte noteOn = 153;

//light up led at pin 13 when receiving noteON message with note = 60

void setup() {
  Serial1.begin(31250);
  pinMode(13, OUTPUT);

  servoElbow1.attach(9);  // attaches the servo on pin 9 to the servo object
  servoElbow1.write(val2);
  servoElbow2.attach(10);  // attaches the servo on pin 9 to the servo object
  servoElbow2.write(val3);
  servoWrist.attach(11);  // attaches the servo on pin 9 to the servo object
  servoWrist.write(val4);
//
//  for (val3 = 110; val3 <= 180; val3 += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servoElbow2.write(val3);              // tell servo to go to position in variable 'pos'
//    servoElbow2.write(val3);
//    servoElbow1.write(val3);              // tell servo to go to position in variable 'pos'
//    servoElbow1.write(val3);
//    servoWrist.write(val3);              // tell servo to go to position in variable 'pos'
//    servoWrist.write(val3);
//
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//  for (val3 = 180; val3 >= 110; val3 -= 1) { // goes from 180 degrees to 0 degrees
//    servoElbow2.write(val3);              // tell servo to go to position in variable 'pos'
//    servoElbow2.write(val3);
//    servoElbow1.write(val3);              // tell servo to go to position in variable 'pos'
//    servoElbow1.write(val3);
//    servoWrist.write(val3);              // tell servo to go to position in variable 'pos'
//    servoWrist.write(val3);
//
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
  digitalWrite(13, LOW);
}

void checkMIDI() {
  do {
    if (Serial1.available()) {
      commandByte = Serial1.read();//read first byte
      noteByte = Serial1.read();//read next byte
      velocityByte = Serial1.read();//read final byte
      if (commandByte == noteOn) { //if note on message
        //check if note == 60 and velocity > 0
        if (noteByte == 60 && velocityByte > 0) {
          digitalWrite(13, HIGH); //turn on led
        }
      }
    }
  }
  while (Serial1.available() > 2);//when at least three bytes available
}


void loop() {
  checkMIDI();
  
  delay(100);
  digitalWrite(13, LOW); //turn led off
}
