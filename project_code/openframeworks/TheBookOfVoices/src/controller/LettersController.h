//
//  LettersController.h
//  TheBookOfVoices
//
//  Created by masahidey on 2014/03/26.
//
//

#ifndef __TheBookOfVoices__LettersController__
#define __TheBookOfVoices__LettersController__

#include "ofMain.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxGui.h"
#include "Cubic.h"
#include "Linear.h"

#include "WordsManager.h"
#include "SoundManager.h"
#include "CameraManager.h"

#include "config.h"

class LettersController
{
public:
    void setup();
    void update();
    void draw();
    void drawFont();
    void addText(vector<string> words);
    void calcRect();
    void shufflePosition();

    ofxTrueTypeFontUC font;
    
    vector<vector<float> > wordWidth;
    vector<vector<vector<ofMesh> > > fontmesh;
    vector<vector<vector<ofRectangle> > > fontrect;
    vector<ofVec3f> position;
    vector<vector<vector<ofVec3f> > > origin, addPosition, rotAngle;
    vector<vector<vector<vector<ofVec3f> > > > noiseDir;
    vector<vector<vector<float> > > degree, eventStart, scale;
    vector<ofRectangle> totalRect;
    vector<vector<vector<int> > > param;
    vector<float> moveBias;
    vector<vector<vector<bool> > > growed;
    
    ofMesh sphere;
    vector<ofVec3f> sphereVertices, bombDir;
    int bombParam;
    float radius, size, bombBias, noiseBias, noiseParam;
    bool startFlg;
    
    ofEasyCam camera;
    
    ofShader shader, shader2, dof, gouraud;
    ofFbo fbo, fboDepth, fboBG;
    
    ofxFloatSlider distance;
    ofxFloatSlider cameraX, cameraY, cameraZ;
    ofxFloatSlider lookAtX, lookAtY, lookAtZ;
    ofxFloatSlider margin, lineSpace, blankWidth, limitWidth;
    ofxFloatSlider radiusBias;
    ofxFloatSlider fogStart, fogEnd;
    
    WordsManager *wm;
    SoundManager *sm;
    CameraManager *cm;
};

#endif /* defined(__TheBookOfVoices__LettersController__) */
