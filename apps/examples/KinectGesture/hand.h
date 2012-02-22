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

class hand{
    public:
    
    int xloc, yloc, dx, dy, dz, dl, ll, dq;
    int numtips;
    double xave, yave, xtot, ytot, zave, ztot;
    float teta;
    
    ofxVec2f   v1,v2,aux1;
    ofxVec3f   v1D,v2D, vxv;

    ofPoint centroid;
    
    vector<ofPoint> posfingers;
    vector<ofPoint> sortedfingers;
    vector<ofPoint> realfingers;
    vector<ofPoint> handpnts;
    vector<ofPoint> posfingerscopy;
    vector<ofPoint> fingerpnts;   
    vector<fourpoint> fourfingers;
    
    hand(){
    }
    
    //takes points that make up hand and shuffles them so the start is at the bottom
    void shuffpnts(vector<ofPoint> oldpoints, int h, ofPoint center){
        centroid = center;
        
        //display center of old hand position
        ofSetColor(0, 255, 0);
        //ofCircle(oldpoints[0].x, oldpoints[0].y, 10);
        
        //display center of new hand position
        ofSetColor(0, 0, 255);
        //ofCircle(oldpoints[oldpoints.size()/2].x, oldpoints[oldpoints.size()/2].y, 10);
        
        ofSetColor(255, 0, 0);
        
        //shuffle the poits by a number h
        for (int k = h; k<oldpoints.size(); k++) {
            ofPoint tempPnt = oldpoints[k];
            handpnts.push_back(tempPnt);
            //ofCircle(handpnts[k].x, handpnts[k].y, 5);
        }
        for (int k = 0; k<h; k++) {
            ofPoint tempPnt = oldpoints[k];
            handpnts.push_back(tempPnt);
            //ofCircle(handpnts[k].x, handpnts[k].y, 5);
        }
        
        //initialize the detection of fingers
        detectfingers();
    }
    
    //detect fingers without shuffling the points around
    void noshuff(vector<ofPoint> oldpoints){
        detectfingers();
    }
    
    //detect the existence of fingers
    void detectfingers(void){
        
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
        
        //draw the "fingers"
        drawfingersq();
        handpnts.clear();
  
    }
    
    //group and draw fingers based on the q value of the points
    void drawfingersq(void){
        
        numtips = 0;
        int counter = 0;
        int mindensity = 1;
        
        for (int k = 1; k<fourfingers.size(); k++) {
            
            //compute z distance between one finger and the next
            dq = fourfingers[k-1].q-fourfingers[k].q;
            //cout<<dz;
            //cout<<"\n";
            
            //if the two fingers are close
            if(abs(dq)<2) {
                xtot += fourfingers[k-1].x;
                ytot += fourfingers[k-1].y;
                ztot += fourfingers[k-1].z;
                
                //cout<<posfingers[k].z<<",";
                //cout<<" " <<k <<" of " <<posfingers.size() <<"\n";
                
                counter++;
            }
            
            else if(abs(dq)>2){
                //cout<<",n,";
                //xtot += posfingers[k-1].x;
                //ytot += posfingers[k-1].y;
                
                //counter++;
                
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
                    //cout<<tempPnt.z<<" ";
                    //cout<<tempPnt.x<<" "<<tempPnt.y<<"\n";
                    realfingers.push_back(tempPnt);
                }
                
                
                dq=0;
                xave = 0;
                yave = 0;
                zave = 0;
                xtot = 0;
                ytot = 0;
                ztot = 0;
                counter = 0;
                
            }
            
            
            if(k==fourfingers.size()-1){
                //cout<<",n,";
                
                //counter++;
                //xtot += posfingers[k].x;
                //ytot += posfingers[k].y;
                
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
                    //cout<<tempPnt.x<<" "<<tempPnt.y<<"\n";
                    realfingers.push_back(tempPnt);
                }
                
                dq=0;
                xave = 0;
                yave = 0;
                zave = 0;
                xtot = 0;
                ytot = 0;
                ztot = 0;
                counter = 0;
            }
            
            
            
            //cout<<numtips<<"\n";
        }
        
        //cout<<"\n";
        
        fourfingers.clear();
        
        
    }

    
};


#endif
