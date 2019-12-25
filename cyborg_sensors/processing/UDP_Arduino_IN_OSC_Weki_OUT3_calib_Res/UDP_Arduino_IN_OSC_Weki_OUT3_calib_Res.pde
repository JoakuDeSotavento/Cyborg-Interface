/*
Receives and visualizes OSCBundles sent over UDP
 
 Use with /examples/UDPSendMessage
 
 or with /examples/SerialSendMessage in conjunction 
 with /Applications/Processing/SLIPSerialToUDP
 */

import oscP5.*;
import netP5.*;

OscP5 oscP5;

NetAddress myRemoteLocation, myRemoteArduino, myRemoteReso, remoteWekiAudio;

int analogValue0 = 50;
int analogValue1 = 50;
int analogValue2 = 50;
int analogValue3 = 50;
int analogValue4 = 50;
int analogValue5 = 50;
int analogValue6 = 50;


int numFeatures = 0;
String featureString = "";


int xPos = 1;         // horizontal position of the graph
float inByte = 0;

int localPort = 12000;

int localPortWeki = 13000;

int localPortWekiAudio = 70000;


//*NICE CONTROLS VARIABLES*//

import controlP5.*;

ControlP5 cp5;
int myColorBackground = color(125, 125, 125);


///////// variables from wekinator to resolume

float p0, p1, p2, p3, p4, p5, p6;



void setup() {
  size(1080, 450);
  frameRate(30);
  //set this to the receiving port
  oscP5 = new OscP5(this, localPort);

  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */

  /// adress for wekinator helper
  myRemoteLocation = new NetAddress("127.0.0.1", 9000);
  remoteWekiAudio = new NetAddress("127.0.0.1", 5000);
  /// address for arduino
  myRemoteArduino = new NetAddress("192.168.43.250", 888);

  //// address for resolume
  //myRemoteReso = new NetAddress("127.0.0.1", 3000);

  myRemoteReso = new NetAddress("192.168.43.188", 3000);

  //*NICE CONTROLS INICILIZATION*//
  cp5 = new ControlP5(this);
  // change the trigger event, by default it is PRESSED.
  cp5.addBang("bang")
    .setPosition(550, 350)
    .setSize(280, 40)
    .setTriggerEvent(Bang.RELEASE)
    .setLabel("TUNNING THE BODY MUSIC")
    ;
}

void draw() {
  background(myColorBackground);
  //draw the analog values
  inputData();

  outputData();
  // stream data
  text("Receiving from ARDUINO on port" + localPort + " sending to Wek Helper " + myRemoteLocation  + 
    " Receiving from Wekinator on port  " + localPort + " Sending to Resolume to port " + myRemoteReso, 20, 430);
  text("Sending to wekinator for MAX/MSP AUDIO to port: " + remoteWekiAudio, 20, 450);
}

void inputData() {

  float analog0Height = map(analogValue0, 0, 1024, 0, 200);
  fill(255);
  rect(50, 250, 50, -analog0Height);
  //and the labels
  textSize(12);
  text("/analog/0", 50, 270);
  text(analogValue0, 50, 290);

  float analog1Height = map(analogValue1, 0, 1024, 0, 200);
  fill(255);
  rect(120, 250, 50, -analog1Height);
  //and the labels
  textSize(12);
  text("/analog/1", 120, 270);
  text(analogValue1, 120, 290);

  float analog2Height = map(analogValue2, 0, 1024, 0, 200);
  fill(255);
  rect(190, 250, 50, -analog2Height);
  //and the labels
  textSize(12);
  text("/analog/2", 190, 270);
  text(analogValue2, 190, 290);

  float analog3Height = map(analogValue3, 0, 1024, 0, 200);
  fill(255);
  rect(260, 250, 50, -analog3Height);
  //and the labels
  textSize(12);
  text("/analog/3", 260, 270);
  text(analogValue3, 260, 290);

  float analog4Height = map(analogValue4, 0, 1024, 0, 200);
  fill(255);
  rect(330, 250, 50, -analog4Height);
  //and the labels
  textSize(12);
  text("/analog/4", 330, 270);
  text(analogValue4, 330, 290);

  float analog5Height = map(analogValue5, 0, 1024, 0, 200);
  fill(255);
  rect(400, 250, 50, -analog5Height);
  //and the labels
  textSize(12);
  text("/analog/5", 400, 270);
  text(analogValue5, 400, 290);

  float analog6Height = map(analogValue6, 0, 1024, 0, 200);
  fill(255);
  rect(470, 250, 50, -analog6Height);
  //and the labels
  textSize(12);
  text("/analog/6", 470, 270);
  text(analogValue6, 470, 290);
}

void outputData() {

  float processHeight0 = map(p0, 0, 1, 0, 200);
  fill(255);
  rect(540, 250, 50, -processHeight0);
  //and the labels
  textSize(12);
  text("OPACITY", 540, 270);
  text(p0, 540, 290);

  float processHeight1 = map(p1, 0, 1, 0, 200);
  fill(255);
  rect(610, 250, 50, -processHeight1);
  //and the labels
  textSize(12);
  text("SPEED", 610, 270);
  text(p1, 610, 290);

  float processHeight2 = map(p2, 0, 1, 0, 200);
  fill(255);
  rect(680, 250, 50, -processHeight2);
  //and the labels
  textSize(12);
  text("DIRECCTION", 680, 270);
  text(p2, 680, 290);

  float processHeight3 = map(p3, 0, 9, 0, 200);
  fill(255);
  rect(750, 250, 50, -processHeight3);
  //and the labels
  textSize(12);
  text("CLIP K", 750, 270);
  text(p3, 750, 290);

  float processHeight4 = map(p4, 0, 1, 0, 200);
  fill(255);
  rect(810, 250, 50, -processHeight4);
  //and the labels
  textSize(12);
  text("POSITION X", 810, 270);
  text(p4, 810, 290);

  float processHeight5 = map(p5, 0, 1, 0, 200);
  fill(255);
  rect(880, 250, 50, -processHeight5);
  //and the labels
  textSize(12);
  text("POSITION X", 880, 270);
  text(p5, 880, 290);

  float processHeight6 = map(p6, 0, 1, 0, 200);
  fill(255);
  rect(950, 250, 50, -processHeight6);
  //and the labels
  textSize(12);
  text("SCALE", 950, 270);
  text(p6, 950, 290);
}

void sendFeaturesWeki(String[] s) {
  OscMessage msg = new OscMessage("/helper");
  StringBuilder sb = new StringBuilder();
  try {
    for (int i = 0; i < s.length; i++) {
      float f = Float.parseFloat(s[i]); 
      msg.add(f);
      sb.append(String.format("%.2f", f)).append(" ");
    }
    // this is for the weki helper for the video
    oscP5.send(msg, myRemoteLocation);
    // this is for the wekinator raw for the Audio
    // dont need of extra translation it goes directli to the MAX/MSP
    oscP5.send(msg, remoteWekiAudio);
    featureString = sb.toString();
  } 
  catch (Exception ex) {
    println("Encountered exception parsing string: " + ex);
  }
}

void sendOscReso(float _p0, float _p1, float _p2, float _p3, float _p4, float _p5, float _p6) {

  /* create an osc bundle */
  OscBundle myBundle = new OscBundle();

  /* createa new osc message object */
  OscMessage myMessage = new OscMessage("/composition/selectedlayer/video/opacity");
  myMessage.add((float)_p0);
  /* add an osc message to the osc bundle */
  myBundle.add(myMessage);
  /* reset and clear the myMessage object for refill. */
  myMessage.clear();

  /* refill the osc message object again */
  myMessage.setAddrPattern("/composition/selectedclip/transport/position/behaviour/speed");
  myMessage.add((float)_p1);
  myBundle.add(myMessage);

  myMessage.clear();

  /* refill the osc message object again */
  myMessage.setAddrPattern("/composition/selectedclip/transport/position/behaviour/playdirection");
  myMessage.add((float)_p2);
  myBundle.add(myMessage);

  myMessage.clear();

  /* refill the osc message object again */
  myMessage.setAddrPattern("/composition/selectedlayer/connectspecificclip");
  myMessage.add((float)_p3);
  myBundle.add(myMessage);

  myMessage.clear();

  /* refill the osc message object again */
  myMessage.setAddrPattern("/composition/selectedclip/video/effects/transform/positionx");
  myMessage.add((float)0.5);
  myBundle.add(myMessage);

  myMessage.clear();

  /* refill the osc message object again */
  myMessage.setAddrPattern("/composition/selectedclip/video/effects/transform/positiony");
  myMessage.add((float)0.5);
  myBundle.add(myMessage);

  myMessage.clear();

  /* refill the osc message object again */
  myMessage.setAddrPattern("/composition/selectedclip/video/effects/transform/scale");
  myMessage.add((float)0.1);
  myBundle.add(myMessage);

  //myMessage.clear();

  //myBundle.setTimetag(myBundle.now() + 10000);
  /* send the osc bundle, containing 2 osc messages, to a remote location. */
  oscP5.send(myBundle, myRemoteReso);
}

void drawGraph(int _xC, int _yC, int _signal) {

  stroke(127, 34, 255);
  rect(_xC, _yC, 100, 100);
  line(xPos, height, xPos, height - _signal);

  // at the edge of the screen, go back to the beginning:
  if (xPos >= width) {
    xPos = 0;
    background(0);
  } else {
    // increment the horizontal position:
    xPos++;
  }
}

// incoming osc message are forwarded to the oscEvent method. 
void oscEvent(OscMessage theOscMessage) {
  //println(theOscMessage.addrPattern());

  if (theOscMessage.addrPattern().equals("/analog/0")) {
    analogValue0 = theOscMessage.get(0).intValue();
  } else if (theOscMessage.addrPattern().equals("/analog/1")) {
    analogValue1 = theOscMessage.get(0).intValue();
  } else if (theOscMessage.addrPattern().equals("/analog/2")) {
    analogValue2 = theOscMessage.get(0).intValue();
  } else if (theOscMessage.addrPattern().equals("/analog/3")) {
    analogValue3 = theOscMessage.get(0).intValue();
  } else if (theOscMessage.addrPattern().equals("/analog/4")) {
    analogValue4 = theOscMessage.get(0).intValue();
  } else if (theOscMessage.addrPattern().equals("/analog/5")) {
    analogValue5 = theOscMessage.get(0).intValue();
  } else if (theOscMessage.addrPattern().equals("/analog/6")) {
    analogValue6 = theOscMessage.get(0).intValue();
  }
  String [] messageToWeki = {str(analogValue0), str(analogValue1), str(analogValue2), str(analogValue3), str(analogValue4), str(analogValue5), str(analogValue6)};
  sendFeaturesWeki(messageToWeki);

  if (theOscMessage.checkAddrPattern("/processing")==true) {
    if (theOscMessage.checkTypetag("fffffff")) { //Now looking for 7 parameters
      p0 = theOscMessage.get(0).floatValue(); //get this parameter
      p1 = theOscMessage.get(1).floatValue(); //get 2nd parameter
      p2 = theOscMessage.get(2).floatValue(); //get third parameters
      p3 = theOscMessage.get(3).floatValue(); //get third parameters
      p4 = theOscMessage.get(4).floatValue(); //get third parameters
      p5 = theOscMessage.get(5).floatValue(); //get third parameters
      p6 = theOscMessage.get(6).floatValue(); //get third parameters

      println("Received new params value from Wekinator");
    } else {   
      println("Error: unexpected params type tag received by Processing");
    }
  }
  //////// messages for Resolume Arena
  sendOscReso(p0, p1, p2, p3, p4, p5, p6);
}


//*NICE CONTROLS FUNCTIONS*//

public void bang() {
  int theColor = (int)random(255);
  myColorBackground = color(theColor);

  /* in the following different ways of creating osc messages are shown by example */
  OscMessage ardMessage = new OscMessage("/calib");

  ardMessage.add(random(10)); /* add an int to the osc message */

  /* send the message */
  oscP5.send(ardMessage, myRemoteArduino); 

  println("### bang().sending Calib instruction " + ardMessage);
}
