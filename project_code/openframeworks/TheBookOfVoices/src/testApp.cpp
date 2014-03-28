#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    //printf("%s %s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));
    
    ofBackground(0);
    ofEnableDepthTest();
    ofSetVerticalSync(true);
    ofSetFullscreen(false);
    ofSetWindowShape(DA::STAGE_WIDTH, DA::STAGE_HEIGHT);
    
    letters.setup();
    
    wm = WordsManager::getInstance();
    sm = SoundManager::getInstance();
    cm = CameraManager::getInstance();

    gui.setup();
	gui.add(letters.cameraX.setup("Position X", 0.0f, -100.0f, 100.0f));
	gui.add(letters.cameraY.setup("Position Y", 0.0f, -100.0f, 100.0f));
    gui.add(letters.cameraZ.setup("Position Z", 200.0f, 25.0f, 2000.0f));
	gui.add(letters.lookAtX.setup("Look At X", 0.0f, -100.0f, 100.0f));
	gui.add(letters.lookAtY.setup("Look At Y", 0.0f, -100.0f, 100.0f));
    gui.add(letters.lookAtZ.setup("Look At Z", 0.0f, -100.0f, 100.0f));
    gui.add(letters.margin.setup("String Margin", 3.0f, 0.0f, 10.0f));
    gui.add(letters.lineSpace.setup("Line Space", 10.0f, 0.0f, 30.0f));
    gui.add(letters.blankWidth.setup("Blank Width", 8.0f, 0.0f, 20.0f));
    gui.add(letters.limitWidth.setup("Limit Width", 100.0f, 10.0f, 300.0f));
    gui.add(letters.distance.setup("Distance", 600.0f, 300.0f, 600.0f));
    gui.add(letters.radiusBias.setup("Radius Bias", 1.0f, 0.0f, 500.0f));
    gui.add(letters.fogStart.setup("Fog Start", 300.0f, 0.0f, 1000.0f));
    gui.add(letters.fogEnd.setup("Fog End", 800.0f, 0.0f, 1500.0f));
    gui.loadFromFile("settings.xml");
    
    debug = false;
}

//--------------------------------------------------------------
void testApp::update(){
    letters.update();
    
    cm->update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    letters.draw();

    if(debug){
        ofSetColor(255);
        ofDrawBitmapString("fps : "+ofToString(ofGetFrameRate()), ofGetWindowWidth()-120, 20);
        gui.draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == ' '){
    }else if(key=='f'){
        ofToggleFullscreen();
    }else if(key=='1'){
        cm->status = 1;
        sm->send("/stop");
    }else if(key=='2'){
        cm->status = 3;
        wm->sendNames();
    }else if(key=='b'){
        wm->bomb = true;
    }else if(key=='g'){
        debug = !debug;
    }
}