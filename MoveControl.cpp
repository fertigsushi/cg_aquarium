#include "MoveControl.hpp"

/**
* zB fuer Fische
*/
MoveControl::MoveControl(float xLimMin, float xLimMax, float yLim, float zLim, float zRotLim) {
	xLimitMin = xLimMin;
	xLimitMax = xLimMax;
	yLimit = yLim;
	zLimit = zLim;
	yRotateLim = zRotLim;
	x = 0.0;
	y = 0.0;
	z = 0.0;
	yRotate = 0.0;
	xSpeed = 0.006;
	ySpeed = 0.005;
	zSpeed = 0.005;
	yRotateSpeed = 0.2;
	isWiggleLeft = false; // x
	isWiggleDown = false; // y
	isWiggleDepth = false; // z
	isWiggleRotateLeft = false; // zRotate
}

/**
* zB fuer Pflanzen
*/
MoveControl::MoveControl(float limit) {
	xLimitMin = limit;
	xLimitMax = limit;
	yLimit = limit;
	zLimit = limit;
	x = 0.0;
	y = 0.0;
	z = 0.0;
	xSpeed = 0.008;
	ySpeed = 0.008;
	zSpeed = 0.004;
	isWiggleLeft = false; // x
	isWiggleDown = false; // y
	isWiggleDepth = false; // z
}

MoveControl::~MoveControl(void)
{
}

/**
* Bewegt (translate) ein Obj (Hier Achse X) von einem Startwert 
* zu Max-Wert auf der jeweiligen Achse.
* Dann automatisch von dem Maxwert zu dem Minwert auf der
* jeweiligen Achse.
* Der Min-Wert kann <, > sein als der Starwert oder gleich sein.
* Min-Wert, Max-Wert und Startwert sind Variabel.
* Die Methoden arbeiten nut mit Klassen Attributen und mussen daher
* unbedingt im Klassen Konstruktor deklariert werden!!!
*/
void MoveControl::moveX() {
	setX();
	setIsWiggleLeft();
}

/**
* Siehe Kommentar fuer moveX()
* Hinweis: Anders als moveX() ist der
* Min-Wert := -Max-Wert weil X die Hauptachse ist.
*/
void MoveControl::moveY() {
	setY();
	setIsWiggleDown();
}

/**
* Siehe Kommentar fuer moveX()
* Hinweis: Anders als moveX() ist der
* Min-Wert := -Max-Wert weil X die Hauptachse ist.
*/
void MoveControl::moveZ() {
	setZ();
	setIsWiggleDepth();
}

/**
* Siehe Kommentar fuer moveX()
* Hinweis1: Anders als moveX() ist der
* Min-Wert := -Max-Wert weil X die Hauptachse ist.
* Hinweis2: Ist fuer das Rotieren auf der Y Achse gedacht nicht
* fuer das bewegen. (translate)
*/
void MoveControl::rotateY() {
	setRotateY();
	setIsWiggleRotateLeft();
}

/**
* Uebergabeparameter fuer wiggle Methoden
* in der Klasse Token.
*/
float MoveControl::getX() {
	return x;
}

/**
* Siehe Kommentar fuer getX()
*/
float MoveControl::getY() {
	return y;
}

/**
* Siehe Kommentar fuer getX()
*/
float MoveControl::getZ() {
	return z;
}

/**
* Siehe Kommentar fuer getX()
* und rotateY().
*/
float MoveControl::getRotateY() {
	return yRotate;
}

/**
* Fuer if abfrage in der while-Schleife 
* in der Main, ob sich ein Obj auf der X Achse
* nach Rechts oder Links bewegt.
*/
bool MoveControl::getIsWiggleLeft() {
	return isWiggleLeft;
}

void MoveControl::setX() {
	if (isWiggleLeft) {
		x -= xSpeed;
	} else {
		x += xSpeed;
	}
}

void MoveControl::setIsWiggleLeft() {
	if (x > xLimitMax && !isWiggleLeft) {
		isWiggleLeft = true;
	} else if (x < xLimitMin && isWiggleLeft) {
		isWiggleLeft = false;
	}
}

void MoveControl::setY() {
	if (isWiggleDown) {
		y -= ySpeed;
	} else {
		y += ySpeed;
	}
}

void MoveControl::setIsWiggleDown() {
	if (y > yLimit && !isWiggleDown) {
		isWiggleDown = true;
	} else if (y < -yLimit && isWiggleDown) {
		isWiggleDown = false;
	}
}

void MoveControl::setZ() {
	if (isWiggleDepth) {
		z -= zSpeed;
	} else {
		z += zSpeed ;
	}
}

void MoveControl::setIsWiggleDepth() {
	if (z > zLimit && !isWiggleDepth) {
		isWiggleDepth = true;
	} else if (z < -zLimit && isWiggleDepth) {
		isWiggleDepth = false;
	}
}

void MoveControl::setRotateY() {
	if (isWiggleRotateLeft) {
		yRotate -= yRotateSpeed;
	} else {
		yRotate += yRotateSpeed;
	}
}

void MoveControl::setIsWiggleRotateLeft() {
	if (yRotate > yRotateLim && !isWiggleRotateLeft) {
		isWiggleRotateLeft = true;
	} else if (yRotate < -yRotateLim && isWiggleRotateLeft) {
		isWiggleRotateLeft = false;
	}
}