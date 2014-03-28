#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "LettersController.h"
#include "WordsManager.h"
#include "SoundManager.h"
#include "CameraManager.h"

#include "config.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    
    LettersController letters;
    WordsManager *wm;
    SoundManager *sm;
    CameraManager *cm;
	ofxPanel gui;
    
    bool debug;
};
