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
  size(1080, 460);
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
  // remoteWekiAudio = new NetAddress("192.168.43.102", 57120);
  remoteWekiAudio = new NetAddress("127.0.0.1", 4000);
  /// address for arduino
  myRemoteArduino = new NetAddress("192.168.8.11", 8888);

  //// address for resolume
  myRemoteReso = new NetAddress("127.0.0.1", 3000);
  //myRemoteReso = new NetAddress("192.168.43.188", 3000);

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
  pushStyle();
  fill(255);
  text("Receiving from ARDUINO on port" + localPort + " sending to Wek Helper " + myRemoteLocation  + 
    " Receiving from Wekinator on port  " + localPort + " Sending to Resolume to port " + myRemoteReso, 20, 430);
  text("Sending to wekinator for MAX/MSP AUDIO to port: " + remoteWekiAudio, 20, 450);
  popStyle();
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
  pushStyle();
  fill(#e66607);
  float processHeight0 = map(p0, 0, 1, 0, 200);
  rect(540, 250, 50, -processHeight0);
  float processHeight1 = map(p1, 0, 1, 0, 200);
  rect(610, 250, 50, -processHeight1);
  float processHeight2 = map(p2, 0, 1, 0, 200);
  rect(680, 250, 50, -processHeight2);
  float processHeight3 = map(p3, 0, 1, 0, 200);
  rect(750, 250, 50, -processHeight3);
  float processHeight4 = map(p4, 0, 1, 0, 200);
  rect(810, 250, 50, -processHeight4);
  float processHeight5 = map(p5, 0, 1, 0, 200);
  rect(880, 250, 50, -processHeight5);
  float processHeight6 = map(p6, 0, 1, 0, 200);
  rect(950, 250, 50, -processHeight6);
  popStyle();

  // labels and numbers of output
  pushStyle();
  textSize(12);
  fill(255);
  text("TEMPO", 540, 270);
  text(p0, 540, 290);
  text("PITCH", 610, 270);
  text(p1, 610, 290);
  text("DIR-REP", 680, 270);
  text(p2, 680, 290);
  text("TRACK-SFL", 750, 270);
  text(p3, 750, 290);
  text("PAN", 810, 270);
  text(p4, 810, 290);
  text("FREQ-F", 880, 270);
  text(p5, 880, 290);
  text("GAIN-F", 950, 270);
  text(p6, 950, 290);
  popStyle();
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

//*NICE CONTROLS FUNCTIONS*//
public void bang() {
  int theColor = (int)random(255);
  myColorBackground = color(theColor);
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage ardMessage = new OscMessage("/calib/");
  ardMessage.add(int(random(10))); /* add an int to the osc message */
  /* send the message */
  oscP5.send(ardMessage, myRemoteArduino); 
  println("### bang().sending Calib instruction " + ardMessage);
}
