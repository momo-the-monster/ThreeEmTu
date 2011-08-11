#pragma once
#define MAX_POINTS	20
#include "ofMain.h"
#include "MSATuioSenderCPP.h"
#include "ofxXmlSettings.h"

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

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void readPacket();

	void startTuio();
	void stopTuio();
	
	ofSerial	serial;
	unsigned char bytes[6];
	string host;
	string device;
	int port;
	MSATuioSenderCPP tuio;
	touchPoint points[MAX_POINTS];
	bool serialRunning;
	ofImage bg;

	ofTrueTypeFont TTF;
	
	string message;
	ofxXmlSettings XML;
};