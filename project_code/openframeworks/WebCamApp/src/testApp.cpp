#include "testApp.h"

static const int STAGE_WIDTH = 1280;
static const int STAGE_HEIGHT = 720;
static const string OSC_HOST = "localhost";
static const int OSC_PORT = 8001;

using namespace ofxCv;
using namespace cv;

void testApp::setup() {
    ofSetWindowShape(STAGE_WIDTH, STAGE_HEIGHT);
    ofSetVerticalSync(true);
    
    grabber.setDeviceID(1);
	grabber.initGrabber(STAGE_WIDTH, STAGE_HEIGHT);
    
    gui.setup();
	gui.add(threshold.setup("Threshold", 173.0f, 0.0f, 255.0f));
	gui.add(minRadius.setup("Min Radius", 50.0f, 5.0f, 300.0f));
    gui.add(maxRadius.setup("Max Radius", 300.0f, 10.0f, 500.0f));
    gui.add(startX.setup("Start X", 0, 0, STAGE_WIDTH-10));
    gui.add(startY.setup("Start Y", 0, 0, STAGE_HEIGHT-10));
    gui.add(width.setup("Width", STAGE_WIDTH, 10, STAGE_WIDTH));
    gui.add(height.setup("Height", STAGE_HEIGHT, 10, STAGE_HEIGHT));
    gui.add(widthLimit.setup("Width Limit", 5, 5, 200));
    gui.loadFromFile("settings.xml");

	contourFinder.setMinAreaRadius(minRadius);
	contourFinder.setMaxAreaRadius(maxRadius);

    sender.setup(OSC_HOST, OSC_PORT);
    
    opened = false;
}

void testApp::update() {
    if(startX+width > STAGE_WIDTH) width = STAGE_WIDTH - startX;
    if(startY+height > STAGE_HEIGHT) height = STAGE_HEIGHT - startY;
    
	grabber.update();
	if(grabber.isFrameNew()) {
        target.setFromPixels(grabber.getPixelsRef());
        target.mirror(false, true);

        resize.setFromPixels(target.getPixelsRef());

        resize.setROI(startX, startY, width, height);
        resize.flagImageChanged();

        contourFinder.setMinAreaRadius(minRadius);
        contourFinder.setMaxAreaRadius(maxRadius);
 		contourFinder.setThreshold(threshold);
		contourFinder.findContours(resize.getRoiPixelsRef());
		
		int n = contourFinder.size();
		quads.clear();
		quads.resize(n);
		for(int i = 0; i < n; i++) {
			quads[i] = contourFinder.getFitQuad(i);
		}
	}
    vector<cv::Rect> rects = contourFinder.getBoundingRects();
    if (!opened && rects.size()>0 && rects[0].width>widthLimit) {
        opened = true;
        ofxOscMessage m;
        m.setAddress("/book");
        m.addIntArg(1);
        sender.sendMessage(m);
    }else if(opened && (rects.size()==0 || (rects.size()>0 && rects[0].width<=widthLimit))){
        opened = false;
        ofxOscMessage m;
        m.setAddress("/book");
        m.addIntArg(0);
        sender.sendMessage(m);
    }
}

void testApp::draw() {

    ofSetColor(255);
	target.draw(0, 0);
    
    ofNoFill();
    ofSetColor(255, 255, 0);
    ofRect(startX, startY, width, height);

	ofPushMatrix();{
        ofTranslate(startX, startY);
        if (opened) {
            ofSetColor(255, 0, 0);
        }else{
            ofSetColor(0, 0, 255);
        }
        ofSetLineWidth(2);
        contourFinder.draw();
    }ofPopMatrix();
    ofFill();

    ofSetColor(255);
    ofDrawBitmapString("fps : "+ofToString(ofGetFrameRate()), ofGetWindowWidth()-120, 20);
    gui.draw();
}