/*
 *  MovieMode.h
 *  RivAR
 *
 *  Created by Lauren McCarthy on 09-25-13.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxARToolkitPlus.h"
#include "ofx3DModelLoader.h"
//#include "ofxQTVideoSaver.h"
#include "ofxOculusRift.h"

#include "Timer.h"
#include "RivarMode.h"


class MovieMode : public RivarMode {
	
public:
	MovieMode(int width, int height);
	~MovieMode();
	
	void enter();
	void exit();
	void draw();
	void update();
	void handleKey(int key);
		
	ofVideoPlayer player;
	ofDirectory dir;
	ofDirectory subDir;
	vector<string> videos;
	
	int curVideo;

};

