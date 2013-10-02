#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxARToolkitPlus.h"
#include "ofx3DModelLoader.h"
//#include "ofxQTVideoSaver.h"
#include "ofxOculusRift.h"

#include "timer.h"
#include "rivarMode.h"



// Uncomment this to use a camera instead of a video file
#define CAMERA_CONNECTED

#define AVGRANGE 4
#define MODL_SIZE 60


class arMode : public rivarMode {
	
public:
	arMode(int width, int height);
	~arMode();
	
	void setUpModel();  //sets up and loads the 3d model;
	void draw();
	void drawRegular();
	void drawData();
	void drawModel();
	void update();
	void positionLights();
	
	/* Use either camera or a video file */
#ifdef CAMERA_CONNECTED
	ofVideoGrabber vidGrabber;
#else
	ofVideoPlayer vidPlayer;
#endif

	/* OpenCV images */
	ofxCvColorImage colorImage;
	ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage grayThres;
	
	/* Image to distort on to the marker */
	ofImage displayImage;
	/* The four corners of the image */
	vector<ofPoint> displayImageCorners;
	
	ofx3DModelLoader riverModel;
	ofImage shadow;
	
	
	int	modelAlpha;
	Timer fadeOutTimer;
	Timer fadeInTimer;
	
	bool haveMarker;
	bool haveMarkerLast;
	
	float modelScale;
	
	int minSize;
	bool tooSmall;
	
	ofVec3f translationData;
	ofVec3f posArray[AVGRANGE];
	
	ofSoundPlayer nineOneOneCall;
	bool audioOnMarkerDetection;
	
	
	/* ARToolKitPlus class */
	ofxARToolkitPlus artk;
	int threshold;
	int screenMode;
	
	bool havePreviousMatrix;
	ofMatrix4x4 lastMatrix;

	
	
	bool toggleBlackBGround;
	
	ofDirectory dir;
	ofDirectory subDir;
	vector<string> models;
	vector<string> shadowFiles;
	vector<string> audioFiles;
	int currentModel;
	
	

};

