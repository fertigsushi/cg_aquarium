#include "MoveControl.hpp"

/**
* zB fuer Pflanzen
*/
MoveControl::MoveControl(float limit) {
	this->limit = limit;
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

void MoveControl::setX() {
	if (isWiggleLeft) {
		x -= xSpeed;
	} else {
		x += xSpeed;
	}
}

void MoveControl::setIsWiggleLeft() {
	if (x > limit && !isWiggleLeft) {
		isWiggleLeft = true;
	} else if (x < limit && isWiggleLeft) {
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
	if (y > limit && !isWiggleDown) {
		isWiggleDown = true;
	} else if (y < -limit && isWiggleDown) {
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
	if (z > limit && !isWiggleDepth) {
		isWiggleDepth = true;
	} else if (z < -limit && isWiggleDepth) {
		isWiggleDepth = false;
	}
}