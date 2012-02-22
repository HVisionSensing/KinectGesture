#include "ofMain.h"
#include "handJesture.h"
#include "ofAppGlutWindow.h"
#include "Tracker.h"



int main(int argc, char** argv){
    
    ofAppGlutWindow window1;
	
    ofSetupOpenGL(&window1, 1024,768, OF_WINDOW);
    
    ofRunApp(new Tracker());
    /*
    Tracker test;
    test.setup();
    
    for (int i = 0; ; i++) {
        test.update();
        test.draw();
    }
    */
    //ofRunApp(new HandJesture());
    
    return 0;
}

