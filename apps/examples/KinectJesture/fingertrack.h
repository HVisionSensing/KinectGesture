//
//  fingertrack.h
//  jestureCap
//
//  Created by Noah Tovares on 1/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//



#include <iostream>
#include "sphere.h"
#include "Math.h"
#include <Carbon/Carbon.h>
#include "MacWindows.h"
#include <GLUT/GLUT.h>

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "hand.h"



class fingertracker{
private:
    bool detectFingers;
    ofxVec2f   v1,v2,aux1;
    ofxVec3f   v1D,vxv;
    ofxVec3f   v2D;
    vector  <ofxPoint2f>      ppico;
    vector  <ofxPoint2f>      smppico;
    vector   <float>            kpointcurv;
    vector   <float>            smkpointcurv;
    vector   <bool>            bfingerRuns;
    vector  <ofxPoint2f>      lhand;
    vector  <ofxPoint2f>      rhand;
    int k, smk;
    float teta,lhd;
    int numfingers;

public:
    fingertracker();
    
    void detectfinger(void);
};



