#include "MovieMode.h"


MovieMode::MovieMode(int width, int height) : RivarMode(width, height) {
	
	
	
	//------------------------------------------LOAD NAMES OF ALL MODELS--
	dir.listDir("videos/");
	dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
	
	for(int i = 0; i < (int)dir.size(); i++){
		
		subDir.allowExt("mov");
		
		string name = "videos/" +  dir.getName(i);
		videos.push_back(name);
		
		cout << "adding video: " << name << endl;
		
	}
	
	curVideo = -1;
	if (videos.size() > 0) {
		curVideo = 0;
	}
}


MovieMode::~MovieMode() {
	
}


//--------------------------------------------------------------
void MovieMode::enter() {
	if (curVideo > -1) {
		player.loadMovie(videos[curVideo]);
		cout << "loading video: " << videos[curVideo] << endl;
	}
}

//--------------------------------------------------------------
void MovieMode::draw() {
	player.draw(0, 0, width, height);
}

//--------------------------------------------------------------
void MovieMode::update() {
	player.update();
	if (player.isLoaded() && !player.isPlaying()) {
		player.play();
		cout << "playing video duration: " << player.getDuration() << endl;
	}
}



//--------------------------------------------------------------
void MovieMode::exit() {
	player.stop();
}

//--------------------------------------------------------------
void MovieMode::handleKey(int key) {
	
	if(key == OF_KEY_UP) {
		
	} else if(key == OF_KEY_DOWN) {
	}
	
	
	else if (key == ' ') {
		curVideo = (curVideo+1)%videos.size();
		player.loadMovie(videos[curVideo]);
	}
	
	
}