//
//  VR.h
//  jestureCap
//
//  Created by Noah Tovares on 1/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxSimpleGuiToo.h"
#include "hand.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <vector>

class VVR{
    
public:
    VVR();
    
    int distance;
    
    void DisplayLoc(void);
    
    ofxCvContourFinder 	contourFinderVR;
};
