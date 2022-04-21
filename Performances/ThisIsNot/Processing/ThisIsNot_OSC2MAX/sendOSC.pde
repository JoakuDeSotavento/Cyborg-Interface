void sendOSC(float _p0, float _p1, float _p2, float _p3, float _p4, float _p5, float _p6, boolean _bunOrMess) {
  if (_bunOrMess == true) {
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
  } else {
    String [] messageToMax = {str(_p0), str(_p1), str(_p2), str(_p3), str(_p4), str(_p5), str(_p6)};
    OscMessage msg = new OscMessage("/max");
    StringBuilder sb = new StringBuilder();
    try {
      for (int i = 0; i < messageToMax.length; i++) {
        float f = Float.parseFloat(messageToMax[i]); 
        msg.add(f);
        sb.append(String.format("%.2f", f)).append(" ");
      }
      // this is for the wekinator raw for the Audio
      // dont need of extra translation it goes directli to the MAX/MSP
      oscP5.send(msg, remoteWekiAudio);
      featureString = sb.toString();
    } 
    catch (Exception ex) {
      println("Encountered exception parsing string: " + ex);
    }
  }
}
