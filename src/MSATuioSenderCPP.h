/*
 TuioPad http://www.tuio.org/
 An Open Source TUIO App for iOS based on OpenFrameworks
 (c) 2010 by Mehmet Akten <memo@memo.tv> and Martin Kaltenbrunner <modin@yuri.at>
 
 TuioPad is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TuioPad is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TuioPad.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "TuioServer.h"
#include "UdpSender.h"
#include "TcpSender.h"
#include "TuioCursor.h"

#define OF_MAX_TOUCHES 10
using namespace TUIO;

class MyCursorInfo {
public:
	float x, y;
	
	bool isAlive;		// is it alive this frame
	bool wasAlive;		// was it alive this frame
	bool moved;			// did it move this frame
	
	
	MyCursorInfo() {
		isAlive		= false;
		wasAlive	= false;
		moved		= false;
	}
};



class MSATuioSenderCPP { 
public:
	bool verbose;
	OscSender		*oscSender;
	TuioServer		*tuioServer;
	
	MSATuioSenderCPP() {
		oscSender	= NULL;
		tuioServer	= NULL;
		//host		= "";
		//port		= 0;
		verbose		= false;
	}
	
	~MSATuioSenderCPP() {
		if (tuioServer) delete tuioServer;
		if (oscSender)  delete oscSender;
	};
	
	void setup(std::string host, int port, int tcp, std::string ip);
	void update();
	void close();

	void cursorPressed(float x, float y, int cursorId);
	void cursorReleased(float x, float y, int cursorId);
	void cursorDragged(float x, float y, int cursorId);
	
protected:
	//std::string		host;
	//int				port;
	TuioCursor		*tuioCursor[OF_MAX_TOUCHES];
	MyCursorInfo	myCursor[OF_MAX_TOUCHES];
	TuioTime		currentTime;
};