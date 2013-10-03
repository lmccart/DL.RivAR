#include "testApp.h"



//--------------------------------------------------------------
void testApp::setup(){
	width =  1280;//720;
	height = 720;//480;
	
	//ofSetLogLevel(OF_LOG_VERBOSE);
	
	ofSetVerticalSync(true);
	//CGDisplayHideCursor(NULL);
	ofHideCursor();
	
	ofBackground(127,127,127);
	
	
	// oculus stuff
	drawOculus = false;
	oculusRift.init( 1280, 800, 4 );
	oculusRift.setPosition( 0,-30,0 );
	
	// save and screencap stuff
	doScreenCap = true;
	nowSaving = false;
	screenCap.allocate(ofGetScreenWidth(), ofGetScreenHeight(), OF_IMAGE_COLOR);

	// build modes
	modes.push_back(new ArMode(width, height));
	modes.push_back(new MovieMode(width, height));
	curMode = 0;
}


//--------------------------------------------------------------
void testApp::update(){
	if (nowSaving == true)
	{
		// figure out how much time elapsed since the last frame
		float time = ofGetElapsedTimef() - mTimestamp;
		
		// add this frame to the movie
		//mVidSaver.addFrame(screenCap.getPixels(), time);
		
		// update the timestamp to the current time
		mTimestamp = ofGetElapsedTimef();
	}
	
	// update current mode
	modes[curMode]->update();
	
//	if( ofGetKeyPressed('i') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() + 0.001f ); }
//	else if( ofGetKeyPressed('o') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() - 0.001f ); }
//	else if( ofGetKeyPressed('k') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() + 1.0f ); }
//	else if( ofGetKeyPressed('l') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() - 1.0f ); }
	
	if (doScreenCap == true && nowSaving == true)
	{
		screenCap.grabScreen(0, 0, ofGetScreenWidth(), ofGetScreenHeight() );
		//cout << "screenCap";
		
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	if (drawOculus) {
		
		oculusRift.beginRenderSceneLeftEye();
		drawScene();
		//drawTestGraphics();
		oculusRift.endRenderSceneLeftEye();
		
		oculusRift.beginRenderSceneRightEye();
		drawScene();
		//drawTestGraphics();
		oculusRift.endRenderSceneRightEye();
		
		ofSetColor( 255 );
		oculusRift.draw( ofVec2f(0,0), ofVec2f( ofGetWidth(), ofGetHeight() ) );
		
	} else {
		drawScene();
	}
	
	
	if (nowSaving) {
		ofSetColor(255, 10, 10);
		ofCircle(ofGetScreenWidth()-20, 20, 10);
		ofSetColor(255, 255, 255);
		
	}
	

	
}


//--------------------------------------------------------------
void testApp::drawScene() {
	
	// update current mode
	modes[curMode]->draw();
}


//--------------------------------------------------------------
void testApp::drawTestGraphics() {

	ofSetColor(120);
	
	ofPushMatrix();
	ofRotate(90, 0, 0, -1);
	ofDrawGridPlane(500.0f, 40.0f, false );
	ofPopMatrix();
	
	ofSetColor( 255, 0, 0 );
	
	ofPushMatrix();
	ofTranslate( ofPoint(10,0,-80) );
	for( int i = 0; i < 20; i ++ )
	{
		ofBox( ofPoint(0,25,i * -100), 50);
	}
	ofPopMatrix();
	
	string tmpStr = "Do Warping: " + ofToString( oculusRift.getDoWarping() ) + "\n";
	tmpStr += "Inter Ocular Distance: "  + ofToString( oculusRift.getInterOcularDistance() ) + "\n";
	
	ofSetColor( 255 );
	
	ofPushMatrix();
	ofTranslate( ofPoint(-60,280,-200) );
	ofRotateZ( 180 );
	ofRotateY( 180 );
	ofPopMatrix();
	
	ofSetColor(255);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	
	printf("key %d\n", key);
	if(key == 'O') { // toggle oculus view
		drawOculus = !drawOculus;
		printf("drawing to oculus set to %d\n", drawOculus);
	}
	
	else if (key > 48 && key <= 57) { // go to mode
		if (key - 49 < modes.size()) {
			curMode = key - 49;
		}
	}
	
	else if (key == 'e') {
		screenCap.grabScreen(0, 0, ofGetScreenWidth(), ofGetScreenHeight() );
		
		string s = "screencap" + ofToString( ofGetTimestampString() ) + ".jpg";
		screenCap.saveImage(s, OF_IMAGE_QUALITY_BEST);
	}
	
	else if (key == 'd') {
		if (nowSaving == false && ofGetElapsedTimeMillis() > 2000) {
			saveMovie();
		}
		else {
			//mVidSaver.finishMovie();
			nowSaving = false;
		}
	}
	
	// pass on to mode handling
	else {
		modes[curMode]->handleKey(key);
	}
	
}

//--------------------------------------------------------------
void testApp::saveMovie()
{
	// create a unique file name for this movie file
	createFileName();
	
	mTimestamp = ofGetElapsedTimef();
	
	// init the movie saver
    //mVidSaver.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_NORMAL);
    //mVidSaver.setup(ofGetScreenWidth(), ofGetScreenHeight(), mFileName);
	
	
	nowSaving = true;
	
	
}

//--------------------------------------------------------------
void testApp::createFileName(void)
{
    // create a uniqe file name
	ostringstream oss;
	oss << "movieClips/";
	oss << ofGetYear() << "-";
	oss << setw(2) << setfill('0') << ofGetMonth() << "-";
	oss << setw(2) << setfill('0') << ofGetDay() << "-";
	oss << setw(2) << setfill('0') << ofGetHours() << "-";
	oss << setw(2) << setfill('0') << ofGetMinutes() << "-";
	oss << setw(2) << setfill('0') << ofGetSeconds() << ".mov";
	mFileName = oss.str();
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

