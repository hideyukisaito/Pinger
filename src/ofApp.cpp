#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(30);
    ofSetWindowPosition(0, 0);
    
    if (setting.loadFile("config/settings.xml"))
    {
        logString = "STATUS : " + ofToString(APP_UNKNOWN) + "\nSETTING LOADED.\nPRESS SPACE OR BOOT UP TARGET APPLICATION TO START WATCHING.";
        
        appName = setting.getValue("settings:target:name", "");
        ofDirectory path(setting.getValue("settings:target:path", ""));
        appPath = path.getAbsolutePath();
        ofLogNotice() << appPath;
        
        receiver.setup(setting.getValue("settings:general:osc:incoming:port", 13373));
        sender.setup(
                     setting.getValue("settings:general:osc:remote:host", "localhost"),
                     setting.getValue("settings:general:osc:remote:port", 13374)
                     );
        
        remotePingAddr = setting.getValue("settings:general:osc:ping:address:remote", "/tebatype/server/ping");
        incomingPingAddr = setting.getValue("settings:general:osc:ping:address:incoming", "/tebatype/client/ping");
        bootAddr = setting.getValue("settings:general:osc:boot:address", "/tebatype/client/boot");
        rebootInterval = setting.getValue("settings:target:reboot:wait_interval", 10000);
        limitTimeMillis = setting.getValue("settings:general:osc:ping:limit_time", 5000);
        
        timer.setup(setting.getValue("settings:general:osc:ping:interval", 2000), true);
        timer.stopTimer();
        
        status = APP_UNKNOWN;
        bStartWatching = false;
        
        ofAddListener(receiver.messageReceived, this, &ofApp::onMessageReceived);
        ofAddListener(timer.TIMER_REACHED, this, &ofApp::onTimerReached);
        
        timer.startTimer();
    }
    else
    {
        ofLogFatalError("Pinger") << "setting file cannot load, exit.";
        ofExit(-1);
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (bStartWatching)
    {
        logString = "STATUS : " + ofToString(status) + "\nLAST PING : " + lastPingTimestamp + ", " + ofToString(ofGetElapsedTimeMillis() - lastPingTime) + " ms LEFT";
        
        if (APP_UNKNOWN == status && limitTimeMillis < ofGetElapsedTimeMillis() - lastPingTime)
        {
            bStartWatching = false;
            status = APP_DEAD;
            logString = "STATUS : " + ofToString(status) + "\nTARGET APPLICATION SEEMES DEAD, TRY KILLALL.";
            
            string killcmd = "killall -9 " + appName;
            int ret = system(killcmd.c_str());
            
            logString = "STATUS : " + ofToString(status) + "\nFINISHED TO KILL, RETURNING VALUE = " + ofToString(ret) + "\nTRY TO REBOOT AFTER " + ofToString(rebootInterval) + " ms";
            
            ofSleepMillis(rebootInterval);
            
            status = APP_REBOOTING;
            lastPingTime = ofGetElapsedTimeMillis();
            
            logString = "STATUS : " + ofToString(status) + "\nTRYING TO REBOOT...";
            
            string rebootcmd = "open " + appPath;
            ret = system(rebootcmd.c_str());
            
            logString = "STATUS : " + ofToString(status) + "\nFINISHED TO REBOOT, RETURNING VALUE = " + ofToString(ret);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackgroundHex(0);
    if (APP_DEAD == status)
    {
        ofSetColor(255, 0, 0);
    }
    else if (APP_ALIVE == status)
    {
        ofSetColor(0, 255, 0);
    }
    else if (APP_REBOOTING == status)
    {
        ofSetColor(255, 255, 0);
    }
    else
    {
        ofSetColor(157);
    }
    ofDrawBitmapString(logString, 70, 25);
    ofRect(10, 10, 50, 50);
}

void ofApp::ping()
{
    ofxOscMessage msg;
    msg.setAddress(remotePingAddr);
    msg.addStringArg(ofGetTimestampString());
    sender.sendMessage(msg);
    
    status = APP_UNKNOWN;
}

void ofApp::onMessageReceived(ofxOscMessage &msg)
{
    string address = msg.getAddress();
    if (incomingPingAddr == address)
    {
        status = APP_ALIVE;
        lastPingTime = ofGetElapsedTimeMillis();
        lastPingTimestamp = ofGetTimestampString();
    }
    else if (bootAddr == address)
    {
        status = APP_ALIVE;
        bStartWatching = true;
    }
}

void ofApp::onTimerReached(ofEventArgs &e)
{
    if (bStartWatching) ping();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (' ' == key) bStartWatching = !bStartWatching;
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