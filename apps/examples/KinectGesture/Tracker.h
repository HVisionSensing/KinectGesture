//
//  sphere.h
//  Lighting
//
//  Created by Noah Tovares on 1/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#ifndef _TEST_APP2
#define _TEST_APP2

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxSimpleGuiToo.h"

#include <GLUT/GLUT.h>

#include "hand.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "finger.h"

class Tracker : public ofBaseApp
{
private:
    int bc;
    int xx;
    int yy;
    int ll;
    signed int hc;
public:
    
    GLdouble size;
    Tracker();
    
    static Tracker* spheretest;
    
    void setup(void);
    void update(void);
    void checkDepthUpdated(void);
    void draw(void);
    void keyPressed(int key);
    void exit(void);
    void trackfinger(void);
    void drawfingertips(void);
    
    // Kinect
	ofxKinect kinect;
	int angle;
	
	ofxCvColorImage		colorImage;
	ofxCvGrayscaleImage		checkGrayImage;
	
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayThresh;
	ofxCvGrayscaleImage		grayThreshPrev;
	
	ofxCvContourFinder 	contourFinder;
	
	bool				debug;
	bool				showConfigUI;
	bool				mirror;
    // for image
	int 				nearThreshold;
	int					farThreshold;
	
	// for mouse controll
	int					displayWidth;
	int					displayHeight;
	
	// for detection
	int					detectCount;
	int					detectTwoHandsCount;
	bool				detectingHands;
	bool				detectingTwoHands;
	//vector<Hand *>		hands;
    
    // fonts
	ofTrueTypeFont		msgFont;
    
    ofxVec2f   v1,v2,aux1;
    ofxVec3f   v1D,vxv;
    ofxVec3f   v2D;
    
    //For hand and finger tracking
    int k, smk;
    float teta,lhd;
    int numfingers;
    int numtips;
    bool detectFingers;
    vector<finger> fingers;
    vector<finger> realfingers;
    vector<finger> tempfingers;
    finger tempfing;

        
};

#endif