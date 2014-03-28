//
//  WordsManager.h
//  TheBookOfVoices
//
//  Created by masahidey on 2014/03/26.
//
//

#ifndef __TheBookOfVoices__WordsManager__
#define __TheBookOfVoices__WordsManager__

#include "ofMain.h"

#include "ofxLibwebsockets.h"
#include "ofxLibsndFileRecorder.h"

#include "SoundManager.h"

#include "config.h"

class WordsManager : public ofBaseApp
{
private:
    static WordsManager* instance;
    WordsManager();
    ~WordsManager();
public:
    // singleton --------------------------------------
    static WordsManager* getInstance()
    {
        if (instance == NULL)
        {
            instance = new WordsManager();
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
    void sendNames();

    // websocket
    ofxLibwebsockets::Server server;
    bool connected;
    
    // websocket methods
    void onConnect( ofxLibwebsockets::Event& args );
    void onOpen( ofxLibwebsockets::Event& args );
    void onClose( ofxLibwebsockets::Event& args );
    void onIdle( ofxLibwebsockets::Event& args );
    void onMessage( ofxLibwebsockets::Event& args );
    void onBroadcast( ofxLibwebsockets::Event& args );

    // recording audio
    bool recording;
    ofxLibsndFileRecorder audioRecorder;
    string name;
    string filename;
    float volume;
    bool bomb;
    
    // recording method
    void audioReceived(float * input, int bufferSize, int nChannels);

    // words
    vector<vector<string> > words;
    vector<string> names;
    
    SoundManager *sm;
};

#endif /* defined(__TheBookOfVoices__WordsManager__) */
