#pragma once

#include "ofMain.h"
#include "settings.h"
#include "ofxOsc.h"
#include "ofxCv.h"
#include "ofxGui.h"


#define PORT 9001
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	int globalVerboseLevel = 0;

	// Communication:
	ofxOscReceiver receiver;
	int currentMsgString;
	string msgStrings[NUM_MSG_STRINGS];
	float timers[NUM_MSG_STRINGS];

	// Video:
	ofVideoPlayer video;
	int video_r, video_g, video_b;

	// Contours:
	ofxCv::ContourFinder contourFinder;
	vector<string> videoPaths;

	// Controls:
	ofxPanel gui;
	ofParameter<float> minArea, maxArea, threshold;
	ofParameter<bool> holes;
	bool bDrawGui = false;
};
