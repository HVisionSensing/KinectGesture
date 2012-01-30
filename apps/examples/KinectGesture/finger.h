//
//  finger.h
//  jestureCap
//
//  Created by Noah Tovares on 1/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef jestureCap_finger_h
#define jestureCap_finger_h

class finger{
    finger();
private:
    double xloc, yloc;
    void setpos(double x, double y);
    double getloc(char q);
};

#endif
