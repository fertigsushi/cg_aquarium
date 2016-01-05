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
	MoveControl(float xLimMin, float xLimMax, float yLim, float zLim, float zRotLim);
	MoveControl(float limit);
	~MoveControl();
	void moveX();
	void moveY();
	void moveZ();
	void rotateY();
	float getX();
	float getY();
	float getZ();
	float getRotateY();
	bool getIsWiggleLeft();

private:
	void setX();
	void setIsWiggleLeft();
	void setY();
	void setIsWiggleDown();
	void setZ();
	void setIsWiggleDepth();
	void setRotateY();
	void setIsWiggleRotateLeft();

	float x;
	float y;
	float z;
	float yRotate;
	float xLimitMin;
	float xLimitMax;
	float yLimit;
	float zLimit;
	float yRotateLim;
	float xSpeed;
	float ySpeed;
	float zSpeed;
	float yRotateSpeed;
	bool isWiggleLeft; // x
	bool isWiggleDown; // y
	bool isWiggleDepth; // z
	bool isWiggleRotateLeft; // zRotate
};