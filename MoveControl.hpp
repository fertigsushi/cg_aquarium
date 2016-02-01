#ifndef MOVECONTROL_H
#define MOVECONTROL_H

#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

/**
* Klasse zum Bewegen bzw. Rotieren von Obj die
* der Klassen Token ins Programm geladen worden.
* Die Getter sind die Uebergabeparmeter fuer die wiggle Methoden
* in Klasse Token.
* void Methoden werden in der while-Schleife in der Main
* ausgefuehrt.
* Es kann fuer alle sich zu bewegenden Obj eine Instanz erzeugt werden oder mehrere
* Instanzen fur unterschiedliche Obj.
*/
class MoveControl {

public:
	MoveControl(float limit);
	void moveX();
	void moveY();
	void moveZ();
	float getX();
	float getY();
	float getZ();

private:
	void setX();
	void setIsWiggleLeft();
	void setY();
	void setIsWiggleDown();
	void setZ();
	void setIsWiggleDepth();

	float limit;
	float x;
	float y;
	float z;
	float yRotate;
	float xSpeed;
	float ySpeed;
	float zSpeed;
	bool isWiggleLeft; // x
	bool isWiggleDown; // y
	bool isWiggleDepth; // z
};

#endif // MOVECONTROL_H