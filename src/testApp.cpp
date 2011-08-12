#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
	
	// OF Settings
	ofSetVerticalSync(true);
	ofSetFrameRate(60.0f);
	ofBackground(0,0,0);	
	ofSetLogLevel(OF_LOG_NOTICE);
	serial.setVerbose(false);
	
	// App Settings, from file
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
	
	// Enumerate Serial Devices
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	// Initialize Serial Connection
	serialRunning = serial.setup(device, 115200); //open the monitor's serial connection
	if(serialRunning) {
		message.append("* Connected To Monitor\n\n");
	} else {
		message.append("* Could not connect to Monitor.\n  Available devices:\n\n");
		printDevices(deviceList);
		message.append("\n\n");
		message.append("* Find your device in the list above\n   and enter it into settings.xml\n\n");
	}
	
	// elaborate string manipulation to print TUIO info
	stringstream buffer;
	buffer << "* Sending TUIO to " << host <<  "\n  on port " << port << "\n\n";
	string tuioMessage = buffer.str();
	message.append(tuioMessage);
	serial.flush(true, true);
	
	startTuio();
}

void testApp::printDevices(vector <ofSerialDeviceInfo> deviceList){
	for(vector<ofSerialDeviceInfo>::iterator it = deviceList.begin(); it != deviceList.end(); ++it) {
		message.append(it->getDevicePath());
		message.append("\n");
	}
}


void testApp::startTuio() {
	tuio.setup(host, port, 0, host);
}
 

void testApp::stopTuio(){
}

//--------------------------------------------------------------
void testApp::update(){
	if(serial.available() > 0) {
		while(serial.available() > 0) {
			readPacket();
		}
	}
	
	tuio.update();
}

void testApp::readPacket(){
	int bytesToRead = 6;
	int bytesRemaining = bytesToRead;
	
	if	(serial.available() > 0) {
		while (bytesRemaining > 0) {
			
			int bytesArrayOffset = bytesToRead - bytesRemaining;
			int result = serial.readBytes( &bytes[bytesArrayOffset],bytesRemaining );
			
			// check for error code
			if ( result == OF_SERIAL_ERROR ) {
				// something bad happened
				ofLog( OF_LOG_ERROR, "unrecoverable error reading from serial" );
				break;
			} else if ( result == OF_SERIAL_NO_DATA ) {
				// nothing was read, try again
			} else {
				// we read some data!
				bytesRemaining -= result;
			}
			
		}
	}
	
	int id = int(bytes[5]);
	
	if(id < MAX_POINTS) {
		int status = int(bytes[0]);
		
		// thanks to Joshua J Noble for the bitshifting help!
		float xPos = (int(bytes[2]) << 7 | int(bytes[1])) / 16383.0f; // byte1 (0 based) is the x low, byte2 is the x high
		float yPos = 1 - (int(bytes[4]) << 7 | int(bytes[3])) / 16383.0f; // byte3 is the y low, byte4Ê is the y high
		
		if(status == 128) {
			// point released
			points[id].active = false;
			tuio.cursorReleased(points[id].x, points[id].y, id);
			
		} else {
			// check to see if we're switching active states (ie just pressed)
			bool pressed = points[id].active == false;
			// switch point to active
			points[id].active = true;
			// update the point only if it's changed
			if(xPos != points[id].prevX) {
				points[id].prevX = points[id].x;
				points[id].x = xPos;
			}
			// update the point only if it's changed
			if(yPos != points[id].prevY) {
				points[id].prevY = points[id].y;
				points[id].y = yPos;
			}
			// send TUIO messages
			if(pressed) {
				tuio.cursorPressed(points[id].x, points[id].y, id);
			} else {
				tuio.cursorDragged(points[id].x, points[id].y, id);
			}
			
		}
	}
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
		if(points[i].active) {
			ofCircle(points[i].x * ofGetWidth(), points[i].y * ofGetHeight(), 5);
		}
	}
	
	//we draw our status message at the top
	ofSetColor(240, 240, 240);
	TTF.drawString(message, 100, 12);
}
