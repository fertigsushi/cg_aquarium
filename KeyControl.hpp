
#include <math.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

/**
* Fuer Tasten steuerung
* void Methoden werden in
* in der Methode key_callback( ... )
* ausgefuehrt, fuer die jeweilige Taste.
*/
class KeyControl { 

public:
	KeyControl();
	~KeyControl();
	void pressUP();
	void pressDOWN();
	void press1();
	void press2();
	void pressLEFT();
	void pressRIGHT();
	void pressR();
	vec3 getCamPos();
	float getLightY();

private:
	vec3 camPos;
	float r;
	float rotate;
	float y;
	float speed;
	float lightY;
};
