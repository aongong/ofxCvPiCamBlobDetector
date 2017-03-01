#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxCvPiCam.h"
#include "ofxGui.h"
class Glow : public ofxCv::RectFollower {
protected:
	ofColor color;
	ofVec2f cur, smooth;
	float startedDying;
	ofPolyline all;
public:
	Glow()
		:startedDying(0) {
	}
	void setup(const cv::Rect& track);
	void update(const cv::Rect& track);
	void kill();
	void draw();
};


class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
        void keyPressed(int key);
	
    ofxCvPiCam cam;
    

    cv::Mat frame, thresh;
    ofxCv::ContourFinder contourFinder;
    ofxCv::RectTrackerFollower<Glow> tracker;
   


ofxPanel gui;
ofParameter<float> minArea, maxArea, thresholdValue;
ofParameter<bool> holes, color;


    
    
};
