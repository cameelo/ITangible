#include "Ball.h"
#include <math.h>

Ball::Ball() {
	this->life_span = 0;
	this->v_module = 0;
}

Ball::Ball(ofxCvBlob blob, Color color) {
	this->blob = blob;
	this->color = color;
	this->life_span = 0;
	this->v_module = 0;
	this->displacement = 0;
	this->centerX = blob.centroid.x;
	this->centerY = blob.centroid.y;
}

Color Ball::getColor() {
	return this->color;
}

float Ball::getSpeed() {
	return 0;
}
void Ball::update_displacement(int x, int y)
{
	this->displacement = (this->displacement + sqrt (pow((this->centerX - x),2) + pow((this->centerY - y), 2)) );
	this->centerX = x;
	this->centerY = y;
	
}
void Ball::update_v()
{
	this->v_module = this->displacement / 10;
	this->displacement = 0;
}
void Ball::lived()
{
	this->life_span = this->life_span++;
}
Ball::~Ball() {

}

void Ball::sendMidi(ofxMidiOut midi, std::vector<StoredNote>* notas)
{
	//PIANO CANAL 1 ROJO
	if (this->color == 0) {
		if (this->life_span % 10 == 0) {
			int nota = 29 + 7 * this->v_module;
			midi.sendNoteOn(1, nota, 64);
			notas->push_back(StoredNote(ofGetElapsedTimef(), 1, 1, 50));
			midi.sendControlChange(1, 1, this->centerX + this->centerY + 1);
		}
	} else if (this->color == 1) { //azul
		//TODO: TIRAR FRUTA EN AZUL
		if (this->life_span % 12 == 0) {
			int nota = 33 + 3 * this->v_module;
			midi.sendNoteOn(1, nota, 64);
			notas->push_back(StoredNote(ofGetElapsedTimef(), 1, 1, 50));
			midi.sendControlChange(1, 2, this->centerX + 1);
		}
	}
	else if (this->color == 2) { //verde
								 //TODO: TIRAR FRUTA EN VERDE
		if (this->life_span % 14 == 0) {
			int nota = 36 + 5 * this->v_module;
			midi.sendNoteOn(1, nota, 64);
			notas->push_back(StoredNote(ofGetElapsedTimef(), 1, 1, 50));
			midi.sendControlChange(1, 3, this->centerY + 1);
		}
	}
}
