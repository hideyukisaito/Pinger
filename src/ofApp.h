#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxTimer.h"
#include "ofxXmlSettings.h"
#include "ofxBundleResources.h"

class ThreadedOSCReceiver : public ofThread
{
public:
    
    ~ThreadedOSCReceiver()
    {
        if (isThreadRunning()) stopThread();
        ofLogNotice() << "ThreadedOSCReceiver stoped";
    }
    
    void setup(int port)
    {
        receiver.setup(port);
        startThread();
    }
    
    void threadedFunction()
    {
        while (isThreadRunning())
        {
            if (receiver.hasWaitingMessages())
            {
                if (receiver.getNextMessage(&msg) && lock())
                {
                    ofxOscMessage m = msg;
                    ofNotifyEvent(messageReceived, msg, this);
                    unlock();
                }
                
            }
            
            ofSleepMillis(10);
        }
    }
    
    ofEvent<ofxOscMessage> messageReceived;
    
private:
    
    ofxOscReceiver receiver;
    ofxOscMessage msg;
    
};


enum AppStatus
{
    APP_ALIVE = 0,
    APP_DEAD = 1,
    APP_UNKNOWN = 2,
    APP_REBOOTING = 3,
};


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
    
    ofxXmlSettings setting;
    
    ThreadedOSCReceiver receiver;
    ofxOscSender sender;
    ofxTimer timer;
    
    unsigned long long lastPingTime;
    string lastPingTimestamp;
    
    AppStatus status;
    string appName;
    string appPath;
    bool bStartWatching;
    string logString;
    
    string remotePingAddr, incomingPingAddr;
    string bootAddr;
    int rebootInterval;
    int limitTimeMillis;
    
    void ping();
    
    void onMessageReceived(ofxOscMessage &msg);
    void onTimerReached(ofEventArgs &e);
    
};