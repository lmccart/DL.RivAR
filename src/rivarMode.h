#pragma once

#include "ofMain.h"
#include "ofxARToolkitPlus.h"
//#include "ofxQTVideoSaver.h"
#include "ofxOculusRift.h"

#include "timer.h"


class rivarMode {
	
public:
	rivarMode(int width, int height);
	~rivarMode();
	
	
	virtual void draw()=0;
	virtual void update()=0;

	int width;
	int	height;
};

