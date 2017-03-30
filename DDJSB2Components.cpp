#include "DDJSB2Components.h"
#include "ofxXmlSettings.h"

void DDJSB2SliderKnob::newMidiMessage(ofxMidiMessage& eventArgs)
{
	//cout << "Message received.";

	if (eventArgs.channel == channel && eventArgs.status == MIDI_CONTROL_CHANGE)
	{
		//cout << "Message for slider" << endl;
		if(eventArgs.control == MSBNote)
		{
			this->value = 0;
			//cout << "Value: " << eventArgs.value << endl;
			//first half of 14bit value
			this->value = eventArgs.value;
			this->value <<= 7;
			//cout << "MSB: Current value :" << this->value << endl;
		} 
		else if (eventArgs.control == LSBNote)
		{
			//Second half of the 14bit value;
			this->value |= eventArgs.value;
			//cout << "LSB: Current value :" << this->value << endl;
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
		if(eventArgs.pitch == rotateNote)
		{
			
		} 
		else if(eventArgs.pitch == touchNote)
		{
			if(eventArgs.velocity == NoteOn)
			{
				this->touched = true;
			}
			else if(eventArgs.velocity == NoteOff)
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
		&& eventArgs.pitch == note)
	{
		if (eventArgs.velocity == NoteOn)
		{
			this->pressed = true;
		}
		else if (eventArgs.velocity == NoteOff)
		{
			this->pressed = false;
		}

		this->delta = eventArgs.deltatime;
	}
}

DDJSB2SliderKnob* DDJSB2Components::addSlider(string name, int channel, int MSBData, int LSBData)
{
	if (channel < 1 || channel > 16)
		return nullptr;
	
	auto pointer = new DDJSB2SliderKnob(channel, MSBData, LSBData);

	if (pointer != nullptr) {
		midiIn.addListener(pointer);
		SLIDERS[name] = pointer;
	}
	return pointer;
}

DDJSB2SliderKnob* DDJSB2Components::getSlider(string name)
{
	auto it = SLIDERS.find(name);

	if (it != SLIDERS.end())
	{
		return static_cast<DDJSB2SliderKnob*>(it->second);
	}

	return nullptr;
}

DDJSB2SliderKnob* DDJSB2Components::addKnob(string name, int channel, int MSBData, int LSBData)
{
	if (channel < 1 || channel > 16)
		return nullptr;

	auto pointer = new DDJSB2SliderKnob(channel, MSBData, LSBData);

	if (pointer != nullptr) {
		midiIn.addListener(pointer);
		KNOBS[name] = pointer;
	}
	return pointer;
}

DDJSB2SliderKnob* DDJSB2Components::getKnob(string name)
{
	auto it = KNOBS.find(name);

	if (it != KNOBS.end())
	{
		return static_cast<DDJSB2SliderKnob*>(it->second);
	}

	return nullptr;
}

DDJSB2Button* DDJSB2Components::addButton(string name, int channel, int note)
{
	if (channel < 1 || channel > 16)
		return nullptr;

	auto pointer = new DDJSB2Button(channel, note);

	if (pointer != nullptr) {
		midiIn.addListener(pointer);
		BUTTONS[name] = pointer;
	}

	return pointer;
}

DDJSB2Button* DDJSB2Components::getButton(string name)
{
	auto it = BUTTONS.find(name);

	if (it != BUTTONS.end())
	{
		return static_cast<DDJSB2Button*>(it->second);
	}

	return nullptr;
}

bool DDJSB2Components::populateFromXML(string filename)
{
	ofXml xml;
	xml.load(filename);
	
	if(!xml.exists("controller"))
	{
		xml.setTo("controller");
		auto currentAttributes = xml.getAttributes();

		if (currentAttributes["manufacturer"] != "pioneer" || currentAttributes["model"] != "ddj-sb2")
			return false;
	} else
	{
		return false;
	}

	for(int i = 0; i < xml.getNumChildren(); ++i)
	{
		xml.setToChild(i);

		auto attributes = xml.getAttributes();
		string type = xml.getName();

		if (type == "slider")
		{
			this->addSlider(attributes["name"], stoi(attributes["channel"]), stoi(attributes["msbnote"]), stoi(attributes["lsbnote"]));
		} 
		else if (type == "button")
		{
			this->addButton(attributes["name"], stoi(attributes["channel"]), stoi(attributes["note"]));
		} 
		else if (type == "knob")
		{
			this->addKnob(attributes["name"], stoi(attributes["channel"]), stoi(attributes["msbnote"]), stoi(attributes["lsbnote"]));
		} 
		else if (type == "platter")
		{
			
		}

		xml.setToParent();
	}

	return true;
}
