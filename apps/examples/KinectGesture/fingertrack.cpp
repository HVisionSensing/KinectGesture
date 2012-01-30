//
//  fingertrack.cpp
//  jestureCap
//
//  Created by Noah Tovares on 1/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//



#include "fingertrack.h"

fingertracker::fingertracker(){
    k=35;
    smk=200;
    teta=0.f;
    numfingers =0;
}

void fingertracker::detectfinger(){
    /*
    for (int j = 0; j < contourFinder.nBlobs; j++){
        
        for(int i=k; i<contourFinder.blobs[0].nPts-k; i++){ 
            
            v1.set(contourFinder.blobs[j].pts[i].x-contourFinder.blobs[j].pts[i-k].x,contourFinder.blobs[j].pts[i].y-contourFinder.blobs[j].pts[i-k].y);
            v2.set(contourFinder.blobs[j].pts[i].x-contourFinder.blobs[j].pts[i+k].x,contourFinder.blobs[j].pts[i].y-contourFinder.blobs[j].pts[i+k].y);
            
            v1D.set(contourFinder.blobs[j].pts[i].x-contourFinder.blobs[j].pts[i-k].x,contourFinder.blobs[j].pts[i].y-contourFinder.blobs[j].pts[i-k].y,0);
            v2D.set(contourFinder.blobs[j].pts[i].x-contourFinder.blobs[j].pts[i+k].x,contourFinder.blobs[j].pts[i].y-contourFinder.blobs[j].pts[i+k].y,0);
            
            vxv = v1D.cross(v2D);
            
            v1.normalize();
            v2.normalize();
            
            teta=v1.angle(v2);
            
            if(fabs(teta) < 30){
                ofCircle(contourFinder.blobs[j].pts[i].x, contourFinder.blobs[j].pts[i].y, 10);
                i=i+100;
                numfingers++;
            }
        }
    }
     */
}