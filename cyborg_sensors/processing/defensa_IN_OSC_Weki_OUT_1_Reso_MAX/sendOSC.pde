void sendOSC(float _p0, float _p1, float _p2, float _p3, float _p4, float _p5, float _p6, float _p7, float _p8, float _p9, boolean _bunOrMess) {
  
  // si es true el mensaje va a resolume, si es false va a max 
  if (_bunOrMess == true) {
    /* create an osc bundle */
    OscBundle myBundle = new OscBundle();
    /* createa new osc message object */
    OscMessage myMessage = new OscMessage("/composition/selectedlayer/video/opacity");
    myMessage.add((float)_p7);
    /* add an osc message to the osc bundle */
    myBundle.add(myMessage);
    /* reset and clear the myMessage object for refill. */
    myMessage.clear();
    /* refill the osc message object again */
    myMessage.setAddrPattern("/composition/selectedclip/transport/position/behaviour/speed");
    myMessage.add((float)_p8);
    myBundle.add(myMessage);
    myMessage.clear();
    /* refill the osc message object again */
    myMessage.setAddrPattern("/composition/selectedclip/transport/position/behaviour/playdirection");
    myMessage.add((float)_p9);
    myBundle.add(myMessage);
    myMessage.clear();
    oscP5.send(myBundle, myRemoteReso);
  } else {
    // este es el mensaje para MAX
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
