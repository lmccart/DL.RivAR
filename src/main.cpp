/*
 *  main.cpp
 *  RivAR
 *
 *  Created by Lauren McCarthy on 09-25-13.
 *
 */

#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1280,800,OF_WINDOW); // <-------- setup the GL context
	//ofSetupOpenGL(1920,1200,OF_WINDOW);	// <-------- setup the GL context // multiplying by 1.5 for better oculus mapping

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}