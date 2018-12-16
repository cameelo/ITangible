#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxBlobsManager\src\ofxBlobsManager.h"
#include "ofxBlobsManager\src\ofxStoredBlobVO.h"
#include "ofxMidi.h"
#include "StoredNote.h"

enum Color {
	Red,
	Green,
	Blue,
	None
};

class Ball {
private:
	Color color;
	int displacement;
	int centerX;
	int centerY;
public:
	ofxCvBlob blob;
	int v_module;
	int life_span;
	Color getColor();
	Ball();
	Ball(ofxCvBlob blob, Color color);
	float getSpeed();
	void update_displacement(int x, int y);
	void update_v();
	void lived();
	~Ball();
	void sendMidi(ofxMidiOut midi, std::vector<StoredNote>* notas);
};