#include "ArMode.h"


ArMode::ArMode(int width, int height) : RivarMode(width, height) {
	
	useOculusOrientation = false;
	
	// Print the markers from the "AllBchThinMarkers.png" file in the data folder
#ifdef CAMERA_CONNECTED
	//vidGrabber.setDeviceID(0); // this uses external camera
	//vidGrabber.setDeviceID(1); // this uses macbook camera
	vidGrabber.initGrabber(width, height);
#else
	vidPlayer.loadMovie("marker.mov");
	vidPlayer.play();
#endif
	
	colorImage.allocate(width, height);
	grayImage.allocate(width, height);
	grayThres.allocate(width, height);
	
	// Load the image we are going to distort
	displayImage.loadImage("of.jpg");
	// Load the corners of the image into the vector
	int displayImageHalfWidth = displayImage.width / 2;
	int displayImageHalfHeight = displayImage.height / 2;
	displayImageCorners.push_back(ofPoint(0, 0));
	displayImageCorners.push_back(ofPoint(displayImage.width, 0));
	displayImageCorners.push_back(ofPoint(displayImage.width, displayImage.height));
	displayImageCorners.push_back(ofPoint(0, displayImage.height));
	
	// This uses the default camera calibration and marker file
	artk.setup(width, height);
	threshold = 85;
	artk.setThreshold(threshold);
	
	
	
	//------------------------------------------LOAD NAMES OF ALL MODELS--
	dir.listDir("models/");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
	
	
	// you can now iterate through the files and load them into the ofImage vector
	for(int i = 0; i < (int)dir.size(); i++){
		//cout << "model 1:" << dir.getName(i) <<endl;
		
		subDir.allowExt("3DS");
		subDir.allowExt("FBX");
		subDir.listDir("models/" +  dir.getName(i) + "/");
		subDir.sort();
		
		string mName = "models/" +  dir.getName(i) +"/"+ subDir.getName(0);
		string sName = "models/" +  dir.getName(i) +"/"+ "shadow.png";
		
		cout << "model 1:" << mName <<endl;
		
		
		subDir.allowExt("wav");
		subDir.allowExt("mp3");
		
		subDir.listDir("models/" +  dir.getName(i) + "/audio/");
		string aName = "NO AUDIO FILE";
		if (subDir.size() > 0)
		{
			aName = "models/" +  dir.getName(i) + "/audio/" + subDir.getName(0);
		}
		cout << "Audio 1:" << aName <<endl;
		
		models.push_back(mName);
		audioFiles.push_back(aName);
		shadowFiles.push_back(sName);
		
	}
	currentModel = 0;
	//------------------------------
	setUpModel();
	
	
	nineOneOneCall.loadSound(audioFiles[currentModel] );
	//nineOneOneCall.play();
	//nineOneOneCall.setVolume(10);
	
	
	
	ofBackground(127,127,127);
	havePreviousMatrix = false;
	screenMode = 0;
	
	modelAlpha = 0;
	
	fadeOutTimer.intialize(500);
	fadeOutTimer.start();
	
	
	haveMarker = false;
	haveMarkerLast = false;
	
	if (width <= 720)
	{
		minSize = 50;
	}
	else {
		minSize = 100;
	}
	
	tooSmall = false;
	
	toggleBlackBGround = false;
	
	audioOnMarkerDetection = true;
	
	modelScale = 1.0;
	//frameBuffer.allocate(ofGetScreenWidth(), ofGetScreenHeight(), GL_RGB, 0);
	

}


ArMode::~ArMode() {
	
}


//--------------------------------------------------------------
void ArMode::setUpModel()
{
	
	//riverModel.loadModel("river/River3D.3DS", MODL_SIZE);
	riverModel.loadModel(models[currentModel], MODL_SIZE);
	
	shadow.loadImage(shadowFiles[currentModel] );
	//
	
	ofDisableSeparateSpecularLight();
	
	
    riverModel.setRotation(0, 90, 1, 0, 0);
	//	riverModel.setRotation(1, 0, 90, 90, 0);
    // riverModel.setRotation(1, 180, 0, 0, 1);
	// riverModel.setScale(0.9, 0.9, 0.9);
    // riverModel.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
	riverModel.setPosition(-20, 0, 10);
	
	
}


//--------------------------------------------------------------
void ArMode::draw() {
	fbo.begin();
	
	if (screenMode  == 0)
	{
		drawRegular();
	}
	else {
		drawData();
	}
	
	fbo.end();
}



//--------------------------------------------------------------
void ArMode::drawRegular(){
	// Main image
	
	ofPushView();
	
	ofSetColor(0, 0, 0);
	ofRect(0,0,ofGetScreenWidth(),ofGetScreenHeight() );
	
	if(toggleBlackBGround == false) {
		ofSetColor(255, 255, 255);
		colorImage.draw(0, 0,  ofGetScreenWidth(), ofGetScreenHeight() );
	}
	
	
	//ofDrawBitmapString(ofToString(artk.getNumDetectedMarkers()) + " marker(s) found", 10, 20);
	
	//Ignore markers below a cetain size on screen.
	tooSmall = false;
	bool badData = false;
	int numDetected = artk.getNumDetectedMarkers();
	if (numDetected > 0)
	{
		vector<ofPoint> corners;
		artk.getDetectedMarkerBorderCorners(0, corners);
		
		//for (int i = 0; i < corners.size(); i++) {
		//	ofCircle(corners[i].x, corners[i].y, 10);
		//}
		
		float dx1 = abs(corners[0].x - corners[2].x);
		float dy1 = abs(corners[0].y - corners[2].y);
		float dist1 = sqrt( (dx1*dx1) + (dy1*dy1) );
		
		float dx2 = abs(corners[1].x - corners[3].x);
		float dy2 = abs(corners[1].y - corners[3].y);
		float dist2 = sqrt( (dx2*dx2) + (dy2*dy2) );
		
		
		//ofDrawBitmapString("Dist 1 = " + ofToString(dist1) , 10, 40);
		//ofDrawBitmapString("Dist 2 = " + ofToString(dist2) , 10, 60);
		
		
		if ( dist1 < minSize && dist2 < minSize)
		{
			//tooSmall = true;
		}
		else {
			tooSmall = false;
		}
		
		
		if (tooSmall == true) {
			
			ofSetColor(0, 255, 255);
			//	ofCircle(100,100, 30,30);
		}
		
		
		
		
		for(int i = AVGRANGE-1; i > 0 ; i--)
		{
			posArray[i] = posArray[i-1];
		}
		posArray[0] = artk.getCameraPosition(0);
		translationData =  artk.getCameraPosition(0);
		
		float avg0 = 0;
		float avg1 = 0;
		float avg2 = 0;
		
		int count = 0;
		for (int i =1; i<AVGRANGE; i++) {
			avg0 += posArray[i].x;
			avg1 += posArray[i].y;
			avg2 += posArray[i].z;
			count++;
		}
		avg0 = avg0/float(count);
		avg1 = avg1/float(count);
		avg2 = avg2/float(count);
		
		
		float diffX = abs(abs(translationData[0] - avg0)/ avg0 );
		float diffY = abs(abs(translationData[1] - avg1)/ avg1 );
		float diffZ = abs(abs(translationData[2] - avg2)/ avg2 );
		
		int minDiff = 5;
		if (diffX > minDiff || diffY > minDiff || diffZ > minDiff )
		{
			ofSetColor(255, 0, 255);
			//	ofCircle(100,200, 30,30);
			
			//badData = true;
		}
		//	ofDrawBitmapString("DIFF Data = " + ofToString(diffX,0) + ",    "  + ofToString(diffY,0) + ",    " + ofToString(diffZ,0)  , 10, 100);
		
		//	ofDrawBitmapString("Translation Data = " + ofToString(translationData[0],0) + ",    "  + ofToString(translationData[1],0) + ",    " + ofToString(translationData[2],0)  , 10, 100);
		
	}
	
	// ARTK 2D stuff
	// See if marker ID '0' was detected
	// and draw blue corners on that marker only
	int myIndex = artk.getMarkerIndex(0);
	if(myIndex >= 0) {
		// Get the corners
		vector<ofPoint> corners;
		artk.getDetectedMarkerBorderCorners(myIndex, corners);
		
		
		// Can also get the center like this:
		// ofPoint center = artk.getDetectedMarkerCenter(myIndex);
		ofSetColor(255, 255, 255);
		for(int i=0;i<corners.size();i++) {
			//ofCircle(corners[i].x, corners[i].y, 10);
		}
	}
	artk.applyProjectionMatrix(ofGetScreenWidth(), ofGetScreenHeight() );
	glMatrixMode(GL_MODELVIEW);
	ofEnableAlphaBlending();
	
	if (numDetected > 0 && badData == false)
	{
		
		if (audioOnMarkerDetection == true && nineOneOneCall.getIsPlaying() == false && ofGetElapsedTimeMillis() > 3000)
		{
			nineOneOneCall.play();
			nineOneOneCall.setLoop(false);
		}
		
		haveMarker = true;
		
		if (haveMarkerLast == false)	
		{
			fadeOutTimer.start();
		}
		haveMarkerLast = true;
		
		
		float p = fadeOutTimer.getPercent();
		if (p > 1.0) p = 1.0;
		if (p < 0.0) p = 0;
		

		//if (modelAlpha < int(255*p)) {
		//	modelAlpha =  int(255*p);
		
		//}
		modelAlpha =  int(255*p);
		
		if ( modelAlpha > 255) modelAlpha =255;
		if (modelAlpha < 0 ) modelAlpha = 0;
		
		//cout << "PUVALUE:  " << p <<  "   alpha value: " << modelAlpha << endl;
	}
	else {
		haveMarker = false;
		if (haveMarker == false && haveMarkerLast == true )
		{
			fadeOutTimer.start();
		}
		haveMarkerLast = false;
		
	}
	
	
	
	// Draw for each marker discovered
	if (tooSmall == false && badData == false)
	{
		for(int i=0; i<numDetected; i++) {
			
			// Set the matrix to the perspective of this marker
			// The origin is in the middle of the marker
			artk.applyModelMatrix(i);
			
			
			lastMatrix = artk.getMatrix(0);
			havePreviousMatrix = true;
			
			drawModel();
		}
	}
	
	bool proceed = false;
	if (numDetected == 0 || tooSmall == true || badData == true )
	{
		proceed = true;
	}
	// && fadeOutTimer.isDone() == false
	if (proceed == true  && havePreviousMatrix == true && fadeOutTimer.isDone() == false) {
		
		//	if (numDetected == 0  && havePreviousMatrix == true && fadeOutTimer.isDone() == false) {
		
		
		float p = 1.0 - fadeOutTimer.getPercent();
		if ( p > 1.0) p = 1.0;
		if (p < 0) p = 0.0;
		p = p*p;
		
		
		if (p >= 0 && p < 1.0)
		{
			modelAlpha = int(255*p);
		}
		if ( modelAlpha > 255) modelAlpha =255;
		if (modelAlpha < 0 ) modelAlpha = 0;
		
		
		//cout << "P VALUE:  " << p <<  "   alpha value: " << modelAlpha << endl;
		
		lastMatrix(0,0);
		
		float m[ 16 ];
		m[0] = lastMatrix(0,0);
		m[1] = lastMatrix(1,0);
		m[2] = lastMatrix(2,0);
		m[3] = 0;
		
		m[4] = lastMatrix(0,1);
		m[5] = lastMatrix(1,1);
		m[6] = lastMatrix(2,1);
		m[7] = 0;
		
		m[8] = lastMatrix(0,2);
		m[9] = lastMatrix(1,2);
		m[10] = lastMatrix(2,2);
		m[11] = 0;
		
		m[12] = lastMatrix(0,3);
		m[13] = lastMatrix(1,3);
		m[14] = lastMatrix(2,3);
		m[15] = 1;
		
		glMatrixMode( GL_MODELVIEW );
		glLoadMatrixf( m );
		
		drawModel();
		//-------------
	}
	ofPopView();
}

//--------------------------------------------------------------
void ArMode::drawModel(){
	
	glPushMatrix();
	
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255);
	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	
	
	if (modelAlpha != 0)
	{
		float sScale = 1.3;
		shadow.draw( -(shadow.width* sScale)/2, -(shadow.height* sScale)/2, shadow.width* sScale, shadow.height*sScale );
	}
	ofDisableBlendMode();
	
	
	positionLights();
	ofSetColor(255, 255, 255, modelAlpha);
	riverModel.draw();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	
	
	//frameBuffer.end();
	//ofSetColor(255, 255, 255, modelAlpha);
	//frameBuffer.draw(0,0,ofGetScreenWidth(), ofGetScreenHeight() );
	
	glPopMatrix();
	
}



//--------------------------------------------------------------
void ArMode::drawData(){
	
	// Threshold image
	ofSetHexColor(0xffffff);
	//grayThres.draw(0, 0);
	ofSetHexColor(0x666666);
	ofDrawBitmapString("Threshold: " + ofToString(threshold), 30, 20);
	ofDrawBitmapString("Use the Up/Down keys to adjust the threshold", 30, 40);
	
	
	// ARTK draw
	// An easy was to see what is going on
	// Draws the marker location and id number
	artk.draw(0, 0);
	
}


//--------------------------------------------------------------
void ArMode::positionLights() {
	
	ofVec3f pos =   artk.getCameraPosition(0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_SMOOTH);
	GLfloat lightOnePosition[] = {pos.x + 40.0, pos.y + 40, pos.z + 0, 0.0};
	GLfloat lightOneColor[] = {0.99, 0.99, 0.99, 1.0};
	
	GLfloat lightTwoPosition[] = {pos.x -40.0, pos.y +40, pos.z + 0, 0.0};
	GLfloat lightTwoColor[] = {0.99, 0.99, 0.99, 1.0};
	
	GLfloat lightThreePosition[] = {pos.x, pos.y , pos.z +100.0, 0.0};
	GLfloat lightThreeColor[] = {0.99, 0.99, 0.99, 1.0};
	
	// initialize lighting //
	glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor);
	glEnable (GL_LIGHT0);
	glLightfv (GL_LIGHT1, GL_POSITION, lightTwoPosition);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
	glEnable (GL_LIGHT1);
	
	glLightfv (GL_LIGHT2, GL_POSITION, lightOnePosition);
	glLightfv (GL_LIGHT2, GL_AMBIENT_AND_DIFFUSE, lightOneColor);
	glEnable (GL_LIGHT2);
	
	glEnable (GL_LIGHTING);
	glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable (GL_COLOR_MATERIAL);
	
	ofEnableAlphaBlending();
	
}

//--------------------------------------------------------------
void ArMode::update() {
#ifdef CAMERA_CONNECTED
	vidGrabber.update();
	bool bNewFrame = vidGrabber.isFrameNew();
#else
	vidPlayer.idleMovie();
	bool bNewFrame = vidPlayer.isFrameNew();
#endif
	
	if(bNewFrame) {
		
#ifdef CAMERA_CONNECTED
		colorImage.setFromPixels(vidGrabber.getPixels(), width, height);
#else
		colorImage.setFromPixels(vidPlayer.getPixels(), width, height);
#endif
		
		// convert our camera image to grayscale
		grayImage = colorImage;
		// apply a threshold so we can see what is going on
		grayThres = grayImage;
		grayThres.threshold(threshold);
		
		// Pass in the new image pixels to artk
		artk.update(grayImage.getPixels());
		
	}
	
	fadeOutTimer.update();
	
}

//--------------------------------------------------------------
void ArMode::handleKey(int key) {
	
	if(key == OF_KEY_UP) {
		artk.setThreshold(++threshold);
		
	} else if(key == OF_KEY_DOWN) {
		artk.setThreshold(--threshold);
	}
	
#ifdef CAMERA_CONNECTED
	if(key == 'f') {
		vidGrabber.videoSettings();
	}
#endif
	
	if(key == 'g') {
		screenMode = !screenMode;
	}
	else if (key == 'a') {
		nineOneOneCall.play	();
		nineOneOneCall.setLoop(false);
	}
	
	else if (key == 'b') {
		nineOneOneCall.stop();
	}
	else if (key == 'q') {
		toggleBlackBGround = !toggleBlackBGround;
	}
	
	else if (key == 61) {
		modelScale += 0.1;
		printf("scale %f\n", modelScale);
		riverModel.setScale(modelScale,modelScale,modelScale);
	}
	else if (key == '-') {
		modelScale -= 0.1;
		printf("scale %f\n", modelScale);
		riverModel.setScale(modelScale, modelScale, modelScale);
	}
	
	else if (key == ' ') {
		currentModel++;
		if (currentModel >= models.size() ) currentModel = 0;
		
		riverModel.loadModel(models[currentModel], MODL_SIZE);
		shadow.loadImage(shadowFiles[currentModel] );
		
		nineOneOneCall.stop();
		nineOneOneCall.loadSound(audioFiles[currentModel] );
	}
	
	
	
}