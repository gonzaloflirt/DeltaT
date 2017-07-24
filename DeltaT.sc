DeltaT : MultiOutUGen
{
	*ar { arg in1, in2, treshold = 0.5, release = 0.1, replyID = 0;
		^this.multiNew('audio', in1, in2, treshold, release, replyID);
	}
	init { arg ... theInputs;
		inputs = theInputs;
		channels = [
			OutputProxy(rate, this, 0),
			OutputProxy(rate, this, 1)
		];
		^channels
	}
}

