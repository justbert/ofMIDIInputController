#pragma once

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

class DDJSB2Controller : public MIDIControllerComponents
{
public:
	DDJSB2Controller()
	{

	}

	~DDJSB2Controller() 
	{
		for(auto it = BUTTONS.begin(); it != BUTTONS.end(); ++it)
		{
			delete it->second;
		}

		for (auto it = SLIDERS.begin(); it != SLIDERS.end(); ++it)
		{
			delete it->second;
		}

		for (auto it = KNOBS.begin(); it != KNOBS.end(); ++it)
		{
			delete it->second;
		}
	}

	//DDJSB2SliderKnob* addSlider(string name, int channel, int MSBData, int LSBData);
	DDJSB2SliderKnob* getSlider(string name);
	DDJSB2SliderKnob* addKnob(string name, int channel, int MSBData, int LSBData);
	DDJSB2SliderKnob* getKnob(string name);
	DDJSB2Button* addButton(string name, int channel, int note);
	DDJSB2Button* getButton(string name);

protected:
	//map<string, DDJSB2Button*> BUTTONS;
	map<string, DDJSB2SliderKnob*> SLIDERS;
	map<string, DDJSB2SliderKnob*> KNOBS;
};