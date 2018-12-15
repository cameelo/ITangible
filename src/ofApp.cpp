#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	learnBg = false;
	//cam.setup(320, 240);
	cam.setDeviceID(0);
	cam.initGrabber(320, 240, false);
	color.allocate(320, 240);
	gray.allocate(320, 240);
	grayBg.allocate(320, 240);
	grayDiff.allocate(320, 240);
	thres = 30;
	//manager = ofxBlobsManager();


	///// MIDI SECTION

	// print the available output ports to the console
	midiOut.listOutPorts();

	// connect
	midiOut.openPort(1); // by number
						 //midiOut.openPort("IAC Driver Pure Data In"); // by name
						 //midiOut.openVirtualPort("ofxMidiOut"); // open a virtual port

	channel = 1;
	currentPgm = 0;
	note = 0;
	velocity = 0;
	pan = 0;
	bend = 0;
	touch = 0;
	polytouch = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	cam.update();
	//cout << thres << endl;

	now = ofGetElapsedTimef();

	for (int i = 0; i < notas.size();i++) {
		if (now - notas[i].creationTime > notas[i].timeToLive) {
			midiOut.sendNoteOff(notas[i].channel, notas[i].note, 64);
			cout << "note off" << endl;
			notas.erase(notas.begin() + i);
		}
	}
	
	if (cam.isFrameNew()) {
		colorPixels = cam.getPixels();
		color.setFromPixels(colorPixels);
		gray = color;
		if (learnBg) {
			grayBg = color;
			learnBg = false;
		}
		grayDiff.absDiff(grayBg, gray);
		grayDiff.threshold(thres);
		contourFinder.findContours(grayDiff, 5, (340 * 240) / 4, 4, false, true);
		manager.update(contourFinder.blobs);
		if(manager.frameNewBlobs->size() > 0) {
			midiOut.sendNoteOn(1, 100, 64);
			notas.push_back(StoredNote(ofGetElapsedTimef(), 1, 1, 100));
		}
		if (manager.frameRemovedBlobs->size() > 0) {
			midiOut.sendNoteOn(1, 50, 64);
			notas.push_back(StoredNote(ofGetElapsedTimef(), 1, 1, 50));
		}
		if (contourFinder.nBlobs > 0) {

			if (manager.blobs.size() > 0) {
				for (int i = 0; i < manager.blobs.size(); i++) {
					ofColor blobColor = getProm(manager.blobs[i]);
					if (isRed(blobColor)) {
						redBlob = new Ball(manager.blobs[i], Color(0));
					};
				}
			}
		}
	}
}

ofColor ofApp::getProm(ofxCvBlob blob) {
	ofColor ret;
	ofColor c1 = colorPixels.getColor(blob.centroid.x, blob.centroid.y);
	ofColor c2;
	ofColor c3;
	ofColor c4;

	if (!color.height < blob.centroid.x + 1)
		c2 = colorPixels.getColor(blob.centroid.x + 1, blob.centroid.y);
	else
		c2 = colorPixels.getColor(blob.centroid.x, blob.centroid.y);

	if (!color.height < blob.centroid.y + 1)
		c3 = colorPixels.getColor(blob.centroid.x, blob.centroid.y + 1);
	else
		c3 = colorPixels.getColor(blob.centroid.x, blob.centroid.y);

	if (!color.height < blob.centroid.y + 1 && !color.height < blob.centroid.x + 1)
		c4 = colorPixels.getColor(blob.centroid.x + 1, blob.centroid.y + 1);
	else
		c4 = colorPixels.getColor(blob.centroid.x, blob.centroid.y);

	ret.r = ((c1.r + c2.r + c3.r + c4.r) / 4);
	ret.g = ((c1.g + c2.g + c3.g + c4.g) / 4);
	ret.b = ((c1.b + c2.b + c3.b + c4.b) / 4);

	return ret;
}


bool ofApp::isRed(ofColor color) {
	//asumimos tolerancia de 10% (25 en cada coordenada)
	//como rojo puro es 255:
	int red = color.r;
	int green = color.g;
	int blue = color.b;
	if (100 <= red && red <= 230 && 0 <= green && green <= 25 && 0 <= blue && blue <= 25)
		return true;

	return false;
}

//--------------------------------------------------------------
void ofApp::draw(){
	//cam.draw(0, 0);
	color.draw(0, 0,320,240);
	gray.draw(320, 0, 320, 240);
	grayDiff.draw(640, 0, 320, 240);
	contourFinder.draw(320, 240, 320, 240);
	//manager.debugDraw(320, 240, cam.getWidth(), cam.getHeight(), 320, 240);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_BACKSPACE) {
		learnBg = true;
	}

	if (key == 'a') {
		thres -= 10;
	}
	
	if (key == 's') {
		thres += 10;
	}

	// send a note on if the key is a letter or a number
	if (key == 'b') {

		// scale the ascii values to midi velocity range 0-127
		// see an ascii table: http://www.asciitable.com/
		note = ofMap(key, 48, 122, 0, 127);
		velocity = 64;
		midiOut.sendNoteOn(channel, note, velocity);

		//midiOut.sendControlChange()

		// print out both the midi note and the frequency
		//ofLogNotice() << "note: " << note
		//	<< " freq: " << ofxMidi::mtof(note) << " Hz";
	}
	if (key == 'z') {
		midiOut.sendControlChange(1,1,100);
	}

	if (key == 'x') {
		midiOut.sendControlChange(2, 1, 100);
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
