#pragma once

#include "ofMain.h"
#include "ofxARToolkitPlus.h"
#include "ofx3DModelLoader.h"
//#include "ofxQTVideoSaver.h"
#include "ofxOculusRift.h"

#include "timer.h"
#include "rivarMode.h"
#include "arMode.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void drawScene();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	void drawTestGraphics();
	
	void createFileName(void);
	void saveMovie();
	
	/* Size of the image */
	int width, height;
	
	
	// oculus rift
	ofxOculusRift oculusRift;
	float lastUpdateTime;
	bool drawOculus;
	
	// screencap and saving
	ofImage screenCap;
	bool doScreenCap;
	bool nowSaving;
	//ofxQtVideoSaver mVidSaver;
	string mFileName;
	float mTimestamp;
	
	// modes
	vector<rivarMode *> modes;
	int curMode;
	
};

