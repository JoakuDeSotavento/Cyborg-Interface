/*
Receives and visualizes OSCBundles sent over UDP
 
 Use with /examples/UDPSendMessage
 
 or with /examples/SerialSendMessage in conjunction 
 with /Applications/Processing/SLIPSerialToUDP
 */

import oscP5.*;
import netP5.*;

OscP5 oscP5;

NetAddress myRemoteLocation;

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


void setup() {
  size(1000, 300);
  frameRate(30);
  //set this to the receiving port
  oscP5 = new OscP5(this, 12000);

  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  myRemoteLocation = new NetAddress("127.0.0.1", 9000);
}


void draw() {
  background(0); 



  //draw the analog values
  float analog0Height = map(analogValue0, 0, 1024, 0, 200);
  fill(255);
  rect(50, 250, 50, -analog0Height);
  //and the labels
  textSize(12);
  text("/analog/0", 50, 270);

  float analog1Height = map(analogValue1, 0, 1024, 0, 200);
  fill(255);
  rect(120, 250, 50, -analog1Height);
  //and the labels
  textSize(12);
  text("/analog/1", 120, 270);

  float analog2Height = map(analogValue2, 0, 1024, 0, 200);
  fill(255);
  rect(190, 250, 50, -analog2Height);
  //and the labels
  textSize(12);
  text("/analog/2", 190, 270);

  float analog3Height = map(analogValue3, 0, 1024, 0, 200);
  fill(255);
  rect(260, 250, 50, -analog3Height);
  //and the labels
  textSize(12);
  text("/analog/3", 260, 270);

  float analog4Height = map(analogValue4, 0, 1024, 0, 200);
  fill(255);
  rect(330, 250, 50, -analog4Height);
  //and the labels
  textSize(12);
  text("/analog/4", 330, 270);

  float analog5Height = map(analogValue5, 0, 1024, 0, 200);
  fill(255);
  rect(400, 250, 50, -analog5Height);
  //and the labels
  textSize(12);
  text("/analog/5", 400, 270);

  float analog6Height = map(analogValue6, 0, 1024, 0, 200);
  fill(255);
  rect(470, 250, 50, -analog6Height);
  //and the labels
  textSize(12);
  text("/analog/6", 470, 270);
}

void sendFeatures(String[] s) {
  OscMessage msg = new OscMessage("/wek/inputs");
  StringBuilder sb = new StringBuilder();
  try {
    for (int i = 0; i < s.length; i++) {
      float f = Float.parseFloat(s[i]); 
      msg.add(f);
      sb.append(String.format("%.2f", f)).append(" ");
    }
    oscP5.send(msg, myRemoteLocation);
    featureString = sb.toString();
  } 
  catch (Exception ex) {
    println("Encountered exception parsing string: " + ex);
  }
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
  sendFeatures(messageToWeki);
}
