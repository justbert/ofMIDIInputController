#pragma once

#include "ofxMidi.h"
#include "ofxMidiMessage.h"
#include "ControllerComponents.h"

enum DDJSB2NoteOnOFf
{
	NoteOn = 0x7F,
	NoteOff = 0x00
};

class DDJSB2SliderKnob : public MIDIComponent
{
public:
	DDJSB2SliderKnob()
	{
		channel = -1;
		value = 0;
		MSBNote = -1;
		LSBNote = -1;
	}

	DDJSB2SliderKnob(int channel, int MSBData, int LSBData)
		: MIDIComponent(channel),
		MSBNote(MSBData),
		LSBNote(LSBData)
	{
		value = 0;
	}

	
	int getMSBNote()
	{
		return MSBNote;
	}

	int getLSBNote()
	{
		return LSBNote;
	}

	int getValue()
	{
		return value;
	}

	void newMidiMessage(ofxMidiMessage& eventArgs) override;

protected:
	int MSBNote;
	int LSBNote;
	int value;
};

class DDJSB2Platter: public MIDIComponent
{
public:
	DDJSB2Platter()
	{
		touched = false;
		touchNote, rotateNote = -1;
	}

	DDJSB2Platter(int channel, int touchNote, int rotateNote)
		:MIDIComponent(channel),
		touchNote(touchNote),
		rotateNote(rotateNote)
	{
		touched = false;
	}

	bool isTouched()
	{
		return touched;
	}

	void newMidiMessage(ofxMidiMessage& eventArgs) override;

protected:
	bool touched;
	int touchNote;
	int rotateNote;
};

class DDJSB2Button : public MIDIComponent
{
public:
	DDJSB2Button()
	{
		pressed = false;
	}

	DDJSB2Button(int channel, int note) : 
		MIDIComponent(channel),
		note(note)
	{
		pressed = false;
		delta = 0.0;
	}

	bool isPressed()
	{
		return pressed;
	}

	//override
	void newMidiMessage(ofxMidiMessage& eventArgs) override;

protected:
	bool pressed;
	int note;
};

class DDJSB2Components : public MIDIControllerComponents
{
public:
	DDJSB2Components(ofxMidiIn midiIn) : MIDIControllerComponents(midiIn) {}

	bool populateFromXML(string filename) override;

	DDJSB2SliderKnob* addSlider(string name, int channel, int MSBData, int LSBData);
	DDJSB2SliderKnob* getSlider(string name) override;
	DDJSB2SliderKnob* addKnob(string name, int channel, int MSBData, int LSBData);
	DDJSB2SliderKnob* getKnob(string name) override;
	DDJSB2Button* addButton(string name, int channel, int note);
	DDJSB2Button* getButton(string name) override;

protected:
	//map<string, DDJSB2Button*> BUTTONS;
	//map<string, DDJSB2SliderKnob*> SLIDERS;
	//map<string, DDJSB2SliderKnob*> KNOBS;
};