#pragma once

#include "ofxCvBlob.h"

class Ball {
private:
	ofxCvBlob blob;
public:
	Ball();
	float getSpeed(ofxCvBlob);
	~Ball();
};