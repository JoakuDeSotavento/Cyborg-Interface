/*Receive MIDI and check if note = 60
  By Amanda Ghassaei
  July 2012
  <a href="https://www.instructables.com/id/Send-and-Receive-MIDI-with-Arduino/">
*/

/*servo part: Joaquin Díaz, Alonso Torres
  august 2017 
*/

#include <Servo.h>
Servo servoElbow1, servoElbow2, servoWrist;  // create one servo object to control each servo
int elbow1Start = 70;    //initial value for elbow1, goes from 0 to 180 menores para la izquierda
int elbow2Start = 100;    //initial value for elbow2, goes to 90. Menores va para arriba
int wristStart = 120;    //initial value for wrist, goes from 0 to 180. Menores va para abajo

byte commandByte;
byte noteByte;
byte velocityByte;

byte noteOn = 152; // to receive notes only from MIDI channel 9
int frameCount = 0;
boolean playSequence = false;

//States sequence for Robot arm "coreography", displacement in degrees from start point.

int wristIndex = 0;
int wristSequence[8] = {-4, -8, -12, -16, -12, -8, -4, 0};

int elbow2Index = 0;
int elbow2Sequence[14] = {2, 4, 6, 8, 10, 12, 14,
                          12, 10, 8, 6, 4, 2, 0};

int elbow1Index = 0;
int elbow1Sequence[42] = {2, 4, 6, 8, 10, 12, 14, 16, 18, 20,
                          19, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0,
                          -2, -4, -6, -8, -10, -12, -14, -16, -18, -20,
                          -19, -18, -16, -14, -12, -10, -8, -6, -4, -2, 0};

void setup() {
  
  Serial1.begin(31250);
  pinMode(13, OUTPUT); //light up led at pin 13 when receiving noteON message with note = 60

  //Put robot arm in start position
  servoElbow1.attach(9);  // attaches the servo on pin 9 to the servo object
  servoElbow1.write(elbow1Start);
  servoElbow2.attach(10);  // attaches the servo on pin 9 to the servo object
  servoElbow2.write(elbow2Start);
  servoWrist.attach(11);  // attaches the servo on pin 9 to the servo object
  servoWrist.write(wristStart);

  digitalWrite(13, LOW);
}


void checkMIDI() {
  do {
    if (Serial1.available()) {
      commandByte = Serial1.read();//read first byte
      noteByte = Serial1.read();//read next byte
      velocityByte = Serial1.read();//read final byte
      
      if (commandByte == noteOn) { //if note on message on channel 9
        if (noteByte == 60 && velocityByte > 0) {   //check if note == 60 and velocity > 0 for starting-resuming robot sequence
          digitalWrite(13, HIGH); //turn on led
          playSequence = true;
        }
        else if (noteByte == 60 && velocityByte <= 0) {  //check for note off to stop
          playSequence = false;
        }
      }
    }
  }
  while (Serial1.available() > 2);//when at least three bytes available
}


void loop() {

  checkMIDI();
  delay(200);
  digitalWrite(13, LOW); //turn led off
  if (playSequence == true) {

    wristIndex = frameCount % 8;                                    //modulus the frame count to follow wrist sequence
    servoWrist.write(wristStart + wristSequence[wristIndex]);       //writes angel in the servo to perform sequence

    elbow2Index = frameCount % 14;                                  //modulus the frame count to follow elbow2 sequence
    servoElbow2.write(elbow2Start + elbow2Sequence[elbow2Index]);   //writes angle from reference start

    elbow1Index = frameCount % 42;                                  //modulus the frame count to follow elbow1 sequence
    servoElbow1.write(elbow1Start + elbow1Sequence[elbow1Index]);   //writes angle from reference start

    frameCount++;                                                   //increases frameCount to perform next frame
  }
}

