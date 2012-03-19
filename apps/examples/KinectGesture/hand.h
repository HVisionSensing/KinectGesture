//
//  hand.h
//  jestureCap
//
//  Created by Noah Tovares on 2/1/12.
//  Copyright (c) 2012 IDIG Lab CMU All rights reserved.
//

#ifndef jestureCap_hand_h
#define jestureCap_hand_h

#include "finger.h"
#include "ofxCvConstants.h"
#include "ofxKinect.h"
#include "Tracker.h"
#include "fourpoint.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

class hand{
    public:
    
    ofstream myfile, myfile1;
    
    int xloc, yloc, dx, dy, dz, dl, ll, dq;
    int numtips;
    double xave, yave, xtot, ytot, zave, ztot;
    float teta;
    
    ofxVec2f   v1,v2,aux1;
    ofxVec3f   v1D,v2D, vxv;

    ofPoint centroid;
    
    vector<ofPoint> posfingers;
    vector<ofPoint> realfingers;
    vector<ofPoint> handpnts;  
    vector<fourpoint> fourfingers;
    
    hand(){
    }
    
    // takes points that make up hand and shuffles them so the start is at the bottom
    void shuffpnts(vector<ofPoint> oldpoints, int h, ofPoint center, const char handnum[]){
        centroid = center;
        
        // display center of old hand position
        ofSetColor(0, 255, 0);
        //ofCircle(oldpoints[0].x, oldpoints[0].y, 10);
        
        // display center of new hand position
        ofSetColor(0, 0, 255);
        //ofCircle(oldpoints[oldpoints.size()/2].x, oldpoints[oldpoints.size()/2].y, 10);
        
        ofSetColor(255, 0, 0);
        
        // shuffle the poits by a number h push back to handpnts
        for (int k = h; k<oldpoints.size(); k++) {
            ofPoint tempPnt = oldpoints[k];
            handpnts.push_back(tempPnt);
            //ofCircle(handpnts[k].x, handpnts[k].y, 5);
        }
        
        // push the remaining points to handpnts
        for (int k = 0; k<h; k++) {
            ofPoint tempPnt = oldpoints[k];
            handpnts.push_back(tempPnt);
            //ofCircle(handpnts[k].x, handpnts[k].y, 5);
        }
        
        // initialize the detection of fingers
        detectfingers(handnum);
    }
    
    //detect fingers without shuffling the points around
    void noshuff(vector<ofPoint> oldpoints, const char handnum[]){
        detectfingers(handnum);
    }
    
    //detect the existence of fingers
    void detectfingers(const char handnum[]){
        
        posfingers.clear();
        
        for(int i=0; i<handpnts.size(); i++){ 
            
            //integers that determines points that make up triangle
            int l = 25;
            
            //vectors that make up triangle
            v1.set(handpnts[i].x-handpnts[i-l].x,handpnts[i].y-handpnts[i-l].y);
            v2.set(handpnts[i].x-handpnts[i+l].x,handpnts[i].y-handpnts[i+l].y);
            v1D.set(handpnts[i].x-handpnts[i-l].x,handpnts[i].y-handpnts[i-l].y,0);
            v2D.set(handpnts[i].x-handpnts[i+l].x,handpnts[i].y-handpnts[i+l].y,0);

            //Cross product to weed out finger hulls
            vxv = v1D.cross(v2D);
            
            //normalize triangle vectors
            v1.normalize();
            v2.normalize();
            
            //the angle between the normalized vectors
            teta=v1.angle(v2);
            
            if(fabs(teta) < 40){
                if(vxv.z > 0){
                    if (handpnts[i].y<centroid.y+10) {
                        
                        //create temporary point to push back
                        ofPoint tempPnt;
                        tempPnt.x = handpnts[i].x;
                        tempPnt.y = handpnts[i].y;
                        
                        //REPLACEMENT CODE 
                        fourpoint tempfour;
                        tempfour.x = handpnts[i].x;
                        tempfour.y = handpnts[i].y;
                        tempfour.z = handpnts[i].z;
                        tempfour.q = i/10;
                        fourfingers.push_back(tempfour);
                        //
                        
                        //assign z value of possible finger to i, or index in handpoints
                        //use later to group fingers
                        tempPnt.z = i/10;
                        
                        //add possible finger to ofPoint vector
                        posfingers.push_back(tempPnt);
                        
                        //ofCircle(tempPnt.x, tempPnt.y, i/10);
                        //cout<<i/10;
                        //cout<<"\n";
                        
                        //ofTranslate(0, 0);
                        
                        //draw circle at all fingertip locations
                        //ofCircle(tempPnt.x, tempPnt.y, 15);
                        
                        //double check to make sure posfingers contains correct points
                        //int k = posfingers.size();
                        //ofCircle(posfingers[k].x, posfingers[k].y, i/10);
                        
                    }
                }
            }
        }
        
        // draw the "fingers"
        drawfingersq(handnum);
        
        handpnts.clear();
  
    }
    
    //group and draw fingers based on the q value of the points
    void drawfingersq(const char handnum[]){
        
        myfile.open (handnum,ios::app);
        
        //myfile1.open ("/Users/noahtovares/Desktop/KinectTxt/numfingers.txt",ios::app);

        numtips = 0;
        int counter = 0;
        int mindensity = 1;
        
        for (int k = 1; k<fourfingers.size(); k++) {
            
            //compute z distance between one finger and the next
            dq = fourfingers[k-1].q-fourfingers[k].q;
            
            //if the two fingers are close
            if(abs(dq)<2) {
                xtot += fourfingers[k-1].x;
                ytot += fourfingers[k-1].y;
                ztot += fourfingers[k-1].z;
                
                counter++;
            }
            
            // if the two fingers are not close
            else if(abs(dq)>2){
                
                // if the density of fingers is significant enough to possible be a real finger, push back average position to realfingers
                if (counter > mindensity) {
                    xave = xtot/counter;
                    yave = ytot/counter;
                    zave = ztot/counter;
                    
                    ofFill();
                    ofSetColor(0,0,255);
                    
                    ofCircle(xave,yave,20);
                    numtips++;
                    ofNoFill();
                    
                    ofPoint tempPnt;
                    tempPnt.x = xave;
                    tempPnt.y = yave;
                    tempPnt.z = zave;
                    realfingers.push_back(tempPnt);
                    
                    // write the real finger position to the txt file
                    myfile <<xave <<"," << yave <<" ";
                }
                
                // reset all the values
                dq=0;
                xave = 0;
                yave = 0;
                zave = 0;
                xtot = 0;
                ytot = 0;
                ztot = 0;
                counter = 0;
                
            }
            
            // if the loop has reached the end of the possible fingers
            if(k==fourfingers.size()-1){
                
                // if the density of fingers is significant enough to possible be a real finger, push back average position to realfingers
                if (counter > mindensity) {
                    xave = xtot/counter;
                    yave = ytot/counter;
                    zave = ztot/counter;
                    
                    ofFill();
                    ofSetColor(0,0,255);
                    ofCircle(xave,yave,20);
                    numtips++;
                    ofNoFill();
                    
                    ofPoint tempPnt;
                    tempPnt.x = xave;
                    tempPnt.y = yave;
                    tempPnt.z = zave;
                    realfingers.push_back(tempPnt);
                    
                    // write the real finger position to the txt file
                    myfile << xave <<"," << yave<<" ";
                }
                
                // reset all the values
                dq=0;
                xave = 0;
                yave = 0;
                zave = 0;
                xtot = 0;
                ytot = 0;
                ztot = 0;
                counter = 0;
            }
            
        }
        
        //if (numtips == 0){
        //    myfile<<"e";
        //}
        
        fourfingers.clear();
        
        myfile<<"\n";
        
        myfile.close();
        
    }
    
};


#endif
