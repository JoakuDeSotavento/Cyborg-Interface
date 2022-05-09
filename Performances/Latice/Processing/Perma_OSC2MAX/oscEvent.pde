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
  sendOSC(p0, p1, p2, p3, p4, p5, p6, false);
}
