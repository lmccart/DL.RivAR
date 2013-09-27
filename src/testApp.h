#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxARToolkitPlus.h"
#include "ofx3DModelLoader.h"
//#include "ofxQTVideoSaver.h"
#include "timer.h"


// Uncomment this to use a camera instead of a video file
#define CAMERA_CONNECTED

#define AVGRANGE 4
#define MODL_SIZE 60


class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	
	void setUpModel();  //sets up and loads the 3d model;
	void drawModel();
	
	void drawRegular();
	void drawData();
	
	void positionLights();
	
	void createFileName(void);
	void saveMovie();
	
	/* Size of the image */
	int width, height;
	
	/* Use either camera or a video file */
#ifdef CAMERA_CONNECTED
	ofVideoGrabber vidGrabber;
#else
	ofVideoPlayer vidPlayer;
#endif
	
	/* ARToolKitPlus class */
	ofxARToolkitPlus artk;
	int threshold;
	
	/* OpenCV images */
	ofxCvColorImage colorImage;
	ofxCvGrayscaleImage grayImage;
	ofxCvGrayscaleImage	grayThres;
	
	/* Image to distort on to the marker */
	ofImage displayImage;
	/* The four corners of the image */
	vector<ofPoint> displayImageCorners;
	
	ofx3DModelLoader riverModel;
	ofImage shadow;
	int screenMode;
	
	bool	havePreviousMatrix;
	ofMatrix4x4 lastMatrix;
	
	int		modelAlpha;
	Timer	fadeOutTimer;
	Timer	fadeInTimer;
	
	bool	haveMarker;
	bool	haveMarkerLast;
	
	
	ofFbo	frameBuffer;
	
	
	//ofVec3f lastCam;
	ofImage screenCap;
	
	int	  minSize;
	bool  tooSmall;
	
	
	ofVec3f translationData;
	ofVec3f posArray[AVGRANGE];
	
	ofSoundPlayer nineOneOneCall;
	
	bool	audioOnMarkerDetection;
	bool	doScreenCap;
	
	
	
	bool				nowSaving;
	//ofxQtVideoSaver     mVidSaver;
	string				mFileName;
	float               mTimestamp;
	
	float				modelScale;
	
	
	ofDirectory dir;
	ofDirectory subDir;
	vector<string> models;
	vector<string> shadowFiles;
	vector<string> audioFiles;
	int currentModel;
	
	bool toggleBlackBGround;
};

