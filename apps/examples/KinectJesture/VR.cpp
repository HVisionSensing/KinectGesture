//
//  VR.cpp
//  jestureCap
//
//  Created by Noah Tovares on 1/25/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "VR.h"

VVR::VVR(){
    
}

void VVR::DisplayLoc(void){
    for (int i = 0; i < contourFinderVR.nBlobs; i++){
        cout<<i;
        cout<<'\n';
        cout<<contourFinderVR.blobs[i].centroid.x;
        cout<<'\n';
        cout<<contourFinderVR.blobs[i].centroid.y;
        cout<<'\n';
        cout<<"test";
    }
}