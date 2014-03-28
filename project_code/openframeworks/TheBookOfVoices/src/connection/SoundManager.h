//
//  SoundManager.h
//  TheBookOfVoices
//
//  Created by masahidey on 2014/03/27.
//
//

#ifndef __TheBookOfVoices__SoundManager__
#define __TheBookOfVoices__SoundManager__

#include "ofMain.h"
#include "ofxOsc.h"

#include "config.h"

class SoundManager
{
private:
    static SoundManager* instance;
    SoundManager();
    ~SoundManager();
public:
    // singleton --------------------------------------
    static SoundManager* getInstance()
    {
        if (instance == NULL)
        {
            instance = new SoundManager();
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
    void send(string address);
    void send1i(string address, int p1);
    void send2i(string address, int p1, int p2);
    void send3i(string address, int p1, int p2, int p3);
    void sendmi(string address, vector<int> mp);
    void send1s(string address, string p1);
    void sendms(string address, vector<string> mp);

    ofxOscSender sender;
};

#endif /* defined(__TheBookOfVoices__SoundManager__) */
