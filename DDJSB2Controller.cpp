#include "DDJSB2Controller.h"

void DDJSB2SliderKnob::newMidiMessage(ofxMidiMessage& eventArgs)
{
	cout << "Message received.";

	if (eventArgs.channel == channel && eventArgs.status == MIDI_CONTROL_CHANGE)
	{
		cout << "Message for slider" << endl;
		if(eventArgs.control == MSBNote)
		{
			this->value = 0;
			cout << "Value: " << eventArgs.value << endl;
			//first half of 14bit value
			this->value = eventArgs.value;
			this->value <<= 7;
			cout << "MSB: Current value :" << this->value << endl;
		} 
		else if (eventArgs.control == LSBNote)
		{
			//Second half of the 14bit value;
			this->value |= eventArgs.value;
			cout << "LSB: Current value :" << this->value << endl;
		}
		
		this->delta = eventArgs.deltatime;
	}
}

void DDJSB2Platter::newMidiMessage(ofxMidiMessage& eventArgs)
{
	if(eventArgs.channel == channel 
		&& eventArgs.status == MIDI_CONTROL_CHANGE
		|| eventArgs.status == MIDI_NOTE_ON)
	{
		if(eventArgs.control == rotateNote)
		{
			
		} 
		else if(eventArgs.control == touchNote)
		{
			if(eventArgs.value == NoteOn)
			{
				this->touched = true;
			}
			else if(eventArgs.value == NoteOff)
			{
				this->touched = false;
			}
		}
	}
}

void DDJSB2Button::newMidiMessage(ofxMidiMessage& eventArgs)
{
	if (eventArgs.channel == channel 
		&& eventArgs.status == MIDI_NOTE_ON
		&& eventArgs.control == note)
	{
		if (eventArgs.value == NoteOn)
		{
			this->pressed = true;
		}
		else if (eventArgs.value == NoteOff)
		{
			this->pressed = false;
		}

		this->delta = eventArgs.deltatime;
	}
}

DDJSB2SliderKnob*  DDJSB2Controller::addSlider(string name, int channel, int MSBData, int LSBData)
{
	SLIDERS[name] = new DDJSB2SliderKnob(channel, MSBData, LSBData);
	return SLIDERS[name];
}

DDJSB2SliderKnob* DDJSB2Controller::getSlider(string name)
{
	map<string, DDJSB2SliderKnob*>::iterator it = SLIDERS.find(name);

	if (it != SLIDERS.end())
	{
		return (it->second);
	}

	return nullptr;
}

DDJSB2SliderKnob* DDJSB2Controller::addKnob(string name, int channel, int MSBData, int LSBData)
{
	KNOBS[name] = new DDJSB2SliderKnob(channel, MSBData, LSBData);
	return KNOBS[name];
}

DDJSB2SliderKnob* DDJSB2Controller::getKnob(string name)
{
	map<string, DDJSB2SliderKnob*>::iterator it = KNOBS.find(name);

	if (it != KNOBS.end())
	{
		return (it->second);
	}

	return nullptr;
}

DDJSB2Button* DDJSB2Controller::addButton(string name, int channel, int note)
	{
		BUTTONS[name] = new DDJSB2Button(channel, note);
		return BUTTONS[name];
	}

DDJSB2Button* DDJSB2Controller::getButton(string name)
	{
		auto it = BUTTONS.find(name);

		if (it != BUTTONS.end())
		{
			return (it->second);
		}

		return nullptr;
	}