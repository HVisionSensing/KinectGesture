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

class hand{
    public:
    
    int xloc, yloc, dx, dy, dz, ll;
    int numtips;
    double xave, yave, xtot, ytot;
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
        //detectfingers();
    }
    
    //detect fingers without shuffling the points around
    void noshuff(vector<ofPoint> oldpoints){
        detectfingers();
    }
    
    //detect the existence of fingers
    void detectfingers(void){
        
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
                        ofCircle(tempPnt.x, tempPnt.y, 10);
                        
                    }
                }
            }
        }
        
        //draw the "fingers"
        //drawfingersnew();
        
        handpnts.clear();
  
    }
    
    //attempt to sort the vectors
    void sortfingers(void){
        
        ofPoint tempPnt;
        
        tempPnt.x = posfingers[0].x;
        tempPnt.y = posfingers[0].y;
        
        for (int b = 0; b < posfingers.size(); b++) {
            
            //FIND FURTHEST TO THE LEFT
            if (posfingers[b].x < tempPnt.x){
                tempPnt.x = posfingers[b].x;
                tempPnt.y = posfingers[b].y;
            }
            
            
        }
        
    }
    
    //draw and group fingers based on pre determined values
    void drawfingerspre(void){
        double x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, x6, y6;
        double xx1 =0;
        double xx2 =0;
        double xx3 =0;
        double xx4 =0;
        double xx5 =0;
        double xx6 =0;
        
        for (int k = 0; k<posfingers.size(); k++) {
            //ofCircle(posfingers[k].x,posfingers[k].y,posfingers[k].z);
        }
        
        //posfingers.clear();
        
        for (int k = 0; k<posfingers.size(); k++){
            
            //if(posfingers[k].z>0 && posfingers[k].z<6){
            //    x1 += posfingers[k].x;
            //    y1 += posfingers[k].y;
            //    xx1++;
            //}
            if(posfingers[k].z>10 && posfingers[k].z<19){
                x2 += posfingers[k].x;
                y2 += posfingers[k].y;
                xx2++;
            }
            if(posfingers[k].z>20 && posfingers[k].z<29){
                x3 += posfingers[k].x;
                y3 += posfingers[k].y;
                xx3++;
            }
            if(posfingers[k].z>30 && posfingers[k].z<39){
                x4 += posfingers[k].x;
                y4 += posfingers[k].y;
                xx4++;
            }
            if(posfingers[k].z>40 && posfingers[k].z<49){
                x5 += posfingers[k].x;
                y5 += posfingers[k].y;
                xx5++;
            }
            if(posfingers[k].z>52 && posfingers[k].z<59){
                x1 += posfingers[k].x;
                y1 += posfingers[k].y;
                xx1++;
            }
            
            if(posfingers[k].z>0 && posfingers[k].z<6){
                x6 += posfingers[k].x;
                y6 += posfingers[k].y;
                xx6++;
            }
        }
        
        x1 /= xx1;
        x2 /= xx2;
        x3 /= xx3;
        x4 /= xx4;
        x5 /= xx5;
        x6 /= xx6;
        
        y1 /= xx1;
        y2 /= xx2;
        y3 /= xx3;
        y4 /= xx4;
        y5 /= xx5;
        y6 /= xx6;
        
        ofFill();
        ofCircle(x1, y1, 10);   
        ofCircle(x2, y2, 10);
        ofCircle(x3, y3, 10);
        ofCircle(x4, y4, 10);
        ofCircle(x5, y5, 10);
        ofCircle(x6, y6, 10);
        ofNoFill();
        
        posfingers.clear();
    }
    
    //create vetor, iterate through points, grab and erase
    void drawfingersvetor(void){
    
        //create posfingers copy and fill with posfingers
        for (int j = 0; j <posfingers.size();j++){
            posfingerscopy.push_back(posfingers[j]);
        }
        
        //continue as long as the copied vector has elements
        if(posfingerscopy.size() > 0) {
             
            //take the first posfingercopy point at the reference
            fingerpnts.push_back(posfingerscopy[0]);
            
            //iterate through all the possible fingers
            for (int k = 1; k<posfingerscopy.size(); k++) {
                dx = fingerpnts[0].x - posfingerscopy[k].x;
                dy = fingerpnts[0].y - posfingerscopy[k].y;
                ll = sqrt((dx*dx)+(dy*dy)); 
                
                //if distance between points is less than 5
                if(ll<5){
                    //add finger to fingerpnts vector
                    fingerpnts.push_back(posfingerscopy[k]);
                    
                    //delete from posfingerscopy
                    posfingerscopy.erase(posfingerscopy.begin()+k-1);
                }
            }
            
            //create average of fingerponts
            for (int j = 0; j<fingerpnts.size(); j++) {
                xtot += fingerpnts[j].x;
                ytot += fingerpnts[j].y;
            }
            
            xave = xtot/fingerpnts.size();
            yave = ytot/fingerpnts.size();
            
            ofFill();
            ofCircle(xave,yave,10);
            ofNoFill();
            
            xave = 0;
            yave = 0;
            xtot = 0;
            ytot = 0;
            
            fingerpnts.clear();

        }
         
                
    }
    
    //group and draw fingers based on the z value of the points
    void drawfingersz(void){
        int counter = 0;
        
        for (int k = 0; k<posfingers.size(); k++) {
            
            dz = posfingers[k].z-posfingers[k+1].z;
            //cout<<dz;
            //cout<<"\n";
            
            if(abs(dz)<5) {
                //realfingers.push_back(posfingers[k+1]);
                xtot += posfingers[k+1].x;
                ytot += posfingers[k+1].y;
                counter++;
                
            }
            
            if(abs(dz)>5){
                
                //xave = xtot/realfingers.size();
                //yave = ytot/realfingers.size();
                
                xave = xtot/counter;
                yave = ytot/counter;
                
                ofFill();
                ofCircle(xave,yave,10);
                numtips++;
                ofNoFill();
                
                dx=0;
                dy=0;
                dz=0;
                ll=0;
                xave = 0;
                yave = 0;
                xtot = 0;
                ytot = 0;
                counter = 0;
                realfingers.clear();
                
                if (numtips == 3) k = posfingers.size();
                
                
            }
            
        }
        
        posfingers.clear();
    }
    
    //determine average finger locations, and draw fingertips based on Z value
    void drawfingers(void){
         
       /* 
        for (int k = 0; k<posfingers.size(); k++) {

            //DETERMINE THE DISTANCE BETWEEN FINGER THE THE NEXT FINGER
            
            dx = posfingers[k].x - posfingers[k+1].x;
            dy = posfingers[k].y - posfingers[k+1].y;
            ll = sqrt((dx*dx)+(dy*dy)); 
            
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
                ofCircle(xave,yave,10);
                numtips++;
                cout<<numtips;
                cout<<"\n";
                ofNoFill();
                
                //RESET VALUES
                dx=0;
                dy=0;
                ll=0;
                xave = 0;
                yave = 0;
                realfingers.clear();
                
                if (numtips == 4) k = posfingers.size();
                
            }
            
        }
        
        numtips = 0;
        posfingers.clear();
        */

    }
    
};


#endif
