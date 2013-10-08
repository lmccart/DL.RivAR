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
	//oculusRift.init( 1280, 800, 4 );
	oculusRift.init( width, height, 4 );
	oculusRift.setPosition( 0,-30,0 );
	
	// save and screencap stuff
	doScreenCap = true;
	nowSaving = false;
	screenCap.allocate(ofGetScreenWidth(), ofGetScreenHeight(), OF_IMAGE_COLOR);

	// build modes
	modes.push_back(new ArMode(width, height));
	modes.push_back(new MovieMode(width, height));
	
	// load shaders
	
	//------------------------------------------LOAD NAMES OF ALL SHADERS--
	dir.allowExt("vert");
	dir.listDir("shaders/");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
	for(int i = 0; i < (int)dir.size(); i++){
		
		
		string name = "shaders/" +  dir.getName(i).substr(0, dir.getName(i).length()-5);
		shaderNames.push_back(name);
		
		cout << "adding shader: " << name << endl;
		
	}
	
	curShader = -1;
	if (shaderNames.size() > 0) {
		curShader = 0;
		shader.load(shaderNames[curShader]);
	}
	
	// load images
	
	//------------------------------------------LOAD NAMES OF ALL SHADERS--
	dir.reset();
	dir.allowExt("png");
	dir.allowExt("jpg");
	dir.allowExt("gif");
	dir.listDir("images/");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
	
	for(int i = 0; i < (int)dir.size(); i++){
		
		
		string name = "images/" +  dir.getName(i).substr(0, dir.getName(i).length());
		if (ofIsStringInString(name, "-L")) {
			imageNames[0].push_back(name);
		} else if (ofIsStringInString(name, "-R")) {
			imageNames[1].push_back(name);
		} else if (ofIsStringInString(name, "-F")) {
			imageNames[2].push_back(name);
		}
		
		cout << "adding image: " << name << endl;
		
	}
	
	for (int i=0; i<3; i++) {
		curImage[i] = -1;
		if (imageNames[i].size() > 0) {
			curImage[i] = 0;
			image[i].loadImage(imageNames[i][0]);
		}
		showImage[i] = false;
	}
	
	// start current mode
	curMode = 0;
	modes[curMode]->enter();
	
	
	//ofDisableArbTex();
	sphere.set(1000, 30);
	sphere.mapTexCoordsFromTexture(modes[curMode]->fbo.getTextureReference());
	
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
	
	float currTime = ofGetElapsedTimef();
	float frameDeltaTime = currTime - lastUpdateTime;
	lastUpdateTime = currTime;
	
	if(		 ofGetKeyPressed('i') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() + 0.001f ); }
	else if( ofGetKeyPressed('o') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() - 0.001f ); }
	else if( ofGetKeyPressed('k') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() + 1.0f ); }
	else if( ofGetKeyPressed('l') ) { oculusRift.setInterOcularDistance( oculusRift.getInterOcularDistance() - 1.0f ); }
//	
//	if(	ofGetKeyPressed(OF_KEY_UP) )    { oculusRift.dolly(  30.0f * frameDeltaTime ); }
//	if( ofGetKeyPressed(OF_KEY_DOWN) )  { oculusRift.dolly( -30.0f * frameDeltaTime ); }
//	if( ofGetKeyPressed(OF_KEY_LEFT) )  { oculusRift.truck(  30.0f * frameDeltaTime ); }
//	if( ofGetKeyPressed(OF_KEY_RIGHT) ) { oculusRift.truck( -30.0f * frameDeltaTime ); }
//	
	
	if (doScreenCap == true && nowSaving == true) {
		screenCap.grabScreen(0, 0, ofGetScreenWidth(), ofGetScreenHeight() );
		//cout << "screenCap";
		
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	

	
	if (drawOculus) {
		
		oculusRift.beginRenderSceneLeftEye();
		drawScene(false);
		//drawTestGraphics();
		oculusRift.endRenderSceneLeftEye();
		
		oculusRift.beginRenderSceneRightEye();
		drawScene(false);
		oculusRift.endRenderSceneRightEye();
		
		ofSetColor( 255 );
		oculusRift.draw( ofVec2f(0,0), ofVec2f( ofGetWidth(), ofGetHeight() ) );
		
	} else {
		ofPushMatrix();
		ofScale(1.0, -1.0);
		drawScene(true);
		ofPopMatrix();
	}
	
	
	if (nowSaving) {
		ofSetColor(255, 10, 10);
		ofCircle(ofGetScreenWidth()-20, 20, 10);
		ofSetColor(255, 255, 255);
		
	}
	
}


//--------------------------------------------------------------
void testApp::drawScene(bool flat) {
	
//	if (useShader) {
//		shader.begin();
//		
//		//we want to pass in some varrying values to animate our type / color
//		shader.setUniform1f("timeValX", ofGetElapsedTimef() * 0.1 );
//		shader.setUniform1f("timeValY", -ofGetElapsedTimef() * 0.18 );
//		
//		//we also pass in the mouse position
//		//we have to transform the coords to what the shader is expecting which is 0,0 in the center and y axis flipped.
//		shader.setUniform2f("mouse", mouseX - ofGetWidth()/2, ofGetHeight()/2-mouseY );
//	}


	ofPushMatrix();
	
	//ofTranslate(width/2, height/2);
	ofScale(1.0, -1.0);
	
	modes[curMode]->draw();
	
	if (flat) {
		modes[curMode]->fbo.draw(0, 0);
	}
	
	else {
		modes[curMode]->fbo.getTextureReference().bind();
		sphere.draw();
		modes[curMode]->fbo.getTextureReference().unbind();
	}
	
	ofPopMatrix();

	//	for (int i=0; i<3; i++) {
	//		if (showImage[i]) {
	//			image[i].draw(0, 0, width, height);
	//		}
	//	}

	//
	//	if (useShader) {
	//		shader.end();
	//	}
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
		ofDrawBox( ofPoint(0,25,i * -100), 50);
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
			modes[curMode]->exit();
			curMode = key - 49;
			modes[curMode]->enter();
		}
	}
	
	else if (key == 's') {
		useShader = !useShader;
		printf("useShader set to %d\n", useShader);
	}
	else if (key == 'S') {
		curShader = (curShader+1)%shaderNames.size();
		shader.load(shaderNames[curShader]);
		printf("curShader set to %d\n", curShader);
	}
	
	
	
	else if (key == 'l') {
		showImage[0] = !showImage[0];
		printf("showImage Left set to %d\n", showImage[0]);
	}
	else if (key == 'L') {
		curImage[0] = (curImage[0]+1)%imageNames[0].size();
		image[0].loadImage(imageNames[0][curImage[0]]);
		printf("curImage Left set to %s\n", (char*)imageNames[0][curImage[0]].c_str());
	}
	/*
	else if (key == 'r') {
		showImage[0] = !showImage[0];
		printf("showImage Right set to %d\n", showImage[0]);
	}
	else if (key == 'R') {
		curImage[0] = (curImage[0]+1)%images[0].size();
	}
	
	else if (key == 'f') {
		showImage[0] = !showImage[0];
		printf("showImage Full set to %d\n", showImage[0]);
	}
	else if (key == 'F') {
		curImage[0] = (curImage[0]+1)%images[0].size();
	}
	
	*/
	
		
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

