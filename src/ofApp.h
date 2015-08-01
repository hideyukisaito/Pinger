#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxBundleResources.h"

class ofApp : public ofBaseApp
{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxOscSender mOscSender;
    ofxOscReceiver mOscReceiver;
    
    string mHost;
    int mSenderPort, mReceiverPort;
    
    string mAppName;
    string mAppPath;
    
    int mPingIntervalMillis;
    int mWaitTimeMillis;
    
    unsigned long long mLastSentTime, mLastReceivedTime;
    string mStatusMsg;
    ofColor mStatColor;
    bool bRebooting;
};
