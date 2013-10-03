/*
 *  RivarMode.h
 *  RivAR
 *
 *  Created by Lauren McCarthy on 09-25-13.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxARToolkitPlus.h"
//#include "ofxQTVideoSaver.h"
#include "ofxOculusRift.h"

#include "Timer.h"


class RivarMode {
	
public:
	RivarMode(int width, int height);
	~RivarMode();
	
	virtual void enter();
	virtual void exit();
	virtual void draw()=0;
	virtual void update()=0;
	virtual void handleKey(int key)=0;
	
	int width;
	int	height;
};

