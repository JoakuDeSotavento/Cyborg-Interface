d = NetAddr.new("192.168.43.102", 57120);

d.sendMsg("/ping");

o = SynthiGME().run;

o.pairDevice;

o.run;

thisProcess.recompile;

OSCFunc.trace(true, true);

OSCFunc.trace(false);

FreqScope.new(400, 200, 0, server: o);


