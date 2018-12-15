#pragma once

class StoredNote {
public:

	float creationTime;
	int note;
	float timeToLive;
	int channel;

	StoredNote(float t, float timeToLive, int channel, int note);
	~StoredNote();
};