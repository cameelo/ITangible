#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOpenCv.h"
#include "ofxBlobsManager\src\ofxBlobsManager.h"
#include "ofxBlobsManager\src\ofxStoredBlobVO.h"
#include "StoredNote.h"
#include "Ball.h"

//enum Colors {
//	Red,
//	Green,
//	Blue
//};

//class Ball {
//private:
//	ofxCvBlob blob;
//public:
//	Ball();
//	float getSpeed(ofxCvBlob);
//	~Ball();
//};


class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		const int ballsCount = 5;

		bool learnBg;
		ofVideoGrabber cam;
		ofxCvColorImage color;
		ofxCvGrayscaleImage gray, grayBg, grayDiff;
		ofxCvContourFinder contourFinder;

		ofxBlobsManager manager;
		ofPixels colorPixels;
		ofColor c;
		int thres;
		//Ball balls[5];

		////// Notas reproducidas
		std::vector<StoredNote> notas;

		///// Tiempo
		float now;

		////// MIDI SECITON

		ofxMidiOut midiOut;
		int channel;

		unsigned int currentPgm;
		int note, velocity;
		int pan, bend, touch, polytouch;

		//// COLOR
		Ball* redBlob;
		bool isRed(ofColor color);

		ofColor getProm(ofxCvBlob blob);
};
