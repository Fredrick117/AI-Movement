#include "ofApp.h"
#include "Boid.h"

float timeBetweenBreadcrumbs = 0.25f;
float breadcrumbTimer;

vector<ofVec2f> breadcrumbPositions;

vector<Boid*> boids;

//--------------------------------------------------------------
void ofApp::setup(){

	for (int i = 0; i < 25; i++)
	{
		Boid* boid = new Boid();

		if (i == 0)
		{
			boid->isFlockLeader = true;
			boid->behaviorType = WANDER2;
		}
		else
		{
			boid->isFlockLeader = false;
			boid->behaviorType = FLOCK;
		}

		boid->movementIsKinematic = false;

		boid->rb.position = { ofRandom(100), ofRandom(700) };

		boids.push_back(boid);
	}

	breadcrumbTimer = timeBetweenBreadcrumbs;

}

//--------------------------------------------------------------
void ofApp::update(){

	float dt = ofGetLastFrameTime();

	breadcrumbTimer -= dt;

	if (breadcrumbTimer <= 0)
	{
		//breadcrumbPositions.push_back(boid1.rb.position);
		//breadcrumbPositions.push_back(boid2.rb.position);
		breadcrumbTimer = timeBetweenBreadcrumbs;
	}

	for (Boid* boid : boids)
	{
		SteeringOutput steering;
		boid->Update(dt, steering, boids);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	ofSetColor(255, 255, 255);

	for (ofVec2f breadcrumb : breadcrumbPositions)
	{
		ofDrawCircle(breadcrumb, 2);
	}

	for (Boid* boid : boids)
	{
		if (boid->isFlockLeader)
			ofSetColor(255, 0, 0);
		
		boid->Draw();
		ofSetColor(255, 255, 255);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
	for (Boid* boid : boids)
	{
		if (boid->behaviorType == SEEK || boid->behaviorType == ARRIVE || boid->behaviorType == ARRIVE2)
		{
			ofVec2f newTarget(x, y);
			boid->target = newTarget;
		}
	}
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

//--------------------------------------------------------------
void ofApp::exit(){
	// TODO: free memory
}
