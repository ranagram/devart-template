//
//  CameraManager.h
//  TheBookOfVoices
//
//  Created by masahidey on 2014/03/27.
//
//

#ifndef __TheBookOfVoices__CameraManager__
#define __TheBookOfVoices__CameraManager__

#include "ofMain.h"
#include "ofxOsc.h"

#include "WordsManager.h"
#include "SoundManager.h"

#include "config.h"

class CameraManager
{
private:
    static CameraManager* instance;
    CameraManager();
    ~CameraManager();
public:
    // singleton --------------------------------------
    static CameraManager* getInstance()
    {
        if (instance == NULL)
        {
            instance = new CameraManager();
            instance->setup();
        }
        return instance;
    }
    static void deleteInstance()
    {
        if (instance != NULL)
        {
            delete instance;
        }
    }
	// ------------------------------------------------
    
    void setup();
    void update();
    
    int status;
    
    ofxOscReceiver receiver;
    
    WordsManager *wm;
    SoundManager *sm;
};

#endif /* defined(__TheBookOfVoices__CameraManager__) */
