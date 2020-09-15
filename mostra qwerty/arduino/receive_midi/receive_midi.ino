/*Receive MIDI and check if note = 60
By Amanda Ghassaei
July 2012
<a href="https://www.instructables.com/id/Send-and-Receive-MIDI-with-Arduino/">

https://www.instructables.com/id/Send-and-Receive-...>

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

*/

/*servo part*/

#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int val = 180;    // variable to read the value from the analog pin



byte commandByte;
byte noteByte;
byte velocityByte;

//

byte noteOn = 153;

//light up led at pin 13 when receiving noteON message with note = 60

void setup(){
  Serial1.begin(31250);
  pinMode(13,OUTPUT);

  //// servo part
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
   myservo.write(val); 
  digitalWrite(13,LOW);
}

void checkMIDI(){
  do{
    if (Serial1.available()){
      commandByte = Serial1.read();//read first byte
      noteByte = Serial1.read();//read next byte
      velocityByte = Serial1.read();//read final byte
      if (commandByte == noteOn){//if note on message
        //check if note == 60 and velocity > 0
        if (noteByte == 60 && velocityByte > 0){
          digitalWrite(13,HIGH);//turn on led
        }
      }
    }
  }
  while (Serial1.available() > 2);//when at least three bytes available
}
    

void loop(){
  checkMIDI();
  delay(100);
  digitalWrite(13,LOW);//turn led off
}
