/*
Joaku De Sotavento
 
 Modofictaion to the example of the library
 
 KinectPV2, Kinect for Windows v2 library for processing
 Thomas Sanchez Lengeling.
 http://codigogenerativo.com/
 
 */

import KinectPV2.KJoint;
import KinectPV2.*;

KinectPV2 kinect;


//esta es la parte de OSC

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress dest;
NetAddress dest2;

PFont f;

// data to send to wekinator
FloatList wings;


void setup() {
  size(1920, 1080, P3D);

  kinect = new KinectPV2(this);

  kinect.enableSkeletonColorMap(true);
  kinect.enableColorImg(true);

  kinect.init();

  // Parte de wekinator
  f = createFont("Courier", 16);
  textFont(f);

  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this, 12000);
  dest = new NetAddress("127.0.0.1", 6448);
  // important It send to two directions
  dest2 = new NetAddress("127.0.0.1", 7448);
  //initialize the FloatList
  wings = new FloatList(24);
}

void draw() {
  background(0);

  image(kinect.getColorImage(), 0, 0, width, height);

  // this gives all the SKELETON DATA
  //  KSKeleton kind array
  ArrayList<KSkeleton> skeletonArray =  kinect.getSkeletonColorMap();

  pushStyle();
  stroke(20, 255, 60);
  //ellipse(Kx, Ky, 100, 100);
  textSize(20);
  text((int)skeletonArray.size(), 400, 50);
  popStyle();

  // this is for sending the data to wekinator
  // I want to send elbows, twrist, thumb finger and pointer finger
  for (int i = 0; i < skeletonArray.size(); i++) {
    KSkeleton otroSkeleton = (KSkeleton)skeletonArray.get(0);
    if (otroSkeleton.isTracked()) {
      KJoint[] otrasJoints = otroSkeleton.getJoints();
      // here I prepare the OSC message for sending to wekinator
      makeOscMessage(otrasJoints);
      if (frameCount % 2 == 0) {
        sendOsc(wings);
      }

      //float Kx = otrasJoints[KinectPV2.JointType_Head].getX();
      //float Ky = otrasJoints[KinectPV2.JointType_Head].getY();

      // this print the value on screen of the sended data
      pushStyle();
      stroke(20, 255, 60);
      //ellipse(Kx, Ky, 100, 100);
      textSize(20);
      text((int)wings.size(), 350, 50);
      for (int m = 0; m < wings.size(); m++) {
        text((float)wings.get(m), 200, (m * 25) + 50);
      }
      popStyle();

      pushStyle();
      stroke(20, 255, 60);
      //ellipse(Kx, Ky, 100, 100);
      textSize(20);
      text((int)wings.size(), 350, 50);
      for (int m = 0; m < wings.size(); m++) {
        text((float)wings.get(m), 200, (m * 25) + 50);
      }
      popStyle();
    }
  }



  //individual JOINTS
  for (int i = 0; i < skeletonArray.size(); i++) {
    KSkeleton skeleton = (KSkeleton) skeletonArray.get(i);
    if (skeleton.isTracked()) {
      KJoint[] joints = skeleton.getJoints();
      color col  = skeleton.getIndexColor();
      fill(col);
      stroke(col);

      // this comand draw all the skeleton 
      // from here a took the data originally
      drawBody(joints);

      //draw different color for each hand state
      drawHandState(joints[KinectPV2.JointType_HandRight]);
      drawHandState(joints[KinectPV2.JointType_HandLeft]);
    }
  }

  fill(255, 0, 0);
  text(frameRate, 50, 50);
}

void makeOscMessage(KJoint[] _otrasJoints) {

  /* the names of this joints are:
   right wing
   KinectPV2.JointType_ShoulderRight
   KinectPV2.JointType_ElbowRight
   KinectPV2.JointType_WristRight
   KinectPV2.JointType_HandRight
   KinectPV2.JointType_ThumbRight
   KinectPV2.JointType_HandTipRight
   
   left wing
   KinectPV2.JointType_ShoulderLeft
   KinectPV2.JointType_ElbowLeft
   KinectPV2.JointType_WristLeft
   KinectPV2.JointType_HandLeft
   KinectPV2.JointType_ThumbLeft
   KinectPV2.JointType_HandTipLeft
   */

  //clear the system

  //initiallize for distances
  wings.clear();
  PVector jointRef = new PVector(0.0, 0.0);
  PVector jointDest = new PVector(0.0, 0.0);
  float dist2joint = 0.0;
  jointRef.set(_otrasJoints[KinectPV2.JointType_SpineMid].getX(), _otrasJoints[KinectPV2.JointType_SpineMid].getY());

  //Initiallize for angles
  float AnShoulderElbow = 0;
  float AnElbowWrist = 0;
  float AnWristFinger = 0;
  PVector VShoulder = new PVector(0.0, 0.0);
  PVector VElbow = new PVector(0.0, 0.0);
  PVector VWrist = new PVector(0.0, 0.0);
  PVector VHandTipe = new PVector(0.0, 0.0);

  //Put values in the vectors and make de distances
  jointDest.set(_otrasJoints[KinectPV2.JointType_ShoulderRight].getX(), _otrasJoints[KinectPV2.JointType_ShoulderRight].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  jointDest.set(_otrasJoints[KinectPV2.JointType_ElbowRight].getX(), _otrasJoints[KinectPV2.JointType_ElbowRight].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  jointDest.set(_otrasJoints[KinectPV2.JointType_WristRight].getX(), _otrasJoints[KinectPV2.JointType_WristRight].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  jointDest.set(_otrasJoints[KinectPV2.JointType_HandRight].getX(), _otrasJoints[KinectPV2.JointType_HandRight].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  jointDest.set(_otrasJoints[KinectPV2.JointType_ThumbRight].getX(), _otrasJoints[KinectPV2.JointType_ThumbRight].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  jointDest.set(_otrasJoints[KinectPV2.JointType_HandTipRight].getX(), _otrasJoints[KinectPV2.JointType_HandTipRight].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  //left wing////////////////////////////////////////////////////////////////

  jointDest.set(_otrasJoints[KinectPV2.JointType_ShoulderLeft].getX(), _otrasJoints[KinectPV2.JointType_ShoulderLeft].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  jointDest.set(_otrasJoints[KinectPV2.JointType_ElbowLeft].getX(), _otrasJoints[KinectPV2.JointType_ElbowLeft].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  jointDest.set(_otrasJoints[KinectPV2.JointType_WristLeft].getX(), _otrasJoints[KinectPV2.JointType_WristLeft].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  jointDest.set(_otrasJoints[KinectPV2.JointType_HandLeft].getX(), _otrasJoints[KinectPV2.JointType_HandLeft].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  jointDest.set(_otrasJoints[KinectPV2.JointType_ThumbLeft].getX(), _otrasJoints[KinectPV2.JointType_ThumbLeft].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  jointDest.set(_otrasJoints[KinectPV2.JointType_HandTipLeft].getX(), _otrasJoints[KinectPV2.JointType_HandTipLeft].getY());
  dist2joint = jointRef.dist(jointDest);
  wings.append(dist2joint);

  //Right angles

  VShoulder.set(_otrasJoints[KinectPV2.JointType_ShoulderRight].getX(), _otrasJoints[KinectPV2.JointType_ShoulderRight].getY());
  VElbow.set(_otrasJoints[KinectPV2.JointType_ElbowRight].getX(), _otrasJoints[KinectPV2.JointType_ElbowRight].getY());
  VWrist.set(_otrasJoints[KinectPV2.JointType_WristRight].getX(), _otrasJoints[KinectPV2.JointType_WristRight].getY());
  VHandTipe.set(_otrasJoints[KinectPV2.JointType_HandTipRight].getX(), _otrasJoints[KinectPV2.JointType_HandTipRight].getY());
  AnShoulderElbow = PVector.angleBetween(VShoulder, VElbow);
  AnElbowWrist = PVector.angleBetween(VElbow, VWrist);
  AnWristFinger = PVector.angleBetween(VWrist, VHandTipe);
  wings.append(AnShoulderElbow);
  wings.append(AnElbowWrist);
  wings.append(AnWristFinger);

  //Left Angles

  VShoulder.set(_otrasJoints[KinectPV2.JointType_ShoulderLeft].getX(), _otrasJoints[KinectPV2.JointType_ShoulderLeft].getY());
  VElbow.set(_otrasJoints[KinectPV2.JointType_ElbowLeft].getX(), _otrasJoints[KinectPV2.JointType_ElbowLeft].getY());
  VWrist.set(_otrasJoints[KinectPV2.JointType_WristLeft].getX(), _otrasJoints[KinectPV2.JointType_WristLeft].getY());
  VHandTipe.set(_otrasJoints[KinectPV2.JointType_HandTipLeft].getX(), _otrasJoints[KinectPV2.JointType_HandTipLeft].getY());
  AnShoulderElbow = PVector.angleBetween(VShoulder, VElbow);
  AnElbowWrist = PVector.angleBetween(VElbow, VWrist);
  AnWristFinger = PVector.angleBetween(VWrist, VHandTipe);
  wings.append(AnShoulderElbow);
  wings.append(AnElbowWrist);
  wings.append(AnWristFinger);
}

// very important note for wekinator: right now the program is sending 24 inputs
// This needs to be configured on wekinator

void sendOsc(FloatList _wings) {
  // this creat the Osc message
  OscMessage msg = new OscMessage("/wek/inputs");
  //this decompress the FloatList and makes the Osc message
  for (int n = 0; n < _wings.size(); n++) {
    msg.add((float)wings.get(n));
  }
  //this send the message
  oscP5.send(msg, dest);
  oscP5.send(msg, dest2);
}

//DRAW BODY
void drawBody(KJoint[] joints) {
  drawBone(joints, KinectPV2.JointType_Head, KinectPV2.JointType_Neck);
  drawBone(joints, KinectPV2.JointType_Neck, KinectPV2.JointType_SpineShoulder);
  drawBone(joints, KinectPV2.JointType_SpineShoulder, KinectPV2.JointType_SpineMid);
  drawBone(joints, KinectPV2.JointType_SpineMid, KinectPV2.JointType_SpineBase);
  drawBone(joints, KinectPV2.JointType_SpineShoulder, KinectPV2.JointType_ShoulderRight);
  drawBone(joints, KinectPV2.JointType_SpineShoulder, KinectPV2.JointType_ShoulderLeft);
  drawBone(joints, KinectPV2.JointType_SpineBase, KinectPV2.JointType_HipRight);
  drawBone(joints, KinectPV2.JointType_SpineBase, KinectPV2.JointType_HipLeft);

  // Right Arm
  drawBone(joints, KinectPV2.JointType_ShoulderRight, KinectPV2.JointType_ElbowRight);
  drawBone(joints, KinectPV2.JointType_ElbowRight, KinectPV2.JointType_WristRight);
  drawBone(joints, KinectPV2.JointType_WristRight, KinectPV2.JointType_HandRight);
  drawBone(joints, KinectPV2.JointType_HandRight, KinectPV2.JointType_HandTipRight);
  drawBone(joints, KinectPV2.JointType_WristRight, KinectPV2.JointType_ThumbRight);

  // Left Arm
  drawBone(joints, KinectPV2.JointType_ShoulderLeft, KinectPV2.JointType_ElbowLeft);
  drawBone(joints, KinectPV2.JointType_ElbowLeft, KinectPV2.JointType_WristLeft);
  drawBone(joints, KinectPV2.JointType_WristLeft, KinectPV2.JointType_HandLeft);
  drawBone(joints, KinectPV2.JointType_HandLeft, KinectPV2.JointType_HandTipLeft);
  drawBone(joints, KinectPV2.JointType_WristLeft, KinectPV2.JointType_ThumbLeft);

  // Right Leg
  drawBone(joints, KinectPV2.JointType_HipRight, KinectPV2.JointType_KneeRight);
  drawBone(joints, KinectPV2.JointType_KneeRight, KinectPV2.JointType_AnkleRight);
  drawBone(joints, KinectPV2.JointType_AnkleRight, KinectPV2.JointType_FootRight);

  // Left Leg
  drawBone(joints, KinectPV2.JointType_HipLeft, KinectPV2.JointType_KneeLeft);
  drawBone(joints, KinectPV2.JointType_KneeLeft, KinectPV2.JointType_AnkleLeft);
  drawBone(joints, KinectPV2.JointType_AnkleLeft, KinectPV2.JointType_FootLeft);

  drawJoint(joints, KinectPV2.JointType_HandTipLeft);
  drawJoint(joints, KinectPV2.JointType_HandTipRight);
  drawJoint(joints, KinectPV2.JointType_FootLeft);
  drawJoint(joints, KinectPV2.JointType_FootRight);

  drawJoint(joints, KinectPV2.JointType_ThumbLeft);
  drawJoint(joints, KinectPV2.JointType_ThumbRight);

  drawJoint(joints, KinectPV2.JointType_Head);
}

//draw joint
void drawJoint(KJoint[] joints, int jointType) {
  pushMatrix();
  translate(joints[jointType].getX(), joints[jointType].getY(), joints[jointType].getZ());
  ellipse(0, 0, 25, 25);
  popMatrix();
}

//draw bone
void drawBone(KJoint[] joints, int jointType1, int jointType2) {
  pushMatrix();
  translate(joints[jointType1].getX(), joints[jointType1].getY(), joints[jointType1].getZ());
  ellipse(0, 0, 25, 25);
  popMatrix();
  line(joints[jointType1].getX(), joints[jointType1].getY(), joints[jointType1].getZ(), joints[jointType2].getX(), joints[jointType2].getY(), joints[jointType2].getZ());
}

//draw hand state
void drawHandState(KJoint joint) {
  noStroke();
  handState(joint.getState());
  pushMatrix();
  translate(joint.getX(), joint.getY(), joint.getZ());
  ellipse(0, 0, 70, 70);
  popMatrix();
}

/*
Different hand state
 KinectPV2.HandState_Open
 KinectPV2.HandState_Closed
 KinectPV2.HandState_Lasso
 KinectPV2.HandState_NotTracked
 */
void handState(int handState) {
  switch(handState) {
  case KinectPV2.HandState_Open:
    fill(0, 255, 0);
    break;
  case KinectPV2.HandState_Closed:
    fill(255, 0, 0);
    break;
  case KinectPV2.HandState_Lasso:
    fill(0, 0, 255);
    break;
  case KinectPV2.HandState_NotTracked:
    fill(255, 255, 255);
    break;
  }
}