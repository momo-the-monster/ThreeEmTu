#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofx3m2.h"
#define	MODE_ABSOLUTE 0
#define MODE_RELATIVE 1

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void printDevices(vector <ofSerialDeviceInfo> deviceList);
	
	// Serial
	ofx3m2 touchMon;
	ofSerial	serial;
	unsigned char bytes[6];
	string host;
	string device;
	int port;
	bool serialRunning;
	
	// Display
	ofImage bg;
	ofTrueTypeFont TTF;
	string message;
	
	// Other
	ofxXmlSettings XML;
	int	mode;
};