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
    float teta;
    
    vector<ofPoint> posfingers;
    vector<ofPoint> realfingers;
    vector<ofPoint> handpnts;
    
    hand(){
        
    }
    
    void shuffpnts(vector<ofPoint> oldpoints){
        for (int k = 0; k<oldpoints.size()/2; k++) {
            ofPoint tempPnt = oldpoints[k];
            handpnts.push_back(tempPnt);
            //ofCircle(handpnts[k].x, handpnts[k].y, 10);
        }
        for (int k = oldpoints.size()/2; k<oldpoints.size(); k++) {
            ofPoint tempPnt = oldpoints[k];
            handpnts.push_back(tempPnt);
            //ofCircle(handpnts[k].x, handpnts[k].y, 10);
        }
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
            
            if(fabs(teta) < 40){
                if(vxv.z > 0){
                    
                    
                    ofPoint tempPnt;
                    tempPnt.x = handpnts[i].x;
                    tempPnt.y = handpnts[i].y;
                    
                    posfingers.push_back(tempPnt);
                    
                    ofCircle(tempPnt.x, tempPnt.y, 10);
                }
            }
        }
        handpnts.clear();
    }
};


#endif
