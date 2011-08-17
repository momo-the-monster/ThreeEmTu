/*
 *  ofx3m2.h
 *  Clusters
 *
 *  Created by Momo the Monster on 8/12/11.
 *  Copyright 2011 MMM Labs. All rights reserved.
 *
 */
#pragma once
	
#define PORT_SPEED	115200 // from the manual
#define MAX_POINTS	20
	
#include "MSATuioSenderCPP.h"
#include "ofMain.h"

// holds info about each serial touch point
struct touchPoint {
	float x;
	float y;
	float prevX;
	float prevY;
	bool active;
	
	touchPoint(){
		x = 0;
		y = 0;
		prevY = 0;
		prevX = 0;
		active = false;
	}
};

class ofx3m2 {	
	
public:
	
	// General Params
	bool						enabled;	// set this to false to temporarily disable all events
	
	// General Functions
								ofx3m2();
	bool						setup(string device, string ip, int port, bool autoUpdate);
	void						_update(ofEventArgs &e);
	void						_exit(ofEventArgs &e);
	void						enableAppEvents();
	void						disableAppEvents();
	
	// Serial Params
	ofSerial					serial;
	bool						bSerialRunning;
	vector<ofSerialDeviceInfo>	deviceList;
	unsigned char				bytes[6];
	string						serialDevice;
	touchPoint					points[MAX_POINTS];
	
	// Serial Functions
	void						readPacket();
	
	// TUIO Params
	MSATuioSenderCPP			tuio;
	string						tuioIP;
	int							tuioPort;
	
};