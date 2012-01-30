//
//  finger.cpp
//  jestureCap
//
//  Created by Noah Tovares on 1/30/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "finger.h"

finger::finger(){

}

void finger::setpos(double x, double y){
    xloc=x;
    yloc=y;
}

double finger::getloc(char q){
    if(q=='x') return xloc;
    if(q=='y') return yloc;
}
