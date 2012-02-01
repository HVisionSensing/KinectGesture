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


class hand{
    public:
    
    int xloc, yloc;
    
    ofxVec2f   v1,v2,aux1;
    ofxVec3f   v1D,vxv;
    ofxVec3f   v2D;
    
    vector<finger> posfingers;
    vector<finger> realfingers;
    vector<ofPoint> handpnts;
    
    hand(){
        
    }
    
    void shuffpnts(vector<ofPoint> oldpoints){
        for (int k = 0; k<oldpoints.size()/2; k++) {
            ofPoint tempPnt = oldpoints[k];
            handpnts.push_back(tempPnt);
            ofCircle(handpnts[k].x, handpnts[k].y, 10);
        }
        for (int k = oldpoints.size()/2; k<oldpoints.size(); k++) {
            ofPoint tempPnt = oldpoints[k];
            handpnts.push_back(tempPnt);
            ofCircle(handpnts[k].x, handpnts[k].y, 10);
        }
        detectfingers();
    }
    
    void detectfingers(void){
        
        for(int i=0; i<handpnts.size(); i++){ 
            
            v1.set(handpnts[i].x-handpnts[i-35].x,handpnts[i].y-handpnts[i-35].y);
            v2.set(handpnts[i].x-handpnts[i+35].x,handpnts[i].y-handpnts[i+35].y);
            
            //v1.set(contourFinder.blobs[j].pts[i].x-contourFinder.blobs[j].pts[i-k].x,contourFinder.blobs[j].pts[i].y-contourFinder.blobs[j].pts[i-k].y);
            //v2.set(contourFinder.blobs[j].pts[i].x-contourFinder.blobs[j].pts[i+k].x,contourFinder.blobs[j].pts[i].y-contourFinder.blobs[j].pts[i+k].y);
            
            v1D.set(handpnts[i].x-handpnts[i-35].x,handpnts[i].y-handpnts[i-35].y,0);
            v2D.set(handpnts[i].x-handpnts[i+35].x,handpnts[i].y-handpnts[i+35].y,0);
            
            //v1D.set(contourFinder.blobs[j].pts[i].x-contourFinder.blobs[j].pts[i-k].x,contourFinder.blobs[j].pts[i].y-contourFinder.blobs[j].pts[i-k].y,0);
            //v2D.set(contourFinder.blobs[j].pts[i].x-contourFinder.blobs[j].pts[i+k].x,contourFinder.blobs[j].pts[i].y-contourFinder.blobs[j].pts[i+k].y,0);

        }
        
        handpnts.clear();
    }
    
    
};


#endif
