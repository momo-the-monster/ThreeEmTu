#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	
	// OF Settings
	ofSetVerticalSync(true);
	ofSetFrameRate(60.0f);
	ofBackground(0,0,0);	
	
	// App Settings
	mode = MODE_ABSOLUTE;
	
	if( XML.loadFile("settings.xml") ){
		message = "* Settings Loaded\n\n";
	} else {
		message = "* Unable to load settings.xml, using defaults\n\n";
	}
	
	//read the settings from XML
	host	= XML.getValue("tuio:host", "127.0.0.1");
	port	= XML.getValue("tuio:port", 9999);
	device = XML.getValue("serial:device", "/dev/tty.PL2303-000013FD");
	
	// load the font for output messages
	TTF.loadFont("DIN.otf", 9);
	
	// load the background picture
	bg.loadImage("bg.png");
	
	// Init Serial-to-TUIO communication
	if(touchMon.setup(device, host, port, true)) {
		// serial & TUIO started successfully
		message.append("* Connected To Monitor\n\n");
		
		// elaborate string manipulation to print TUIO info
		stringstream buffer;
		buffer << "* Sending TUIO to " << host <<  "\n  on port " << port << "\n\n";
		string tuioMessage = buffer.str();
		message.append(tuioMessage);
		
	} else {
		message.append("* Could not connect to Monitor.\n  Available devices:\n\n");
		printDevices(touchMon.serial.getDeviceList());
		message.append("\n\n");
		message.append("* Find your device in the list above\n   and enter it into settings.xml\n\n");
	}
}

void testApp::printDevices(vector <ofSerialDeviceInfo> deviceList){
	for(vector<ofSerialDeviceInfo>::iterator it = deviceList.begin(); it != deviceList.end(); ++it) {
		message.append(it->getDevicePath());
		message.append("\n");
	}
}

//--------------------------------------------------------------
void testApp::update(){
	//ofx3M2 is set to auto-update, so nothing is needed here
}


//--------------------------------------------------------------
void testApp::draw(){
	
	ofEnableAlphaBlending();
	
	// Draw Background
	ofSetColor(255, 255, 255);
	bg.draw(0, 0);
	
	// Draw circles to show touch points
	ofSetColor(100, 255, 255);
	for (int i = 0; i < MAX_POINTS; ++i) {
		if(touchMon.points[i].active) {
			ofCircle(touchMon.points[i].x * ofGetWidth(), touchMon.points[i].y * ofGetHeight(), 5);
		}
	}
	
	//we draw our status message at the top
	ofSetColor(240, 240, 240);
	TTF.drawString(message, 100, 12);
}
