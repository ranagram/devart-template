//
//  CameraManager.cpp
//  DevArtStudy
//
//  Created by masahidey on 2014/03/27.
//
//

#include "CameraManager.h"

CameraManager* CameraManager::instance = NULL;

#pragma mark -
#pragma mark constructor & destructor - private

CameraManager::CameraManager()
{
}

CameraManager::~CameraManager()
{
    
}

#pragma mark -
#pragma mark public methods

void CameraManager::setup()
{
	receiver.setup(DA::CAMERA_PORT);
    
    wm = WordsManager::getInstance();
    sm = SoundManager::getInstance();
}

void CameraManager::update()
{
    while(receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);

        if(m.getAddress() == "/book"){
            if(m.getArgAsInt32(0)==1){
                status = 3;
                wm->sendNames();
            }else{
                status = 1;
                sm->send("/stop");
            }
        }
    }
}