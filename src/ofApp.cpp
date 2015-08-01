#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofxBundleResources::setup();
    
    ofSetWindowShape(800, 200);
    
    ofxXmlSettings xml_("settings.xml");
    mHost = xml_.getAttribute("sender", "host", "localhost");
    mSenderPort = xml_.getAttribute("sender", "port", 10001);
    mReceiverPort = xml_.getAttribute("receiver", "port", 10000);
    
    mPingIntervalMillis = xml_.getValue("ping_interval_millis", 1000);
    mWaitTimeMillis = xml_.getValue("reboot_after_millis", 5000);
    
    mAppName = xml_.getAttribute("app", "name", "");
    mAppPath = xml_.getAttribute("app", "path", "");
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    mOscSender.setup(mHost, mSenderPort);
    mOscReceiver.setup(mReceiverPort);
    
    mLastSentTime = ofGetElapsedTimeMillis();
    mLastReceivedTime = ofGetElapsedTimeMillis();
    
    mStatusMsg = "OK";
    mStatColor = ofColor(0, 255, 0);
    bRebooting = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (mPingIntervalMillis < ofGetElapsedTimeMillis() - mLastSentTime) {
        ofxOscMessage msg_;
        msg_.setAddress("/rcm/ping");
        mOscSender.sendMessage(msg_);
        
        mLastSentTime = ofGetElapsedTimeMillis();
    }
    
    while (mOscReceiver.hasWaitingMessages()) {
        ofxOscMessage msg_;
        mOscReceiver.getNextMessage(&msg_);
        
        string addr_ = msg_.getAddress();
        
        if ("/rcm/ping" == addr_) {
            mLastReceivedTime = ofGetElapsedTimeMillis();
        }
    }
    
    // 死んだ判定
    if (mWaitTimeMillis < ofGetElapsedTimeMillis() - mLastReceivedTime) {
        mStatusMsg = "rebooting...";
        mStatColor.set(255, 0, 0);
        
        if (!bRebooting) {
            bRebooting = true;
            string command_ = "killall -9 " + mAppName;
            int result_ = system(command_.c_str());
            ofLog() << "shutdown: " << result_;
            
            command_ = "open \"" + mAppPath + "\"";
            result_ = system(command_.c_str());
            ofLog() << "open: " << result_;
            bRebooting = false;
        }
    } else {
        mStatusMsg = "ok";
        mStatColor.set(0, 255, 0);
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);
    ofSetColor(255);
    
    ofDrawBitmapString("name: " + mAppName, 10, 20);
    ofDrawBitmapString("path: " + mAppPath, 10, 40);
    
    ofDrawBitmapString("ping to " + mHost + ":" + ofToString(mSenderPort), 10, 70);
    ofDrawBitmapString("elapsed time from last ping: ", 10, 90);
    ofDrawBitmapString(ofToString(ofGetElapsedTimeMillis() - mLastSentTime), 280, 90);
    
    ofDrawBitmapString("ping from " + mHost + ":" + ofToString(mReceiverPort), 10, 120);
    ofDrawBitmapString("elapsed time from last received: ", 10, 140);
    ofDrawBitmapString(ofToString(ofGetElapsedTimeMillis() - mLastReceivedTime), 280, 140);
    
    ofDrawBitmapString("status: ", 10, 170);
    ofSetColor(mStatColor);
    ofDrawBitmapString(mStatusMsg, 70, 170);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}
