//
//  hand.h
//  jestureCap
//
//  Created by Noah Tovares on 2/1/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef jestureCap_hand_h
#define jestureCap_hand_h
#include "finger.h"
#include "ofxCvConstants.h"
#include "ofxKinect.h"
#include "Tracker.h"

class hand{
    public:
    
    int xloc, yloc;
    double xave;
    double yave;
    
    ofxVec2f   v1,v2,aux1;
    ofxVec3f   v1D,vxv;
    ofxVec3f   v2D;
    float teta;
    
    ofPoint centroid;
    
    vector<ofPoint> posfingers;
    vector<ofPoint> realfingers;
    vector<ofPoint> handpnts;
    
    hand(){
    }
    
    void shuffpnts(vector<ofPoint> oldpoints, int h, ofPoint center){
        centroid = center;
        //h = oldpoints.size()/2;
        
        ofSetColor(0, 255, 0);
        ofCircle(oldpoints[0].x, oldpoints[0].y, 10);
        
        ofSetColor(0, 0, 255);
        ofCircle(oldpoints[oldpoints.size()/2].x, oldpoints[oldpoints.size()/2].y, 10);
        
        ofSetColor(255, 0, 0);
        
        //for (int k = 0; k<oldpoints.size()/2; k++) {
        for (int k = h; k<oldpoints.size(); k++) {

            ofPoint tempPnt = oldpoints[k];
            handpnts.push_back(tempPnt);
            //ofCircle(handpnts[k].x, handpnts[k].y, 5);
        }
        //for (int k = oldpoints.size()/2; k<oldpoints.size(); k++) {
        for (int k = 0; k<h; k++) {
            ofPoint tempPnt = oldpoints[k];
            handpnts.push_back(tempPnt);
            //ofCircle(handpnts[k].x, handpnts[k].y, 5);
        }
        //detectfingers();
    }
    
    void noshuff(vector<ofPoint> oldpoints){
        detectfingers();
    }
    
    void detectfingers(void){
        
        for(int i=0; i<handpnts.size(); i++){ 
            
            v1.set(handpnts[i].x-handpnts[i-35].x,handpnts[i].y-handpnts[i-35].y);
            v2.set(handpnts[i].x-handpnts[i+35].x,handpnts[i].y-handpnts[i+35].y);
            
            v1D.set(handpnts[i].x-handpnts[i-35].x,handpnts[i].y-handpnts[i-35].y,0);
            v2D.set(handpnts[i].x-handpnts[i+35].x,handpnts[i].y-handpnts[i+35].y,0);

            vxv = v1D.cross(v2D);
            
            v1.normalize();
            v2.normalize();
            
            teta=v1.angle(v2);
            
            if(fabs(teta) < 30){
                if(vxv.z > 0){
                    if (handpnts[i].y<centroid.y+20) {
                        ofPoint tempPnt;
                        tempPnt.x = handpnts[i].x;
                        tempPnt.y = handpnts[i].y;
                        
                        posfingers.push_back(tempPnt);
                        
                        ofCircle(tempPnt.x, tempPnt.y, 10);
                        
                    }
                }
            }
        }
        drawfingers();
        handpnts.clear();
        
  
    }
    
    void drawfingers(void){
        
        for (int k = 0; k<posfingers.size(); k++) {
            
            //DETERMINE THE DISTANCE BETWEEN FINGER THE THE NEXT FINGER
            
            int dx = posfingers[k].x - posfingers[k+1].x;
            int dy = posfingers[k].y - posfingers[k+1].y;
            int ll = sqrt((dx*dx)+(dy*dy)); 
            
            if (ll<10){
                realfingers.push_back(posfingers[k]);
            }
            
            else {
                for (int p = 0; p < realfingers.size(); p++){
                    xave += realfingers[p].x;
                    yave += realfingers[p].y;
                }
                
                xave /= realfingers.size();
                yave /= realfingers.size();
                
                //CREATE CIRCLE AT AVERAGE
                ofFill();
                //ofCircle(xave,yave,10);
                //numtips++;
                ofNoFill();
                
                //RESET AVERAGES
                xave = 0;
                yave = 0;
                
                //CLEAR TEMP FINGERS
                realfingers.clear();
                
            }
            
        }
        
        posfingers.clear();

    }
    
};


#endif
