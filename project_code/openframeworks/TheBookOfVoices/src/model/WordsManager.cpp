//
//  WordsManager.cpp
//  TheBookOfVoices
//
//  Created by masahidey on 2014/03/26.
//
//

#include "WordsManager.h"

#define NUM_CHANNELS 1
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 1024

WordsManager* WordsManager::instance = NULL;

#pragma mark -
#pragma mark constructor & destructor - private

WordsManager::WordsManager()
{
}

WordsManager::~WordsManager()
{
}

#pragma mark -
#pragma mark public methods

void WordsManager::setup()
{
    ofHttpResponse res = ofLoadURL("http://devart.localhost/words/list.xml");
    ofXml xml;
    xml.loadFromBuffer(res.data);
    for (int i=0; i<xml.getNumChildren(); i++) {
        string item = xml.getValue("item["+ofToString(i)+"]/word");
        vector<string> tmp = ofSplitString(item, " ");
        
        words.push_back(tmp);
        names.push_back(xml.getValue("item["+ofToString(i)+"]/name"));
    }
    
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = DA::WS_PORT;
    options.bUseSSL = false;
    connected = server.setup( options );
    
    server.addListener(this);

	recording=false;
    volume = 0.0f;
    bomb = false;
    
	ofSoundStreamSetup(0, NUM_CHANNELS, this, SAMPLE_RATE, BUFFER_SIZE, 4);
    
    sm = SoundManager::getInstance();
}

//--------------------------------------------------------------
void WordsManager::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void WordsManager::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
}

//--------------------------------------------------------------
void WordsManager::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
}

//--------------------------------------------------------------
void WordsManager::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void WordsManager::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
    
    vector<string> mesArr = ofSplitString(args.message, ":");
    if(mesArr[0]=="[[start]]"&& !recording){
		cout<<"start recording\n";
        name = mesArr[1];
		filename="sounds/"+name+".wav";
        
		cout << filename<<"----\n";
		audioRecorder.setup(filename);
		audioRecorder.setFormat(SF_FORMAT_WAV | SF_FORMAT_PCM_16);
		recording=true;
	}else if(recording){
		cout<<"stop recording\n";
		recording=false;
        
        bool result = false;
        if (args.message!="[[noresult]]") {
            string word = args.message;
            cout << word << endl;
            
            int start = 0;
            for (int i=0; i<word.length(); i++) {
                if(word[i]==' ')start++;
                else break;
            }
            int end = word.length() - 1;
            for (int i=word.length()-1; i>=0; i--) {
                if(word[i]==' ')end--;
                else break;
            }
            
            float *buffer = new float[44100];
            for (int i=0; i<44100; i++) {
                buffer[i] = 0.0f;
            }
            audioRecorder.addSamples(buffer, 44100);
            audioRecorder.finalize();
            audioRecorder.close();
            
            if (!bomb) {
                string mes = args.message.substr(start, end-start+1);
                words.push_back(ofSplitString(mes, " "));
                names.push_back(name);
                sm->send1s("/realtime", name+".wav");
                bomb = true;
                result = true;
            }
        }else{
            cout << "[[noresult]]" << endl;
            audioRecorder.close();
            ofFile file;
            if (file.doesFileExist(filename)) {
                file.removeFile(filename);
            }
        }
        
        if (result) {
            server.send("ok");
        }else{
            server.send("nothing");
        }
        cout << "send restart" << endl;
    }
}

//--------------------------------------------------------------
void WordsManager::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"on Broadcast"<<endl;
}

//--------------------------------------------------------------
void WordsManager::audioReceived(float * input, int bufferSize, int nChannels){
	if(recording)
		audioRecorder.addSamples(input,bufferSize*nChannels);

    if (bomb)return;
    
	float curVol = 0.0;
    int numCounted = 0;
    
	for (int i = 0; i < bufferSize; i++){
		float left = input[i]*0.5;
        
		curVol += left * left;
		numCounted++;
	}
	
	curVol /= (float)numCounted;
    curVol = sqrt( curVol );
	
	volume *= 0.93;
	volume += 0.07 * curVol;
}

void WordsManager::sendNames()
{
    vector<string> data;
    for (int i=names.size()-1; i>=0; i--) {
        data.push_back(names[i]+".wav");
        if(data.size()>=5)break;
    }
    
    sm->sendms("/stock", data);
}