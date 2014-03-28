//
//  SoundManager.cpp
//  DevArtStudy
//
//  Created by masahidey on 2014/03/27.
//
//

#include "SoundManager.h"

SoundManager* SoundManager::instance = NULL;

#pragma mark -
#pragma mark constructor & destructor - private

SoundManager::SoundManager()
{
    sender.setup(DA::SOUND_HOST,DA::SOUND_PORT);
}

SoundManager::~SoundManager()
{
    
}

#pragma mark -
#pragma mark public methods

void SoundManager::setup()
{
    
}

void SoundManager::send(string address)
{
    ofxOscMessage m;
    m.setAddress(address);
    sender.sendMessage(m);
    
    ofLog(OF_LOG_NOTICE, "Sound Send : " + address);
}

void SoundManager::send1i(string address, int p1)
{
    ofxOscMessage m;
    m.setAddress(address);
    m.addIntArg(p1);
    sender.sendMessage(m);
    
    ofLog(OF_LOG_NOTICE, "Sound Send : " + address + " " + ofToString(p1));
}

void SoundManager::send2i(string address, int p1, int p2)
{
    ofxOscMessage m;
    m.setAddress(address);
    m.addIntArg(p1);
    m.addIntArg(p2);
    sender.sendMessage(m);

    ofLog(OF_LOG_NOTICE, "Sound Send : " + address + " " + ofToString(p1) + " " + ofToString(p2));
}

void SoundManager::send3i(string address, int p1, int p2, int p3)
{
    ofxOscMessage m;
    m.setAddress(address);
    m.addIntArg(p1);
    m.addIntArg(p2);
    m.addIntArg(p3);
    sender.sendMessage(m);

    ofLog(OF_LOG_NOTICE, "Sound Send : " + address + " " + ofToString(p1) + " " + ofToString(p2) + " " + ofToString(p3));
}

void SoundManager::sendmi(string address, vector<int> mp)
{
    ofxOscMessage m;
    m.setAddress(address);
    for (int i=0; i<mp.size(); i++) {
        m.addIntArg(mp[i]);
    }
    sender.sendMessage(m);
}
void SoundManager::send1s(string address, string p1)
{
    ofxOscMessage m;
    m.setAddress(address);
    m.addStringArg(p1);
    sender.sendMessage(m);
    
    ofLog(OF_LOG_NOTICE, "Sound Send : " + address + " " + p1);
}

void SoundManager::sendms(string address, vector<string> mp)
{
    ofxOscMessage m;
    m.setAddress(address);
    for (int i=0; i<mp.size(); i++) {
        m.addStringArg(mp[i]);
    }
    sender.sendMessage(m);
}