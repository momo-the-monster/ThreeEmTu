/*
 *  ofx3m2.cpp
 *  Clusters
 *
 *  Created by Momo the Monster on 8/12/11.
 *  Copyright 2011 MMM Labs. All rights reserved.
 *
 */

#include "ofx3m2.h"

ofx3m2::ofx3m2(){
	enabled = true;
	bSerialRunning = false;
	serial.listDevices();
	deviceList = serial.getDeviceList();
}

//--------------------------------------------------------------

bool ofx3m2::setup(string device, string ip, int port, bool autoUpdate) {
	// Initialize Serial Connection
	bSerialRunning = serial.setup(device, PORT_SPEED); //open the monitor's serial connection
	if(bSerialRunning) {
		// Clear Serial buffer, remember device
		//	serial.flush(true, true);
		serialDevice = device;
		
		// Initialize Tuio Communication
		tuio.setup(ip, port, 0, ip);
		
		// Remember TUIO values
		tuioIP = ip;
		tuioPort = port;
		
		// listen for update and exit events automatically
		if(autoUpdate) enableAppEvents();
		
		return true;
	} else {
		return false;
	}
}

//--------------------------------------------------------------

void ofx3m2::enableAppEvents() {
	ofAddListener(ofEvents.update, this, &ofx3m2::_update);
	ofAddListener(ofEvents.exit, this, &ofx3m2::_exit);
}

//--------------------------------------------------------------

void ofx3m2::disableAppEvents() {
	ofRemoveListener(ofEvents.update, this, &ofx3m2::_update);
	ofRemoveListener(ofEvents.exit, this, &ofx3m2::_exit);
}

//--------------------------------------------------------------

void ofx3m2::_update(ofEventArgs &e){
	if(!enabled) return;
	
	if(serial.available() > 0) {
		while(serial.available() > 0) {
			readPacket();
		}
	}
	tuio.update();
}

//--------------------------------------------------------------

void ofx3m2::_exit(ofEventArgs &e){
	if(!enabled) return;
	
	if(bSerialRunning) {
		serial.close();
	}
	
	// TODO: close tuio?
}

//--------------------------------------------------------------

void ofx3m2::readPacket(){
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
		
		// thanks to Joshua Noble for the bitshifting help!
		float xPos = (int(bytes[2]) << 7 | int(bytes[1])) / 16383.0f; // byte1 (0 based) is the x low, byte2 is the x high
		float yPos = 1 - (int(bytes[4]) << 7 | int(bytes[3])) / 16383.0f; // byte3 is the y low, byte4  is the y high
		
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