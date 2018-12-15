#include "StoredNote.h"

StoredNote::StoredNote(float t, float timeToLive, int channel, int note) {
	this->creationTime = t;
	this->timeToLive = timeToLive;
	this->channel = channel;
	this->note = note;
}

StoredNote::~StoredNote() {

}