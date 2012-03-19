//
//  sphere.cpp
//  Lighting
//
//  Created by Noah Tovares on 1/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "tracker.h"
#include "hand.h"  

#include <iostream>
#include <fstream>

using namespace std;

Tracker::Tracker(){
    bc=0;
    hc=1;
}

void Tracker::setup(void){
    
    //Setup Kinect
	angle = -5;
	//kinect.init(true);  //shows infrared image
	kinect.init();
	//kinect.setVerbose(true);
	kinect.open();
	kinect.setCameraTiltAngle(angle);
    
    // Setup ofScreen
	//ofSetFullscreen(true);
	ofSetFrameRate(30);
	ofBackground(0, 0, 0);
	ofSetWindowShape(800, 600);
    
    // For images
	grayImage.allocate(kinect.width, kinect.height);
	checkGrayImage.allocate(kinect.width, kinect.height);
	grayThresh.allocate(kinect.width, kinect.height);
    
	// For hand detection
	nearThreshold = 5;
	farThreshold = 30;
	detectCount = 0;
	detectTwoHandsCount = 0;
    
	displayWidth = 1280;
	displayHeight = 800;
    
    // Fonts
	msgFont.loadFont("Courier New.ttf",14, true, true);
	msgFont.setLineHeight(20.0f);
    
    gui.setup();
	gui.config->gridSize.x = 300;
    
    gui.addTitle("KINECT SETTINGS");
	gui.addSlider("Tilt Angle", angle, -30, 30);
	gui.addToggle("Mirror Mode", mirror);
	gui.addTitle("DETECT RANGE");
    
    gui.addSlider("Near Distance", nearThreshold, 5, 20);
	gui.addSlider("Far Distance", farThreshold, 20, 60);
    
    gui.addTitle("MOUSE CONTROLL");
	gui.addSlider("Display Width", displayWidth, 600, 1980);
	gui.addSlider("Display height", displayHeight, 600, 1980);
    
    gui.setDefaultKeys(true);
    
	gui.loadFromXML();
}

void Tracker::update(void){
    kinect.update();
    
    checkDepthUpdated();
    
	grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
	grayThresh.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
    
	unsigned char * pix = grayImage.getPixels();
	unsigned char * grayThreshPix = grayThresh.getPixels();
	int numPixels = grayImage.getWidth() * grayImage.getHeight();
    
	int maxThreshold = 255 - nearThreshold;// default 250
	int minThreshold = 255 - farThreshold; // default 225
    
	int nearestDepth = 0;
	for(int i = 0; i < numPixels; i++){
		if (minThreshold < pix[i] && pix[i] < maxThreshold && pix[i] > nearestDepth) {
			nearestDepth = pix[i];
		}
	}
    
	for(int i = 0; i < numPixels; i++){
		//if( pix[i] < nearThreshold && pix[i] > farThreshold ){
		if(minThreshold < pix[i] 
           && pix[i] < nearestDepth+2 
           && pix[i] > nearestDepth - 10 ){
			//grayThreshPix[i] = floor((5 - (nearestDepth - pix[i]))/5.0f*255.0f);
			pix[i] = 255; // white
		}else{
			pix[i] = 0;
			//grayThreshPix[i] = 0;
		}
	}
    
	//update the cv image
	grayImage.flagImageChanged();
	//grayThresh.flagImageChanged();
	if (mirror) {
		grayImage.mirror(false, true);
	}
	//grayThresh.mirror(false, true);
    
    contourFinder.findContours(grayImage, 2000, (kinect.width*kinect.height)/4, 2, false);
    
	if (showConfigUI) {
		return;
	}
    
}

void Tracker::checkDepthUpdated(){
    if (ofGetFrameNum() % 150 == 0) {
		ofLog(OF_LOG_VERBOSE, "check depth updated");
        unsigned char * nextDepth = kinect.getDepthPixels();
        
        if (ofGetFrameNum() != 150) {
			ofLog(OF_LOG_VERBOSE, "Start compare depth pixels");
			unsigned char * currentDepthPixels = checkGrayImage.getPixels();
            
		    int pixNum = kinect.width * kinect.height;
            for (int i=0; i<pixNum; i++) {
                if (nextDepth[i] != currentDepthPixels[i]) {
                    break;
				}
				if (i > pixNum / 2) {
					ofLog(OF_LOG_ERROR, "Depth pixels could not be refreshed. Reset Kinect");
					kinect.close();
					kinect.open();
					kinect.setCameraTiltAngle(angle);
					break;
				}
			}                  
		}
		checkGrayImage.setFromPixels(nextDepth, kinect.width, kinect.height);
	}
}

// this is the main loop 
void Tracker::draw() {
    
    trackhand();
    
    trackfinger();
    
    std::ostringstream osstream,stream2, stream3, stream4, stream5;
    
    int x [contourFinder.nBlobs];
    int y [contourFinder.nBlobs];
    
    double length;
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        x[i] = contourFinder.blobs[i].centroid.x;
        osstream << "x";
        osstream << i;
        osstream << " = ";
        osstream << x[i];
        osstream << " ";
        //cout<<"x"<<i<<" = "<< x[i];
        y[i] = contourFinder.blobs[i].centroid.y;
        osstream << "y";
        osstream << i;
        osstream << " = ";
        osstream << y[i];
        osstream << " ";
        //cout<<"y"<<i<<" = "<< y[i];
    }
    
    if (contourFinder.nBlobs == 2){
        int dx = contourFinder.blobs[1].centroid.x-contourFinder.blobs[0].centroid.x;
        int dy = contourFinder.blobs[1].centroid.y-contourFinder.blobs[0].centroid.y;
        length = sqrt((dx*dx)+(dy*dy));
        stream2<< "length = ";
        stream2<< length;
        stream2<< " dx = ";
        stream2<< dx;
        stream2<< " dy = ";
        stream2<< dy;
    }
    
	ofSetColor(255, 255, 255);
    
    gui.draw();
    
    if (contourFinder.nBlobs == 2){
        stream3<<contourFinder.blobs[0].nPts;
        stream4<<contourFinder.blobs[1].nPts;
    }
    
    ofPopMatrix();
    
	ofSetColor(255, 255, 255);
    
    //stream5<<numfingers;
    int c = hands[0].numtips + hands[1].numtips;
    stream5<<c;
    
    std::string string_x = osstream.str();
    std::string string_y = stream2.str();
    std::string string_z = stream3.str();
    std::string string_q = stream4.str();
    std::string string_p = stream5.str();
    
    msgFont.drawString("# of Fingers " +string_p, 20, ofGetHeight()-100);
    msgFont.drawString("blob1 corners: " +string_z, 20, ofGetHeight()-80);
    msgFont.drawString("blob2 corners: " +string_q, 20, ofGetHeight()-60);
    msgFont.drawString("distance: " +string_y, 20, ofGetHeight()-40);
    msgFont.drawString("locations: " +string_x, 20, ofGetHeight()-20);
    
	ofNoFill();
    
};

//track hand, write location to txt file
void Tracker::trackhand() {
    
    // this code is for the two handed system
    hand1.open ("/Users/noahtovares/Desktop/KinectTxt/hand1.txt",ios::app);
    hand2.open ("/Users/noahtovares/Desktop/KinectTxt/hand2.txt",ios::app);
    
    hand1 << contourFinder.blobs[0].centroid.x << "," << contourFinder.blobs[0].centroid.y <<" ";
    hand2 << contourFinder.blobs[1].centroid.x << "," << contourFinder.blobs[1].centroid.y <<" ";
    
}

void Tracker::trackfinger(){
    
    fingers.clear();
    
    tempfingers.clear();
    
    ofPoint tempcenter;
    
    // loop through blobs, store hand centroid to be passed to hand class
    for (int j = 0; j < contourFinder.nBlobs; j++){
        vector<ofPoint> tempPnts;
        
        tempcenter.x = contourFinder.blobs[j].centroid.x;
        tempcenter.y = contourFinder.blobs[j].centroid.y;
        
        // loop through all hand points, create ofPoint, push to vector to be passed to hand class
        for (int i=0; i<contourFinder.blobs[j].nPts; i++){
            ofPoint tempPnt;
            tempPnt.x = contourFinder.blobs[j].pts[i].x;
            tempPnt.y = contourFinder.blobs[j].pts[i].y;
            tempPnt.z = kinect.getDistanceAt(tempPnt.x, tempPnt.y);
            
            tempPnts.push_back(tempPnt);            
            
        }
        // initiate finger tracking
        
        // char lists of file names for positions to be written to 
        char file1[] = "/Users/noahtovares/Desktop/KinectTxt/hand1.txt";
        char file2[] = "/Users/noahtovares/Desktop/KinectTxt/hand2.txt";
        
        if (j == 0){
            hands[j].shuffpnts(tempPnts, tempPnts.size()/2, tempcenter, file1); 
        }
        
        if(j == 1){
            hands[j].shuffpnts(tempPnts, tempPnts.size()/2, tempcenter, file2);
        }
        
        tempPnts.clear();
    }
    
}

void Tracker::keyPressed (int key) {
	ofLog(OF_LOG_VERBOSE, ofToString(key));
    
	switch (key)
	{
        case 's':
            gui.show();
            break;
        case 'h':
            gui.hide();
            break;
		case '>':
            
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
		case '<':		
		case ',':		
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
            
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
		case '-':		
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
        case 'd':
            debug ? debug=false : debug=true;
            break;
		case 'r':
			// reboot kinect
			kinect.close();
			kinect.open();
			kinect.setCameraTiltAngle(angle);
            break;
            
		case ' ':
			showConfigUI = !showConfigUI;
			if (showConfigUI) {
				ofSetWindowShape(800, 600);
			} else {
				ofSetWindowShape(400, 300);
				kinect.setCameraTiltAngle(angle);
			}
			break;			
            
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	}
    
}

void Tracker::exit(){
	kinect.close();
	ofLog(OF_LOG_NOTICE, "Close Kinect and exit");
}