//
//  LettersController.cpp
//  TheBookOfVoices
//
//  Created by masahidey on 2014/03/26.
//
//

#include "LettersController.h"

void LettersController::setup()
{
    ofDisableArbTex();
    
    font.loadFont("/Library/Fonts/KozGoPro-Medium.otf", DA::SIZE, true, true);

    dof.load("shaders/dof.vs", "shaders/dof.fs");
	shader.setGeometryInputType(GL_TRIANGLES);
	shader.setGeometryOutputType(GL_TRIANGLES);
	shader.setGeometryOutputCount(3);
	shader.load("shaders/break.vs", "shaders/break.fs", "shaders/break.gs");
	shader2.setGeometryInputType(GL_TRIANGLES);
	shader2.setGeometryOutputType(GL_TRIANGLES);
	shader2.setGeometryOutputCount(3);
    shader2.load("shaders/break2.vs", "shaders/break2.fs", "shaders/break2.gs");
    
    gouraud.load("shaders/gouraud.vs", "shaders/gouraud.fs");
    
    camera.setupPerspective( true, 30.0f, 0.1f, 2000.0f );
    
    wm = WordsManager::getInstance();
    sm = SoundManager::getInstance();
    cm = CameraManager::getInstance();
    cm->status = 1;
    
    ofMesh tmpSphere;
    tmpSphere = tmpSphere.sphere(1.0f, 48, OF_PRIMITIVE_TRIANGLES);
    for (int i=0; i<tmpSphere.getNumIndices(); i++) {
        int idx = tmpSphere.getIndex(i);
        ofVec3f vec = tmpSphere.getVertex(idx);
        sphereVertices.push_back(vec);
        sphere.addVertex(vec);
        sphere.addNormal(vec);
        sphere.addColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
        
        bombDir.push_back(vec + ofVec3f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.5)));
    }

    bombParam = 0;
    bombBias = 0.0f;
    noiseParam = 0.0f;
    noiseBias = 1.0f;
    size = 1.0f;
    
    startFlg = true;
}

void LettersController::update()
{
    camera.setGlobalPosition( -cameraX, cameraY, cameraZ );
    camera.lookAt( ofVec3f( -lookAtX, lookAtY, lookAtZ ) );
    
    if (wm->words.size()>0) {
        for (int i =0; i<wm->words.size(); i++) {
            addText(wm->words[i]);
        }
        wm->words.clear();
    }

    calcRect();

    if (cm->status == 1 || cm->status == 2) {
        startFlg = false;
        bool finished = true;
        for (int i=0; i<fontmesh.size(); i++) {
            bool allGrowed = true;;
            for (int j=0; j<fontmesh[i].size(); j++) {
                for (int k=0; k<fontmesh[i][j].size(); k++) {
                    float addDegree = 0.75f;
                    
                    if(param[i][j][k]<=120)addDegree = 0.05f + Cubic::easeIn(param[i][j][k], 0.0f, 0.5f, 120);
                    degree[i][j][k] += addDegree;
                    addPosition[i][j][k].y = Cubic::easeIn(param[i][j][k], 0, 1.0f, 750)*distance;
                    
                    if(!growed[i][j][k]){ finished = false; continue; }
                    if(param[i][j][k]<750){
                        param[i][j][k]++;
                        finished = false;
                    }
                }
            }
        }
        if (cm->status == 1 && finished) {
            cm->status = 2;
            shufflePosition();
        }
    }else if(cm->status == 3) {
        bool finished = true;
        for (int i=0; i<fontmesh.size(); i++) {
            for (int j=0; j<fontmesh[i].size(); j++) {
                for (int k=0; k<fontmesh[i][j].size(); k++) {
                    float addDegree = 0.75f;
                    if(param[i][j][k]<=120)addDegree = 0.25f + Cubic::easeIn(param[i][j][k], 0.0f, 0.5f, 120);
                    degree[i][j][k] -= addDegree;
                    addPosition[i][j][k].y = Cubic::easeIn(param[i][j][k], 0, 1.0f, 750)*distance;

                    if(!growed[i][j][k])continue;
                    if(param[i][j][k]>0){
                        param[i][j][k]--;
                        finished = false;
                    }
                }
            }
        }
        if (!startFlg && finished) {
            startFlg = true;
            sm->send("/stop");
        }
    }
    
    if (wm->bomb) {
        int idx = fontmesh.size() - 1;
        int stringNum = 0;
        for (int j=0; j<fontmesh[idx].size(); j++) {
            for (int k=0; k<fontmesh[idx][j].size(); k++) {
                stringNum++;
            }
        }
        position[idx] = ofVec3f(0.0f, -30.0f, 320.0f);
        
        int e1 = 60; // sphere
        int e2 = 480; // bomb
        int e3 = 480; // scale
        float e4 = 20.0f; // time lag
        int e5 = 480; // move
        if (bombParam>=0 && bombParam<e1) {
            bombBias = 0.0f;
            if (wm->volume*radiusBias<0.5f) {
                radius = Linear::easeNone(bombParam, wm->volume*radiusBias, 0.5f, e1);
                noiseBias = 1.0f - Linear::easeNone(bombParam, 0.0f, 1.0f, e1);
            }else{
                bombParam = e1;
                noiseBias = 0.0f;
            }
        }else if (bombParam>=e1 && bombParam<e1+e2) {
            bombBias = Linear::easeNone(bombParam-e1, 0.0f, 1.0f, e2)*300;
            size = 1.0f - Linear::easeNone(bombParam-e1, 0, 1.0f, e2);
        }
        if (bombParam>=e1 && bombParam<e1+e3+stringNum*e4) {
            int cnt = 0;
            for (int j=0; j<fontmesh[idx].size(); j++) {
                for (int k=0; k<fontmesh[idx][j].size(); k++) {
                    if (bombParam-e1 > cnt*e4 && bombParam-e1 <= cnt*e4+e3) {
                        scale[idx][j][k] = Cubic::easeInOut(bombParam-e1-cnt*e4, 0, 0.8f, e3);
                        if(bombParam-e1 == cnt*e4+e3)growed[idx][j][k] = true;
                    }
                    cnt++;
                }
            }
        }else if (bombParam>=e1+e3+stringNum*e4 && bombParam<e1+e3+e5+stringNum*e4) {
            moveBias[idx] = 1.0 - Linear::easeInOut(bombParam-(e1+e3+stringNum*e4), 0, 1.0f, e5);
        }
        
        int limit = (e1+e3+stringNum*e4 > e1+e2) ? e1+e3+stringNum*e4 : e1+e2;
        if(bombParam>=limit){
            wm->bomb = false;
            radius = 0.0f;
            bombParam = 0;
            bombBias = 0.0f;
            noiseBias = 1.0f;
            size = 1.0f;
        }
        bombParam++;
    }else{
        radius = wm->volume*radiusBias;
        for (int i=0; i<fontmesh.size(); i++) {
            moveBias[i] = 0.0f;
            for (int j=0; j<fontmesh[i].size(); j++) {
                for (int k=0; k<fontmesh[i][j].size(); k++) {
                    scale[i][j][k] = 0.8f;
                    growed[i][j][k] = true;
                }
            }
        }
    }
    
    noiseParam += noiseBias;
}

void LettersController::draw()
{
    ofEnableDepthTest();
    ofEnableLighting();
    ofDisableAlphaBlending();
    ofSetSmoothLighting(true);
    
    camera.begin();
    
    drawFont();
    
    shader2.begin();
    shader2.setUniform1f("noiseParam", noiseParam);
    shader2.setUniform1f("noiseBias", noiseBias);
    shader2.setUniform1f("radius", radius);
    shader2.setUniform1f("bias", bombBias);
    shader2.setUniform1f("size", size);
    int bombDirAttLoc = shader2.getAttributeLocation("bombDir");
    glVertexAttribPointer(bombDirAttLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), &bombDir[0]);
    glBindAttribLocation(shader2.getProgram(), bombDirAttLoc, "bombDir");
    glEnableVertexAttribArray(bombDirAttLoc);

    sphere.draw();

    shader2.end();
    
    camera.end();
    
    ofDisableDepthTest();
    ofDisableLighting();
    ofEnableAlphaBlending();
}

void LettersController::drawFont()
{
    for (int i=0; i<fontmesh.size(); i++) {
        for (int j=0; j<fontmesh[i].size(); j++) {
            for (int k=0; k<fontmesh[i][j].size(); k++) {
                ofVec3f base = ofVec3f(fontrect[i][j][k].x+fontrect[i][j][k].width/2, fontrect[i][j][k].y+fontrect[i][j][k].height/2, 0.0f) + ofVec3f(-totalRect[i].x-totalRect[i].width/2, -totalRect[i].height/2, 0.0f) + origin[i][j][k];
                //ofVec3f base2 = base + ofVec3f(0, 0, 320)*moveBias[i] + position[i]*(1.0f-moveBias[i]);
                ofVec3f base2 = base + position[i];
                ofVec3f pos = base2 + addPosition[i][j][k];
                
                shader.begin();
                shader.setUniform1f("base", base2.y);
                shader.setUniform1f("start", eventStart[i][j][k]);
                shader.setUniform1f("scale", scale[i][j][k]);
                shader.setUniform1f("fogStart", fogStart);
                shader.setUniform1f("fogEnd", fogEnd);
                int noiseDirAttLoc = shader.getAttributeLocation("noiseDir");
                glVertexAttribPointer(noiseDirAttLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ofVec3f), &noiseDir[i][j][k][0]);
                glBindAttribLocation(shader.getProgram(), noiseDirAttLoc, "noiseDir");
                glEnableVertexAttribArray(noiseDirAttLoc);
                
                ofPushMatrix();{
                    float limit = 100.0f;
                    if (pos.y <= eventStart[i][j][k]) {
                        ofTranslate(pos-base);
                    }else if (pos.y > eventStart[i][j][k] && pos.y <= limit + eventStart[i][j][k]){
                        ofTranslate(ofVec3f(0.0f, pos.y - base.y) + ofVec3f(pos.x - base.x, 0.0f, pos.z - base.z)*(1.0f - Linear::easeOut(pos.y - eventStart[i][j][k], 0, 1.0f, limit)));
                    }else if (pos.y > limit + eventStart[i][j][k]){
                        ofTranslate(ofVec3f(0.0f, pos.y - base.y));
                    }
                    ofPushMatrix();{
                        ofScale(scale[i][j][k], scale[i][j][k], scale[i][j][k]);
                        if (pos.y <= eventStart[i][j][k]) {
                            ofTranslate(base);
                        }else if (pos.y > eventStart[i][j][k] && pos.y <= limit + eventStart[i][j][k]){
                            ofTranslate(ofVec3f(0.0f, base.y) + ofVec3f(base.x, 0.0f, base.z)*(1.0f - Linear::easeOut(pos.y - eventStart[i][j][k], 0, 1.0f, limit)));
                        }else if (pos.y > limit + eventStart[i][j][k]){
                            ofTranslate(ofVec3f(0.0f, base.y));
                        }
                        ofPushMatrix();{
                            ofRotate(degree[i][j][k], rotAngle[i][j][k].x, rotAngle[i][j][k].y, rotAngle[i][j][k].z);
                            ofPushMatrix();{
                                fontmesh[i][j][k].draw();
                            }ofPopMatrix();
                        }ofPopMatrix();
                    }ofPopMatrix();
                }ofPopMatrix();
                
                shader.end();
            }
         }
    }
}

void LettersController::addText(vector<string> words)
{
    vector<vector<string> > targets;
    for (int i=0; i<words.size(); i++) {
        cout << words[i] << endl;
        
        vector<string> tmpTarget;

        int char_size;
        unsigned char lead;
        for (int j=0; j<words[i].length(); j+=char_size) {
            lead = words[i][j];
            
            if (lead < 0x80) {
                char_size = 1;
            } else if (lead < 0xE0) {
                char_size = 2;
            } else if (lead < 0xF0) {
                char_size = 3;
            } else {
                char_size = 4;
            }
            
            string target;
            target = words[i].substr(j, char_size);
            tmpTarget.push_back(target);
        }
        targets.push_back(tmpTarget);
    }
    
    int vn = 0;
    vector<vector<ofMesh> > fontmeshArr;
    vector<vector<ofRectangle> > fontrectArr;
    vector<vector<ofVec3f> > originArr, addPositionArr, rotAngleArr;
    vector<vector<vector<ofVec3f> > > noiseDirArr;
    vector<vector<float> > degreeArr, eventStartArr, scaleArr;
    vector<float> wordWidhtArr;
    vector<vector<bool> > growedArr;
    vector<vector<int> > paramArr;
    for (int i=0; i<targets.size(); i++) {
        vector<ofMesh> fontmeshArr2;
        vector<ofRectangle> fontrectArr2;
        vector<ofVec3f> originArr2, addPositionArr2, rotAngleArr2;
        vector<vector<ofVec3f> > noiseDirArr2;
        vector<float> degreeArr2, eventStartArr2, scaleArr2;
        vector<bool> growedArr2;
        vector<int> paramArr2;
        wordWidhtArr.push_back(0.0f);
        for (int j=0; j<targets[i].size(); j++) {
            vector<ofPath> fontpath = font.getStringAsPoints(targets[i][j]);
            if(fontpath.size()==0)continue;
            
            ofMesh tmp;
            tmp.setMode(OF_PRIMITIVE_TRIANGLES);
            ofMesh mesh = fontpath[0].getTessellation();
            vector<ofPolyline> pl = fontpath[0].getOutline();
            
            ofRectangle rect = font.getStringBoundingBox(targets[i][j], 0, 0);
            
            for (int k=mesh.getNumIndices()-1; k>=0; k--) {
                tmp.addVertex(mesh.getVertex(mesh.getIndex(k))+ofVec3f(-rect.x-rect.width/2, -rect.y-rect.height/2, DA::WIDTH));
                tmp.addColor(ofFloatColor(1.0,1.0,1.0));
                tmp.addNormal(ofVec3f(0.0, 0.0, 1.0));
            }
            
            for (int k=0; k<mesh.getNumIndices(); k++) {
                tmp.addVertex(mesh.getVertex(mesh.getIndex(k))+ofVec3f(-rect.x-rect.width/2, -rect.y-rect.height/2, -DA::WIDTH));
                tmp.addColor(ofFloatColor(1.0,1.0,1.0));
                tmp.addNormal(ofVec3f(0.0, 0.0, -1.0));
            }
            
            for (int k=0; k<pl.size(); k++) {
                vector<ofPoint> pts = pl[k].getVertices();
                for (int l=0; l<pts.size(); l++) {
                    int idx0 = (pts.size() + l - 1)%pts.size();
                    int idx1 = (pts.size() + l + 0)%pts.size();
                    int idx2 = (pts.size() + l + 1)%pts.size();
                    int idx3 = (pts.size() + l + 2)%pts.size();
                    ofVec3f pt0 = pts[idx0]+ofVec3f(-rect.x-rect.width/2, -rect.y-rect.height/2, -DA::WIDTH);
                    ofVec3f pt1 = pts[idx0]+ofVec3f(-rect.x-rect.width/2, -rect.y-rect.height/2, DA::WIDTH);
                    ofVec3f pt2 = pts[idx1]+ofVec3f(-rect.x-rect.width/2, -rect.y-rect.height/2, -DA::WIDTH);
                    ofVec3f pt3 = pts[idx1]+ofVec3f(-rect.x-rect.width/2, -rect.y-rect.height/2, DA::WIDTH);
                    ofVec3f pt4 = pts[idx2]+ofVec3f(-rect.x-rect.width/2, -rect.y-rect.height/2, -DA::WIDTH);
                    ofVec3f pt5 = pts[idx2]+ofVec3f(-rect.x-rect.width/2, -rect.y-rect.height/2, DA::WIDTH);
                    ofVec3f pt6 = pts[idx3]+ofVec3f(-rect.x-rect.width/2, -rect.y-rect.height/2, -DA::WIDTH);
                    ofVec3f pt7 = pts[idx3]+ofVec3f(-rect.x-rect.width/2, -rect.y-rect.height/2, DA::WIDTH);
                    
                    ofVec3f gaiseki04 = ofVec3f(
                                                (pt1.y-pt0.y)*(pt4.z-pt0.z)-(pt1.z-pt0.z)*(pt4.y-pt0.y),
                                                (pt1.z-pt0.z)*(pt4.x-pt0.x)-(pt1.x-pt0.x)*(pt4.z-pt0.z),
                                                (pt1.x-pt0.x)*(pt4.y-pt0.y)-(pt1.y-pt0.y)*(pt4.x-pt0.x)
                                                );
                    ofVec3f gaiseki73 = ofVec3f(
                                                (pt6.y-pt7.y)*(pt3.z-pt7.z)-(pt6.z-pt7.z)*(pt3.y-pt7.y),
                                                (pt6.z-pt7.z)*(pt3.x-pt7.x)-(pt6.x-pt7.x)*(pt3.z-pt7.z),
                                                (pt6.x-pt7.x)*(pt3.y-pt7.y)-(pt6.y-pt7.y)*(pt3.x-pt7.x)
                                                );
                    ofVec3f gaiseki24 = ofVec3f(
                                                (pt3.y-pt2.y)*(pt4.z-pt2.z)-(pt3.z-pt2.z)*(pt4.y-pt2.y),
                                                (pt3.z-pt2.z)*(pt4.x-pt2.x)-(pt3.x-pt2.x)*(pt4.z-pt2.z),
                                                (pt3.x-pt2.x)*(pt4.y-pt2.y)-(pt3.y-pt2.y)*(pt4.x-pt2.x)
                                                );
                    ofVec3f gaiseki53 = ofVec3f(
                                                (pt4.y-pt5.y)*(pt3.z-pt5.z)-(pt4.z-pt5.z)*(pt3.y-pt5.y),
                                                (pt4.z-pt5.z)*(pt3.x-pt5.x)-(pt4.x-pt5.x)*(pt3.z-pt5.z),
                                                (pt4.x-pt5.x)*(pt3.y-pt5.y)-(pt4.y-pt5.y)*(pt3.x-pt5.x)
                                                );
                    
                    float rot0 = acos(gaiseki04.x*gaiseki24.x + gaiseki04.y*gaiseki24.y + gaiseki04.z*gaiseki24.z) * 180.0f / PI;
                    float rot1 = acos(gaiseki73.x*gaiseki53.x + gaiseki73.y*gaiseki53.y + gaiseki73.z*gaiseki53.z) * 180.0f / PI;
                    ofVec3f gaiseki0 = (rot0 < 45) ? gaiseki04 : gaiseki24;
                    ofVec3f gaiseki1 = (rot1 < 45) ? gaiseki73 : gaiseki53;
                    
                    tmp.addVertex(pt2);
                    tmp.addColor(ofFloatColor(1.0,1.0,1.0));
                    tmp.addNormal(gaiseki0);
                    tmp.addVertex(pt3);
                    tmp.addColor(ofFloatColor(1.0,1.0,1.0));
                    tmp.addNormal(gaiseki0);
                    tmp.addVertex(pt4);
                    tmp.addColor(ofFloatColor(1.0,1.0,1.0));
                    tmp.addNormal(gaiseki1);
                    
                    tmp.addVertex(pt5);
                    tmp.addColor(ofFloatColor(1.0,1.0,1.0));
                    tmp.addNormal(gaiseki1);
                    tmp.addVertex(pt4);
                    tmp.addColor(ofFloatColor(1.0,1.0,1.0));
                    tmp.addNormal(gaiseki1);
                    tmp.addVertex(pt3);
                    tmp.addColor(ofFloatColor(1.0,1.0,1.0));
                    tmp.addNormal(gaiseki0);
                }
            }
            fontmeshArr2.push_back(tmp);
            fontrectArr2.push_back(rect);
            originArr2.push_back(ofVec3f());
            addPositionArr2.push_back(ofVec3f(0, 0, 0));
            vector<ofVec3f> noiseDirArr3;
            for (int k=0; k<tmp.getNumVertices(); k++) {
                noiseDirArr3.push_back(ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)));
            }
            noiseDirArr2.push_back(noiseDirArr3);
            rotAngleArr2.push_back(ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)));
            degreeArr2.push_back(0.0f);
            eventStartArr2.push_back(ofRandom(25, 50));
            scaleArr2.push_back(0.0f);
            growedArr2.push_back(false);
            paramArr2.push_back(0);
            vn += tmp.getNumVertices();
        }
        fontmeshArr.push_back(fontmeshArr2);
        fontrectArr.push_back(fontrectArr2);
        originArr.push_back(originArr2);
        addPositionArr.push_back(addPositionArr2);
        noiseDirArr.push_back(noiseDirArr2);
        rotAngleArr.push_back(rotAngleArr2);
        degreeArr.push_back(degreeArr2);
        eventStartArr.push_back(eventStartArr2);
        scaleArr.push_back(scaleArr2);
        growedArr.push_back(growedArr2);
        paramArr.push_back(paramArr2);
    }
    fontmesh.push_back(fontmeshArr);
    fontrect.push_back(fontrectArr);
    origin.push_back(originArr);
    position.push_back(ofVec3f((int)ofRandom(-10, 10)*10, (int)ofRandom(-10, 10)*10, (int)ofRandom(-30, 30)*10));
    addPosition.push_back(addPositionArr);
    noiseDir.push_back(noiseDirArr);
    rotAngle.push_back(rotAngleArr);
    degree.push_back(degreeArr);
    eventStart.push_back(eventStartArr);
    wordWidth.push_back(wordWidhtArr);
    scale.push_back(scaleArr);
    totalRect.push_back(ofRectangle());
    moveBias.push_back(1.0f);
    param.push_back(paramArr);
    growed.push_back(growedArr);
    cout << vn << endl;
}

void LettersController::calcRect()
{
    // Origin Position
    for (int i=0; i<fontrect.size(); i++) {
        for (int j=0; j<fontrect[i].size(); j++) {
            wordWidth[i][j] = 0.0f;
            for (int k=0; k<fontrect[i][j].size(); k++) {
                if(j==0)wordWidth[i][j] += fontrect[i][j][k].x + fontrect[i][j][k].width;
                else wordWidth[i][j] += fontrect[i][j][k].x + fontrect[i][j][k].width + margin;
            }
        }
    }
    
    for (int i=0; i<fontrect.size(); i++) {
        float totalWidth = 0.0f;
        ofVec3f current = ofVec3f();
        int idx = 0;
        for (int j=0; j<fontrect[i].size(); j++) {
            
//            if(totalWidth==0.0f){ totalWidth += wordWidth[i][j]; }
//            else totalWidth += wordWidth[i][j] + blankWidth;
//            
//            if(i > 0 && totalWidth > limitWidth){
//                current = ofVec3f(0, current.y+lineSpace, 0);
//                totalWidth = 0.0f;
//            }
 
            float radius = 15.0f;
            float bias = 0.005f;
            for (int k=0; k<fontrect[i][j].size(); k++) {

                if(current.x > limitWidth){
                    current = ofVec3f(0, current.y+lineSpace, 0);
                    totalWidth = 0.0f;
                }
                
                float ns1 = ofSignedNoise(current.x*bias, current.y*bias, current.z*bias, 100.0f*i+100.0f*j);
                float ns2 = ofSignedNoise(current.x*bias, current.y*bias, current.z*bias, 200.0f*i+200.0f*j);
                float ns3 = ofSignedNoise(current.x*bias, current.y*bias, current.z*bias, 300.0f*i+300.0f*j);
                origin[i][j][k] = current + ofVec3f(ns1,ns2,ns3)*radius;
                rotAngle[i][j][k] = ofVec3f(ns1, ns2, ns3).normalize();
                
                if (j==0 && k==0) {
                    totalRect[i].x = fontrect[i][j][k].x;
                    totalRect[i].y = fontrect[i][j][k].y;
                    totalRect[i].width = fontrect[i][j][k].width;
                    totalRect[i].height = fontrect[i][j][k].height;
                }else{
                    if(totalRect[i].x > fontrect[i][j][k].x)totalRect[i].x = fontrect[i][j][k].x;
                    if(totalRect[i].y > fontrect[i][j][k].y+current.y)totalRect[i].y = fontrect[i][j][k].y + current.y;
                    if((current.x + fontrect[i][j][k].x + fontrect[i][j][k].width) - totalRect[i].x > totalRect[i].width)totalRect[i].width = (current.x + fontrect[i][j][k].x + fontrect[i][j][k].width) - totalRect[i].x;
                    if((current.y + fontrect[i][j][k].y + fontrect[i][j][k].height) - totalRect[i].y > totalRect[i].height)totalRect[i].height = (current.y + fontrect[i][j][k].y + fontrect[i][j][k].height) - totalRect[i].y;
                }
                
                current.x += fontrect[i][j][k].x + fontrect[i][j][k].width + margin;
            }
            current.x += blankWidth;
        }
    }
}

void LettersController::shufflePosition()
{
    for (int i=0; i<position.size(); i++) {
        position[i] = ofVec3f((int)ofRandom(-10, 10)*10, (int)ofRandom(-10, 10)*10, (int)ofRandom(-30, 30)*10);
    }
}