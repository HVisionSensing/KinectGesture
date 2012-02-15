//
//  sphere.cpp
//  Lighting
//
//  Created by Noah Tovares on 1/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "tracker.h"
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
#include "finger.h"


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

void Tracker::drawfingertips(){
    /*
     finger tempfing;
     
     double xave = 0;
     double yave = 0;
     int h = 0;
     int b = 0;
     numtips=0;
     
     int tester = 0;
     
     //ALL FINGERS HAVE BEEN DETERMINED
     //NOW DETERMINING IF FINGERS ARE CLOSE
     //GO THROUGH ALL THE FINGERS
     
     for (int k = h; k<fingers.size(); k++) {
     
     //DETERMINE THE DISTANCE BETWEEN FINGER THE THE NEXT FINGER
     
     int dx = fingers[k].xloc - fingers[k+1].xloc;
     int dy = fingers[k].yloc - fingers[k+1].yloc;
     int ll = sqrt((dx*dx)+(dy*dy)); 
     
     //OUTPUTS FINGER LOCATIONS - KEEP
     //cout<<"finger ";
     //cout<<k;
     //cout<<"\nx loc = ";
     //cout<<"\n";
     //cout<<fingers[k].xloc;
     //cout<<"\n";
     //cout<<"\ny loc = ";
     //cout<<fingers[k].yloc;
     //cout<<"\n";
     
     
     //IF FINGERS ARE CLOSE ADD TO LIST
     if (ll<10){
     tempfing.xloc=fingers[k].xloc;
     tempfing.yloc=fingers[k].yloc;
     tempfingers.push_back(tempfing);
     
     }
     
     //IF FINGERS ARE NOT CLOSE, AVERAGE ALL PREVIOUS FINGERS
     else {
     for (int p = 0; p < tempfingers.size(); p++){
     xave += tempfingers[p].xloc;
     yave += tempfingers[p].yloc;
     }
     
     xave /= tempfingers.size();
     yave /= tempfingers.size();
     
     //CREATE CIRCLE AT AVERAGE
     ofFill();
     ofCircle(xave,yave,10);
     numtips++;
     ofNoFill();
     
     //RESET AVERAGES
     xave = 0;
     yave = 0;
     
     //CLEAR TEMP FINGERS
     tempfingers.clear();
     }
     
     }
     
     fingers.clear();
     */
    
}

void Tracker::trackfinger(){
    
    fingers.clear();
    tempfingers.clear();
    
    ofPoint tempcenter;
    
    for (int j = 0; j < contourFinder.nBlobs; j++){
        vector<ofPoint> tempPnts;
        
        tempcenter.x = contourFinder.blobs[j].centroid.x;
        tempcenter.y = contourFinder.blobs[j].centroid.y;
        
        for(int i=0; i<contourFinder.blobs[j].nPts; i++){
            ofPoint tempPnt;
            tempPnt.x = contourFinder.blobs[j].pts[i].x;
            tempPnt.y = contourFinder.blobs[j].pts[i].y;
            
            //ofCircle(tempPnt.x, tempPnt.y, 10);
            
            tempPnts.push_back(tempPnt);
            
            //ofCircle(tempPnts[i].x, tempPnts[i].y, 10);
            
        }
        //INITIATE HAND TRACKING AN FINGER TRACKING
        hands[j].noshuff(tempPnts);
        hands[j].shuffpnts(tempPnts, tempPnts.size()/2, tempcenter);
        
        tempPnts.clear();
    }
    
    
    
    if (hands[0].numtips==1) {
        if (hands[1].numtips==1) {
            
            avex = 0;
            avey = 0;
            avel = 0;
                        
            for (int j = 0; j < 2; j++){
                for (int p = 0; p <1; p++) {
                    avex += hands[j].realfingers[p].x;
                    avey += hands[j].realfingers[p].y;
                }
                
            }
            
            avel = sqrt(
                        
                        (((hands[0].realfingers[0].x)-(hands[1].realfingers[0].x))*
                        ((hands[0].realfingers[0].x)-(hands[1].realfingers[0].x)))
                        +(((hands[0].realfingers[0].y)-(hands[1].realfingers[0].y))*
                          ((hands[0].realfingers[0].y)-(hands[1].realfingers[0].y)))
                        
                        );
            
            avex /=2;
            avey /=2;
            
            //cout<<avex<<" "<<avey<<"\n";
            
            ofPushMatrix();
            
            ofCircle(avex, avey, avel/5);
            
            ofPopMatrix();
            
        }
        
        else{
            ofPushMatrix();
            ofCircle(avex, avey, avel/5);
            ofPopMatrix();
        }

        
    }
    
    else{
        ofPushMatrix();
        ofCircle(avex, avey, avel/5);
        ofPopMatrix();
    }
    
    for (int p = 0; p <2; p++) {
        hands[p].realfingers.clear();
    }
    
    }

//tracking of the hand, and gestures associated with hands
void Tracker::trackhand() {
    ofPushMatrix();
    
    ofTranslate(200, 150, 0);
    glScalef(0.9, 0.9, 1.0f); 
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        ofPushMatrix();
        contourFinder.blobs[i].draw(0,0);
        ofSetColor(255, 0, 0);
        //ofFill();
        //ofEllipse(contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y, 4, 4);
        
        float centroidX = 0;
        float centroidY = 0;
        float addCount = 0;
        for (int j = 0; j < contourFinder.blobs[i].nPts; j+=1){
            addCount++;
            centroidX += contourFinder.blobs[i].pts[j].x;
            centroidY += contourFinder.blobs[i].pts[j].y;
        }
        centroidX = centroidX/addCount;
        centroidY = centroidY/addCount;
        //ofCircle(centroidX, centroidY, 5);
        
        glPushMatrix();
        
        //this draws a 3d sphere at the center of the hand
        int z = kinect.getDistanceAt(centroidX,centroidY);
        glTranslatef(centroidX,centroidY,z/10);
        ofSetColor(0, 255, 0);
        ofSolidSphere(40);
        glPopMatrix();
    
        ofPopMatrix();
    }
    
    
    
    /*
    
    //gesture to track hands when they are closed
    if (contourFinder.nBlobs == 2) {
        if (contourFinder.blobs[0].nPts + contourFinder.blobs[1].nPts < 600) {
            ofPushMatrix();
            xx=(contourFinder.blobs[1].centroid.x+contourFinder.blobs[0].centroid.x)/2;
            yy=(contourFinder.blobs[1].centroid.y+contourFinder.blobs[0].centroid.y)/2;
            int dx = contourFinder.blobs[1].centroid.x-contourFinder.blobs[0].centroid.x;
            int dy = contourFinder.blobs[1].centroid.y-contourFinder.blobs[0].centroid.y;
            ll=sqrt((dx*dx)+(dy*dy))/4;
            
            
            ofCircle(xx, yy, ll);
            ofPopMatrix();
            
        }
        else{
            ofPushMatrix();
            ofCircle(xx, yy, ll);
            ofPopMatrix();
        }
    }
    
    */

}

void Tracker::draw() {
    
    trackhand();
    
    trackfinger();
    
    //this draws a 3d sphere at the center of each finger
    
    for (int p = 0; p<2; p++) {
        for (int i = 0; i<hands[p].numtips; i++) {
            int z = 0;
            
            if (hands[p].numtips != 0){
                z = kinect.getDistanceAt(hands[p].realfingers[i].x,hands[p].realfingers[i].y);
            }
            
            //glLoadIdentity();
            
            //ofSetColor(255, 0, 0);
            //ofCircle(hands[p].realfingers[i].x,hands[p].realfingers[i].y, 5);

            glPushMatrix();

            glTranslatef(hands[p].realfingers[i].x,hands[p].realfingers[i].y,z/10);
            
            ofSetColor(0, 255, 0);
            ofSolidSphere(10);
            
            glPopMatrix();
        }
        
    }
    
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