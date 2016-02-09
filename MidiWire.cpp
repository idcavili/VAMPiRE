#include <stdafx>
#include "midiwire.h"

class MidiWire
{
	public void mapEvents()
	{
		VstMidiEvent *inEvents = nodes.getAt(source).outEvents;
		VstMidiEvent *wireEvents;
		int n = 0;
		for (int i = 0; i < sizeof(inEvents); i++)
		{
			int channel = inEvents(i).channel;
			for (int j = 0; j < 16; j++)
			{
				if (chanMap[channel, j])
				{
					VstMidiEvent ev = inEvents(i);
					inEvents.channel = j;
					wireEvents[n++] = ev;
				}
			}
		}
		return wireEvents;
	}
};
