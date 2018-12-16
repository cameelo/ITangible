#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	learnBg = false;
	start = false;
	cam.setDeviceID(1);
	//cam.initGrabber(320, 240, false);
	cam.setup(320, 240);
	color.allocate(320, 240);
	gray.allocate(320, 240);
	grayBg.allocate(320, 240);
	grayDiff.allocate(320, 240);
	thres = 30;
	notas = new std::vector<StoredNote>();
	
	app_life_span = 0;

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
	if (start) {

		bool red_found = false;
		bool blue_found = false;
		bool green_found = false;

		if (app_life_span == 10) {
			//reseteo los desplazamientos y actualizo las velocidades
			for (int i = 0; i < 3; i++) {
				if(balls[i] != NULL)
					balls[i]->update_v();
			}
			app_life_span = 0;
		}

		now = ofGetElapsedTimef();

		for (int i = 0; i < notas->size(); i++) {
			if (now - (*notas)[i].creationTime > (*notas)[i].timeToLive) {
				midiOut.sendNoteOff((*notas)[i].channel, (*notas)[i].note, 64);
				cout << "note off" << endl;
				notas->erase(notas->begin() + i);
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
			/*if (manager.frameNewBlobs->size() > 0) {
				ofColor c = getProm(*((*manager.frameNewBlobs)[0]));
				if (isRed(c)) {
					midiOut.sendNoteOn(1, 39, 64);
					notas->push_back(StoredNote(ofGetElapsedTimef(), 1, 1, 100));
				}
			}
			if (manager.frameRemovedBlobs->size() > 0) {
				midiOut.sendNoteOn(1, 29, 64);
				notas->push_back(StoredNote(ofGetElapsedTimef(), 1, 1, 50));
			}*/
			if (contourFinder.nBlobs > 0) {

				if (manager.blobs.size() > 0) {
					for (int i = 0; i < manager.blobs.size(); i++) {
						ofColor blobColor = getProm(manager.blobs[i]);
						//RED BALL PROCESS
						if (isRed(blobColor) ) {
							if (!red_found) {
								if (balls[0] == NULL) {
									balls[0] = new Ball(manager.blobs[i], Color(0));
								}
								else {
									//actualizo su datos
									balls[0]->update_displacement(manager.blobs[i].centroid.x, manager.blobs[i].centroid.y);
									balls[0]->sendMidi(midiOut, notas);
								}
								balls[0]->lived();
							}
							red_found = true;

						}else if (isBlue(blobColor)) {
							if (!blue_found) {
								if (balls[1] == NULL) {
									balls[1] = new Ball(manager.blobs[i], Color(0));
								}
								else {
									//actualizo su datos
									balls[1]->update_displacement(manager.blobs[i].centroid.x, manager.blobs[i].centroid.y);
									balls[1]->sendMidi(midiOut, notas);
								}
								balls[1]->lived();
								
							}
							blue_found = true;
						}
						else if (isGreen(blobColor)) {
							if (!green_found) {
								if (balls[2] == NULL) {
									balls[2] = new Ball(manager.blobs[i], Color(0));
								}
								else {
									//actualizo su datos
									balls[2]->update_displacement(manager.blobs[i].centroid.x, manager.blobs[i].centroid.y);
									balls[2]->sendMidi(midiOut, notas);
								}
								balls[2]->lived();

							}
							green_found = true;
						}
					}

					if (balls[0] != NULL && !red_found) {
						delete(balls[0]);
						balls[0] = NULL;
					}
					if (balls[1] != NULL && !blue_found) {
						delete(balls[1]);
						balls[1] = NULL;
					}
					if (balls[2] != NULL && !green_found) {
						delete(balls[2]);
						balls[2] = NULL;
					}
				}
			}
		}
		app_life_span++;
		
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
	if (0 <= red && red <= 255 && 0 <= green && green <= 140 && 0 <= blue && blue <= 140)
		return true;

	return false;
}

bool ofApp::isBlue(ofColor color) {
	//asumimos tolerancia de 10% (25 en cada coordenada)
	//como rojo puro es 255:
	int red = color.r;
	int green = color.g;
	int blue = color.b;
	//TODO: SET BLUE PARAMS
	if (0 <= red && red <= 60 && 0 <= green && green <= 120 && 0 <= blue && blue <= 255)
		return true;

	return false;
}

bool ofApp::isGreen(ofColor color) {
	//asumimos tolerancia de 10% (25 en cada coordenada)
	//como rojo puro es 255:
	int red = color.r;
	int green = color.g;
	int blue = color.b;
	//TODO: SET GREEN PARAMS
	if (0 <= red && red <= 120 && 0 <= green && green <= 255 && 0 <= blue && blue <= 170)
		return true;

	return false;
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (!start) {
		cam.draw(0, 0);
	}
	else{
		ofSetColor(255, 255, 255);
		color.draw(0, 0, 320, 240);
		gray.draw(320, 0, 320, 240);
		grayDiff.draw(640, 0, 320, 240);
		contourFinder.draw(320, 240, 320, 240);
		//manager.debugDraw(320, 240, cam.getWidth(), cam.getHeight(), 320, 240);

		///// DIBUJAR VALOR DE VARIABLES
		ofSetColor(0, 0, 0);
		ofDrawBitmapString(string("Threshold: ").append(std::to_string(thres)), 100, 500);
		string blobs = "Blobs: ";
		for (int i = 0; i < manager.blobs.size(); i++) {
			blobs.append(std::to_string(manager.blobs[i].id));
			blobs.append("; ");
		}
		ofDrawBitmapString(blobs, 100, 515);

		if (balls[0] != NULL) {
			ofDrawBitmapString("vida pelota roja: " + ofToString(balls[0]->life_span), 100, 555);
			ofDrawBitmapString("velocidad pelota roja: " + ofToString(balls[0]->v_module), 100, 595);
			ofDrawBitmapString("centro pelota roja: " + ofToString(balls[0]->blob.centroid.x) + "," + ofToString(balls[0]->blob.centroid.y), 100, 615);

		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_BACKSPACE) {
		learnBg = true;
		start = true;
	}

	if (key == 'a') {
		thres -= 10;
	}
	
	if (key == 's') {
		thres += 10;
	}

	if (key == 'z') {
		midiOut.sendControlChange(1,1,100);
	}

	if (key == 'x') {
		midiOut.sendControlChange(2, 1, 100);
	}

	if (key == 'c') {
		midiOut.sendControlChange(3, 1, 100);
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
