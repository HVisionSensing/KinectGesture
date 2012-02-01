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
        
    }
};


#endif
