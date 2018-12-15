#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBlobsManager\src\ofxBlobsManager.h"
#include "ofxBlobsManager\src\ofxStoredBlobVO.h"

enum Color {
	Red,
	Green,
	Blue,
	None
};

class Ball {
private:
	ofxCvBlob blob;
	Color color;
public:
	Color getColor();
	Ball();
	Ball(ofxCvBlob blob, Color color);
	float getSpeed();
	~Ball();
};