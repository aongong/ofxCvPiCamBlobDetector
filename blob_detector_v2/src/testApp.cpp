#include "testApp.h"

using namespace ofxCv;
using namespace cv;

const float dyingTime = 1;

void Glow::setup(const cv::Rect& track) {
	color.setHsb(ofRandom(0, 255), 255, 255);
	cur = toOf(track).getCenter();
	smooth = cur;
}

void Glow::update(const cv::Rect& track) {
	cur = toOf(track).getCenter();
	smooth.interpolate(cur, .5);
	all.addVertex(smooth);
}

void Glow::kill() {
	float curTime = ofGetElapsedTimef();
	if(startedDying == 0) {
		startedDying = curTime;
	} else if(curTime - startedDying > dyingTime) {
		dead = true;
	}
}

void Glow::draw() {
	ofPushStyle();
	float size = 16;
	ofSetColor(255);
	if(startedDying) {
		ofSetColor(ofColor::red);
		size = ofMap(ofGetElapsedTimef() - startedDying, 0, dyingTime, size, 0, true);
	}
	ofNoFill();
	ofDrawCircle(cur, size);
	ofSetColor(color);
	all.draw();
	ofSetColor(255);
        //information of cur
	ofDrawBitmapStringHighlight(ofToString(label)+": "+ofToString(cur.x)+"/"+ofToString(cur.y), cur);
	ofPopStyle();
}

void testApp::setup() {
    ofSetVerticalSync(true);
    cam.setup(320, 240,false);

     	tracker.setPersistence(15);
	// an object can move up to 50 pixels per frame
	tracker.setMaximumDistance(50);
    
    gui.setup();
    gui.add(minArea.set("Min area", 5, 1, 100));
    gui.add(maxArea.set("Max area", 30, 1, 500));
    gui.add(thresholdValue.set("Threshold", 128, 0, 255));
    gui.add(holes.set("Holes", true));
    gui.add(color.set("Color", false));
}

void testApp::update() {
    ofBackground(100,100,100);
    frame = cam.grab();
    thresh = frame.clone(); 
    
    if(!frame.empty()) {
        blur(thresh, 5);
        contourFinder.setMinAreaRadius(minArea);
        contourFinder.setMaxAreaRadius(maxArea);
	contourFinder.setThreshold(thresholdValue);
        contourFinder.findContours(thresh);
        contourFinder.setFindHoles(holes);
        threshold(thresh, thresholdValue);


        tracker.track(contourFinder.getBoundingRects());
	}
}

void testApp::draw() {
        ofSetColor(255);
    if(!frame.empty()) {
        
        drawMat(frame,0,0);
        drawMat(thresh,320, 0);
	//contourFinder.draw();
        ofNoFill();
        
        //int n = contourFinder.size();
        //for(int i = 0; i < n; i++) {
        //    ofVec2f centroid = toOf(contourFinder.getCentroid(i));
        //    ofSetColor(cyanPrint);
        //    ofCircle(centroid, 5);
        //if(contourFinder.getHole(i)) {
        //    ofDrawBitmapStringHighlight("hole "+ofToString(centroid.x)+", "+ofToString(centroid.y), centroid.x, centroid.y);
        //}
        //}
        //
	vector<Glow>& followers = tracker.getFollowers();
	for(int i = 0; i < followers.size(); i++) {
	      followers[i].draw();
              
	}
}

        //report
        ofSetHexColor(0xffffff);
	stringstream reportStr;
	reportStr << "blob detection" << endl
		  << "threshold " << thresholdValue << " (press: p/-)" << endl
		//<< "num blobs found " << contourFinder.nBlobs 
                  << ", fps: " << ofGetFrameRate();
	ofDrawBitmapString(reportStr.str(), 20, 400);

    gui.draw();
    gui.setPosition(20,250);


}

void testApp::keyPressed(int key){

	switch (key){
		case 'p':
			thresholdValue ++;
			if (thresholdValue > 255) thresholdValue = 255;
			break;
		
		case '-':
			thresholdValue --;
			if (thresholdValue < 0) thresholdValue = 0;
			break;
                    }
}
