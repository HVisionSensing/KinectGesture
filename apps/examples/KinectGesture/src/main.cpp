#include "ofMain.h"
#include "handJesture.h"
#include "ofAppGlutWindow.h"
#include "Tracker.h"



int main(int argc, char** argv){
    
    ofAppGlutWindow window1;
	
    ofSetupOpenGL(&window1, 1024,768, OF_WINDOW);
    
    ofRunApp(new Tracker());
    
    return 0;
}

