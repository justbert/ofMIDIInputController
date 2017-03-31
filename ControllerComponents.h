#pragma once

#include "ofxMidi.h"
#include "ofxMidiMessage.h"

class MIDIComponent : public ofxMidiListener
{
public:
	MIDIComponent()
	{
		channel = -1;
		delta = -1;
	}

	MIDIComponent(int channel) : channel(channel)
	{
		delta = 0;
	}

	int getChannel()
	{
		return channel;
	}

	float getDelta()
	{
		return delta;
	}

protected:
	int channel;
	float delta;
};

class MIDIControllerComponents
{
public:
	MIDIControllerComponents(ofxMidiIn midiIn) : midiIn(midiIn){}

	virtual ~MIDIControllerComponents()
	{
		for (auto it = BUTTONS.begin(); it != BUTTONS.end(); ++it)
		{
			midiIn.removeListener(it->second);
			delete it->second;
		}

		for (auto it = SLIDERS.begin(); it != SLIDERS.end(); ++it)
		{
			midiIn.removeListener(it->second);
			delete it->second;
		}

		for (auto it = KNOBS.begin(); it != KNOBS.end(); ++it)
		{
			midiIn.removeListener(it->second);
			delete it->second;
		}
	}

	virtual bool populateFromXML(string) = 0;
	virtual MIDIComponent* getSlider(string) = 0;
	virtual MIDIComponent* getButton(string) = 0;
	virtual MIDIComponent* getKnob(string) = 0;

protected:
	ofxMidiIn midiIn;
	map<string, MIDIComponent*> BUTTONS;
	map<string, MIDIComponent*> SLIDERS;
	map<string, MIDIComponent*> KNOBS;
};