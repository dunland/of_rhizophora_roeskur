#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{

    // general app setup:
    ofSetFrameRate(30);

    // Communication
    receiver.setup(PORT);

    // GUI:
    gui.setup();
    gui.add(minArea.set("Min area", 10, 1, 100));
    gui.add(maxArea.set("Max area", 200, 1, 500));
    gui.add(threshold.set("Threshold", 128, 0, THRESHOLD_MAX));
    gui.add(holes.set("Holes", false));

    // video setup
    ofDirectory dir("video"); // read folder ./bin/data
    dir.allowExt("mp4");
    dir.listDir();
    dir = dir.getSorted();
    for (int i = 0; i < dir.size(); i++)
    {
        videoPaths.push_back(dir.getPath(i)); // TODO: alle Bilder anfangs laden!
        video.load(dir.getPath(i));
    }

    video.play();
}

//--------------------------------------------------------------
void ofApp::update()
{

    // Video ---------------------------------------------------
    video.update();
    if (video.isFrameNew())
    {
        ofPixels &pixels = video.getPixels();
        contourFinder.setMinAreaRadius(minArea);
        contourFinder.setMaxAreaRadius(maxArea);
        contourFinder.setThreshold(threshold);
        contourFinder.findContours(pixels);
        contourFinder.setFindHoles(holes);
    }

    // OSC -----------------------------------------------------
    // check for waiting messages
    while (receiver.hasWaitingMessages())
    {

        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);

        // handle messages:
        if (m.getAddress() == "/kick")
        {
            ofLogNotice(m.getAddress());
            if (threshold > THRESHOLD_MIN)
                threshold -= THRESHOLD_STEP;
            else
                threshold = THRESHOLD_MIN;
            ofLogNotice("threshold: " + ofToString(threshold));
        }
        else if (m.getAddress() == "/snare")
        {
            maxArea = 35;
            minArea = 0;
        }
        else if (m.getAddress() == "/bp/90")
        {
            video_r = int(m.getArgAsFloat(0) * 25500);
            ofLogNotice("video_r:" + ofToString(video_r));
        }
        else if (m.getAddress() == "/bp/1250")
        {
            video_g = int(m.getArgAsFloat(0) * 25500);
            ofLogNotice("video_g" + ofToString(video_g));
        }
        else if (m.getAddress() == "/bp/5000")
        {
            video_b = int(m.getArgAsFloat(0) * 25500);
            ofLogNotice(ofToString(video_b));
        }
        else
        {

            // unrecognized message: display on the bottom of the screen
            string msgString;
            msgString = m.getAddress();
            msgString += ":";
            for (size_t i = 0; i < m.getNumArgs(); i++)
            {

                // get the argument type
                msgString += " ";
                msgString += m.getArgTypeName(i);
                msgString += ":";

                // display the argument - make sure we get the right type
                if (m.getArgType(i) == OFXOSC_TYPE_INT32)
                {
                    msgString += ofToString(m.getArgAsInt32(i));
                }
                else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT)
                {
                    msgString += ofToString(m.getArgAsFloat(i));
                }
                else if (m.getArgType(i) == OFXOSC_TYPE_STRING)
                {
                    msgString += m.getArgAsString(i);
                }
                else
                {
                    msgString += "unhandled argument type " + m.getArgTypeName(i);
                }
            }

            // add to the list of strings to display
            msgStrings[currentMsgString] = msgString;
            timers[currentMsgString] = ofGetElapsedTimef() + 5.0f;
            currentMsgString = (currentMsgString + 1) % NUM_MSG_STRINGS;

            // clear the next line
            msgStrings[currentMsgString] = "";
        }
    }

    // cooldown/update contour variables:
    if (minArea < 50)
        minArea += MIN_AREA_RELEASE;
    if (maxArea < 500)
        maxArea += MAX_AREA_RELEASE;
    threshold = (threshold < THRESHOLD_MAX) ? threshold + THRESHOLD_RELEASE : THRESHOLD_MAX;
    video_r = (video_r > 0) ? video_r - 1 : 0;
    video_g = (video_g > 0) ? video_g - 1 : 0;
    video_b = (video_b > 0) ? video_b - 1 : 0;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(255);

    // Video: --------------------------------------------------
    ofSetColor(video_r, video_g, video_b);
    // ofSetColor(255);
    video.draw(0, 0);

    // contours
    ofSetColor(255-video_r, 255-video_g, 255-video_b);
    ofFill();
    contourFinder.draw();
    ofNoFill();

    if (video.getIsMovieDone())
    {
        ofSetHexColor(0xFF0000);
        ofDrawBitmapString("end of movie", 20, 440);
    }

    // Communication -------------------------------------------
    if (globalVerboseLevel > 0)
    {

        // draw recent unrecognized messages
        for (int i = 0; i < NUM_MSG_STRINGS; i++)
        {
            ofDrawBitmapStringHighlight(msgStrings[i], 10, 40 + 15 * i);
        }

        string buf = "listening for osc messages on port " + ofToString(PORT);
        ofDrawBitmapStringHighlight(buf, 10, 20);
    }

    // Controls ------------------------------------------------
    if (bDrawGui)
        gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    switch (key)
    {
    case 'f':
        ofToggleFullscreen();
        break;
    case 'g':
        bDrawGui = !bDrawGui;
        break;
    case 'r':
        video.setFrame(ofRandom(video.getTotalNumFrames()));
        break;
    case 'v':
        globalVerboseLevel++;
        ofLogNotice("globalVerboseLevel = " + globalVerboseLevel);
        break;
    case 'V':
        globalVerboseLevel--;
        ofLogNotice("globalVerboseLevel = " + globalVerboseLevel);
        break;

    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
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
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
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