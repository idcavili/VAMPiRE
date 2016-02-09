@@ -0,0 +1,25 @@
#include "stdafx.h"
#include "effectnode.h"

void EffectNode::processEffect() //Process signal through plugin
{
	int n = 0;
	for (int i = 0; i < sizeof(modWires); i++) //Add modulation signals and adjust parameters
	{
		Node modSource = nodes.getAt(modWires(i).source);
		double modSample = modSource.outputBuffer[0];
		modWires[i].calcParam(modSample);
	}
	for (int i = 0; i < sizeof(midiWires); i++) //Add MIDI events
	{
		VstMidiEvent *wireEvents = midiWires[i].mapEvents();
		for (int j = 0; j < sizeof(wireEvents); i++)
		{
			inEvents[n++] = wireEvents(j);
		}
	}
	plugin.processEvents(inEvents); //Send MIDI events to plugin (Call to VST function)
	sumInputs(); //Sum audio signals at inputs
	plugin.processDoubleReplacing(&inputBuffers, &outputBuffers, bufferSize); //Call VST process function to process effect

}
