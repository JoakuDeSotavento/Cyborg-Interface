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
    //oscP5.send(msg, remoteWekiAudio);
    featureString = sb.toString();
  } 
  catch (Exception ex) {
    println("Encountered exception parsing string: " + ex);
  }
}
