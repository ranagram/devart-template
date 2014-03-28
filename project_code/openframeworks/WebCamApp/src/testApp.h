#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxGui.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofVideoGrabber grabber;
    
	ofxCv::ContourFinder contourFinder;
	vector< vector<cv::Point> > quads;

    ofxCvColorImage target, resize;
    
    ofxOscSender sender;
    
    bool opened;

    ofxFloatSlider threshold, minRadius, maxRadius;
    ofxIntSlider startX, startY, width, height, widthLimit;
	ofxPanel gui;
};
