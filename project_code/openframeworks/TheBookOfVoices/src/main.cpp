#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
    ofAppGlutWindow window;
    window.setGlutDisplayString("rgba double samples>=4");
    window.setGlutDisplayString("rgba double depth samples>=4");
    window.setGlutDisplayString("rgba double depth alpha samples>=4");
	ofSetupOpenGL(&window, 1920, 1080, OF_WINDOW);
	ofRunApp(new testApp());
}
