#include "KeyControl.hpp"


KeyControl::KeyControl(void) {
	speed = 0.1;
	r = 18.0; // Sollte < 1.0 sein!
	y = 0.0f;
	rotate = 0.0;
	lightY = 6.0;
	camPos = vec3(sin(rotate), y, cos(rotate));
}


KeyControl::~KeyControl(void) {
}

/**
* Siehe Kommentar Klasse KeyControl
*/
void KeyControl::pressUP() {
	if(r > 1.0) {
		r -= speed;
	}
}

/**
* Siehe Kommentar Klasse KeyControl
*/
void KeyControl::pressDOWN() {
	r += speed;
}

/**
* Siehe Kommentar Klasse KeyControl
*/
void KeyControl::press1() {
	lightY -= speed;
}

/**
* Siehe Kommentar Klasse KeyControl
*/
void KeyControl::press2() {
	lightY += speed;
}

/**
* Siehe Kommentar Klasse KeyControl
*/
void KeyControl::pressLEFT() {
	rotate -= speed;
	camPos = vec3(sin(rotate), y, cos(rotate));
}

/**
* Siehe Kommentar Klasse KeyControl
*/
void KeyControl::pressRIGHT() {
	rotate += 0.3;
	camPos = vec3(sin(rotate), y, cos(rotate));
}

/**
* Siehe Kommentar Klasse KeyControl
*/
void KeyControl::pressR() {
	r = 18.0;
	y = 0.0f;
	lightY = 6.0;
	rotate = 0.0;
	camPos = vec3(sin(rotate), y, cos(rotate));
}

/**
* Parameter vec3 fuer Methode
* setCamPos( ... ) in Klasse SceneControl
*/
vec3 KeyControl::getCamPos() {
	return camPos * r;
}

/**
* Parameter float y fuer Methode
* setLightPos( ... , float y, ... ) in Klasse SceneControl
*/
float KeyControl::getLightY() {
	return lightY;
}