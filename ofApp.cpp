/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#include "ofApp.h"
#include "DDJSB2Components.h"

DDJSB2Components* components;
string sliders[] = { "crossfader", "channelFader1", "channelFader2" };
string knobs[] = { "trim1", "eqHi1", "eqMid1", "eqLow1", "trim2", "eqHi2", "eqMid2", "eqLow2" };
//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(255, 255, 255);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// print input ports to console
	midiIn.listPorts(); // via instance
	//ofxMidiIn::listPorts(); // via static as well
	
	// open port by number (you may need to change this)
	//midiIn.openPort(0);
	midiIn.openPort("DDJ-SB2 MIDI 0");
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	//idiIn.ignoreTypes(false, false, false);
	
	// add ofApp as a listener
	midiIn.addListener(this);
	
	// print received messages to the console
	midiIn.setVerbose(true);

	components = new DDJSB2Components(midiIn);
	//components.addSlider("crossfader", 7, 31, 63);
	//must be located in bin/data
	components->populateFromXML("Pioneer_DDJ-SB2.xml");
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(0);

	int y = 20;
	int value;

	for (string name : sliders)
	{
		DDJSB2SliderKnob* current = components->getSlider(name);
		if (current != nullptr) {
			text << "Name: " << name;
			ofDrawBitmapString(text.str(), 20, y);
			text.str(""); // clear
			value = current->getValue();
			ofDrawRectangle(20, y + 5, ofMap(value, 0, 16383, 0, ofGetWidth() - 40), 20);
			y += 40;
		}
	}

	for (string name : knobs)
	{
		DDJSB2SliderKnob* current = components->getKnob(name);
		if (current != nullptr) {
			text << "Name: " << name;
			ofDrawBitmapString(text.str(), 20, y);
			text.str(""); // clear
			value = current->getValue();
			ofDrawRectangle(20, y + 5, ofMap(value, 0, 16383, 0, ofGetWidth() - 40), 20);
			y += 40;
		}
	}
	//cout << components->getButton("play1")->isPressed();
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	delete components;

	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {

	// make a copy of the latest message
	midiMessage = msg;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	switch(key) {
		case 'l':
			midiIn.listPorts();
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased() {
}
