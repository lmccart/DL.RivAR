/*
 *  testApp.h
 *  RivAR
 *
 *  Created by Lauren McCarthy on 09-25-13.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxARToolkitPlus.h"
#include "ofx3DModelLoader.h"
//#include "ofxQTVideoSaver.h"
#include "ofxOculusRift.h"

#include "Timer.h"
#include "RivarMode.h"
#include "ArMode.h"
#include "MovieMode.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void drawScene(bool flat);
	
	void keyPressed(int key);
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
	vector<RivarMode *> modes;
	int curMode;
	
	// shaders
	ofDirectory dir;
	vector<string> shaderNames;
	int curShader;
	ofShader shader;
	bool useShader;
	
	// images
	vector<string> imageNames[3];
	ofImage image[3];
	int curImage[3]; // L, R, F
	bool showImage[3]; // L, R, F
	
	ofSpherePrimitive sphere;
};

