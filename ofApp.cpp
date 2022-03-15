#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetColor(0);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 100, true, true, true);

	for (char c = 'A'; c <= 'Z'; c++) {

		this->chara_path.push_back(font.getCharacterAsPoints(c, true, false));
	}

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->fbo.begin();
	ofClear(0);
	ofSetColor(0);

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	ofDrawCircle(glm::vec2(), 300);

	this->fbo.end();
	this->fbo.readToPixels(this->pix);
}

//--------------------------------------------------------------
void ofApp::draw() {

	vector<glm::vec2> location_list;
	vector<float> radius_list;
	vector<int> chara_index_list;

	while (location_list.size() < 250) {

		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());

		ofColor pix_color = this->pix.getColor(x, y);
		if (pix_color != ofColor(0)) {

			continue;
		}

		glm::vec2 location = glm::vec2(x, y);
		float radius = ofRandom(8, 40);

		bool flag = true;
		for (int i = 0; i < location_list.size(); i++) {

			if (glm::distance(location, location_list[i]) < radius_list[i] + radius) {

				flag = false;
				break;
			}
		}

		if (flag) {

			location_list.push_back(location);
			radius_list.push_back(radius);
			chara_index_list.push_back(ofRandom(this->chara_path.size()));
		}
	}

	for (int i = 0; i < location_list.size(); i++) {

		ofPushMatrix();
		ofTranslate(location_list[i]);
		ofRotate(ofRandom(360) + ofGetFrameNum() * ofRandom(-6, 6));

		int chara_index = chara_index_list[i];
		chara_index = (int)(chara_index + ofMap(ofNoise(glm::vec3(location_list[i] * 0.005, ofGetFrameNum() * 0.00015)), 0, 1, 0, chara_path.size())) % chara_path.size();

		auto font_size = radius_list[i] * 1.5;
		auto outline = this->chara_path[chara_index].getOutline();

		ofBeginShape();
		for (int line_index = 0; line_index < outline.size(); line_index++) {

			if (line_index != 0) { ofNextContour(true); }

			auto vertices = outline[line_index].getVertices();
			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				auto point = (vertices[vertices_index] / 100 * font_size) + glm::vec2(font_size * -0.5, font_size * 0.5);
				ofVertex(point);
			}
		}
		ofEndShape(true);

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}