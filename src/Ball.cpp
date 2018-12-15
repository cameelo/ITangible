#include "Ball.h"

Ball::Ball() {

}

Ball::Ball(ofxCvBlob blob, Color color) {
	this->blob = blob;
	this->color = color;
}

Color Ball::getColor() {
	return this->color;
}

float Ball::getSpeed() {
	return 0;
}
Ball::~Ball() {

}