/**
* #################################################
* # @author Julian Osmer; s0542530@htw-berlin.de  #
* # Studiengang: Angewandte Informatik (B)        #
* # Modul: B43.1 Computergrafik [WS 15/16]        #
* # Beleg: Aquarium                               #
* #################################################
*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp> 
#include "shader.hpp"
#include "objects.hpp"
#include "objloader.hpp"
#include "texture.hpp"

using namespace glm;
using namespace std;

/**
* Klasse zum einlesen von Obj-Files und Texturen
* Hat versch. draw Methoden fuer Obj inkl.
* vorherige Model transformationen, rotationen.
* Beinhaltet die mat4 Model, MVP und die
* sendMVP() Methode fuer jedes Obj.
*/
class Token {

public:

	/*
	* Fuer nicht transparente Texturen
	*/
	Token(const char * path, const char * imagepath, GLuint progID) {
		path = path;
		imagepath = imagepath;
		programID = progID;
		Model = mat4(1.0f);
		VertexArrayIDObj = 0;
		res = loadOBJ(path, vertices, uvs, normals);		
		TexturObj = loadBMP_custom(imagepath);
		create();
	}

	/**
	* Fuer transparente Texturen
	*/
	Token(const char * path, const char * imagepath, bool isRGBA, GLuint progID) {
		path = path;
		imagepath = imagepath;
		programID = progID;
		Model = mat4(1.0f);
		VertexArrayIDObj = 0;
		res = loadOBJ(path, vertices, uvs, normals);		
		TexturObj = loadDDS(imagepath);
		create();
	}
	
	/**
	* Fuer statische Obj
	*/
	void draw(mat4& Projection , mat4& View ) {
		setModelToOrigin();
		sendMVP(Projection , View);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexturObj);
		glUniform1i(glGetUniformLocation(programID, "myTexturSampler"), 0);
		glBindVertexArray(VertexArrayIDObj);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}

	/**
	* Fuer statische transparente Obj
	*/
	void draw(bool isRGBA, mat4& Projection , mat4& View ) {
		setModelToOrigin();
		sendMVP(Projection , View );

        glEnable(GL_BLEND);     // Turn Blending On
        glDisable(GL_DEPTH_TEST);   // Turn Depth Testing Off
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexturObj);
		glUniform1i(glGetUniformLocation(programID, "myTexturSampler"), 0);
		glBindVertexArray(VertexArrayIDObj);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		
		glDisable(GL_BLEND);        // Turn Blending Off
        glEnable(GL_DEPTH_TEST);    // Turn Depth Testing On
	}

	/**
	* Fuer dynamische Obj, davor muss obj.wiggle( ... ) ausgefuehrt werden!
	*/
	void draw() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexturObj);
		glUniform1i(glGetUniformLocation(programID, "myTexturSampler"), 0);
		glBindVertexArray(VertexArrayIDObj);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}

	/**
	* Fuer Fische
	* Siehe Klasse MoveControl
	*/
	void wiggle(float x, float y, float z, float yRotate, mat4& Projection , mat4& View) {
		setModelToOrigin();
		wiggleRotate(yRotate);
		Model = translate(Model, vec3(x, y, z));
		sendMVP(Projection , View);
	}
	
	/**
	* Fuer Pflanzen
	* Siehe Klasse MoveControl
	*/
	void wiggle(float x, float y, float z, mat4& Projection , mat4& View) {
		setModelToOrigin();
		Model = rotate(Model, y, vec3(0.0, 1.0, 0.0));
		Model = rotate(Model, z, vec3(0.0, 0.0, 1.0));
		Model = rotate(Model, x, vec3(1.0, 0.0, 0.0));
		sendMVP(Projection , View);
	}
	
	/**
	* Getter fuer Model
	*/
	mat4 getModel() {
		return Model;
	}

private:

	void setModelToOrigin() {
		Model = mat4(1.0f);
	}

	void sendMVP(mat4& Projection , mat4& View ) {
		MVP = Projection * View * Model; 
		glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	}

	void wiggleRotate(float yRotate) {
		Model = rotate(Model, yRotate, vec3(0.0, 1.0, 0.0));
	}
	
	void create() {
		GLuint vertexbuffer;
		GLuint normalbuffer;
		GLuint uvsbuffer;

		glGenVertexArrays(1, &VertexArrayIDObj);
		glBindVertexArray(VertexArrayIDObj);

		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &uvsbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvsbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
	}
	
	mat4 MVP;
	mat4 Model;
	vector<vec3> vertices; // Buffer
	vector<vec2> uvs; // Buffer
	vector<vec3> normals; // Buffer
	GLuint programID;
	GLuint VertexArrayIDObj; // Obj
	GLuint TexturObj; // Textur
	const char * path; // Obj path
	const char * imagepath; // Textur path
	bool res; // TexturLoader loadDDS or loadBMP
};

/**
* Fuer Tasten steuerung
* void Methoden werden in
* in der Methode key_callback( ... )
* ausgefuehrt, fuer die jeweilige Taste.
*/
class KeyControl { 

public:
	
	KeyControl() {
		speed = 0.1;
		r = 18.0; // Sollte < 1.0 sein!
		y = 0.0f;
		rotate = 0.0;
		lightY = 6.0;
		camPos = vec3(sin(rotate), y, cos(rotate));
	}
	
	/**
	* Siehe Kommentar Klasse KeyControl
	*/
	void pressUP() {
		if(r > 1.0) {
		r -= speed;
		}
	}
	
	/**
	* Siehe Kommentar Klasse KeyControl
	*/
	void pressDOWN() {
		r += speed;
	}
	
	/**
	* Siehe Kommentar Klasse KeyControl
	*/
	void press1() {
		lightY -= speed;
	}
		
	/**
	* Siehe Kommentar Klasse KeyControl
	*/
	void press2() {
		lightY += speed;
	}
	
	/**
	* Siehe Kommentar Klasse KeyControl
	*/
	void pressLEFT() {
		rotate -= speed;
		camPos = vec3(sin(rotate), y, cos(rotate));
	}
	
	/**
	* Siehe Kommentar Klasse KeyControl
	*/
	void pressRIGHT() {
		rotate += 0.3;
		camPos = vec3(sin(rotate), y, cos(rotate));
	}

	/**
	* Siehe Kommentar Klasse KeyControl
	*/
	void pressR() {
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
	vec3 getCamPos() {
		return camPos * r;
	}

	/**
	* Parameter float y fuer Methode
	* setLightPos( ... , float y, ... ) in Klasse SceneControl
	*/
	float getLightY() {
		return lightY;
	}

private:
	vec3 camPos;
	float r;
	float rotate;
	float y;
	float speed;
	float lightY;
};

/**
* Klasse zum Setzen und Steuern der Lichtquelle
* und Kamera.
* Beinhaltet die mat4 View, Projection und 
* GLuint programID.
*/
class SceneControl {
	
public:

	SceneControl(float aspect) {
		programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
		aspectRatio = aspect;
	}
	
	/**
	* Pfeiltasten Steuerung
	* Uebergabeparameter ist Methode getCamPos()
	* aus Klasse SceneControl
	*/
	void setCamPos(vec3& camPos) {
		View = lookAt(camPos, 
			vec3(0.0f, 0.0f, 0.0f), 
			vec3(0.0f, 1.0f,  0.0f));
	}

	/**
	* Projection matrix : fov Field of View, display range : 0.1 unit <-> 100 units
	*/
	void setPerspective(float fov, float u1, float u2) {
		Projection = perspective(fov, aspectRatio, u1, u2);
	}

	/**
	* Setzt das Licht an die gewuenschte Position.
	* Ohne aufrufen der Methode ist das Licht bei 0, 0, 0
	*/
	void setLightPos(float x, float y, float z) {
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), x, y, z);
	}

	/**
	* Muss direkt vor der While-Schleife in
	* der Main ausgefuehrt werden.
	*/
	void useShader() {
		glUseProgram(programID);
	}

	/**
	* Sollte direkt nach der While-Schleife in
	* der Main ausgefuehrt werden.
	*/
	void deleteProgramm() {
		glDeleteProgram(programID);
	}

	/**
	* Getter Projection Matrix
	*/
	mat4 getProj() {
		return Projection;
	}
	
	/**
	* Getter View Matrix
	*/
	mat4 getView() {
		return View;
	}

	/**
	* Getter GLuint ID
	*/
	GLuint getProgID() {
		return programID;
	}

private:
	mat4 Projection;
	mat4 View;
	GLuint programID;
	float aspectRatio;
};

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
	
	/**
	* zB fuer Fische
	*/
	MoveControl(float xLimMin, float xLimMax, float yLim, float zLim, float zRotLim) {
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
	MoveControl(float limit) {
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
	void moveX() {
		setX();
		setIsWiggleLeft();
	}

	/**
	* Siehe Kommentar fuer moveX()
	* Hinweis: Anders als moveX() ist der
	* Min-Wert := -Max-Wert weil X die Hauptachse ist.
	*/
	void moveY() {
		setY();
		setIsWiggleDown();
	}

	/**
	* Siehe Kommentar fuer moveX()
	* Hinweis: Anders als moveX() ist der
	* Min-Wert := -Max-Wert weil X die Hauptachse ist.
	*/
	void moveZ() {
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
	void rotateY() {
		setRotateY();
		setIsWiggleRotateLeft();
	}

	/**
	* Uebergabeparameter fuer wiggle Methoden
	* in der Klasse Token.
	*/
	float getX() {
		return x;
	}

	/**
	* Siehe Kommentar fuer getX()
	*/
	float getY() {
		return y;
	}

	/**
	* Siehe Kommentar fuer getX()
	*/
	float getZ() {
		return z;
	}

	/**
	* Siehe Kommentar fuer getX()
	* und rotateY().
	*/
	float getRotateY() {
		return yRotate;
	}

	/**
	* Fuer if abfrage in der while-Schleife 
	* in der Main, ob sich ein Obj auf der X Achse
	* nach Rechts oder Links bewegt.
	*/
	bool getIsWiggleLeft() {
		return isWiggleLeft;
	}

private:

	void setX() {
		if (isWiggleLeft) {
			x -= xSpeed;
		} else {
			x += xSpeed;
		}
	}

	void setIsWiggleLeft() {
		if (x > xLimitMax && !isWiggleLeft) {
			isWiggleLeft = true;
		} else if (x < xLimitMin && isWiggleLeft) {
			isWiggleLeft = false;
		}
	}

	void setY() {
		if (isWiggleDown) {
			y -= ySpeed;
		} else {
			y += ySpeed;
		}
	}

	void setIsWiggleDown() {
		if (y > yLimit && !isWiggleDown) {
			isWiggleDown = true;
		} else if (y < -yLimit && isWiggleDown) {
			isWiggleDown = false;
		}
	}

	void setZ() {
		if (isWiggleDepth) {
			z -= zSpeed;
		} else {
			z += zSpeed ;
		}
	}

	void setIsWiggleDepth() {
		if (z > zLimit && !isWiggleDepth) {
			isWiggleDepth = true;
		} else if (z < -zLimit && isWiggleDepth) {
			isWiggleDepth = false;
		}
	}

	void setRotateY() {
		if (isWiggleRotateLeft) {
			yRotate -= yRotateSpeed;
		} else {
			yRotate += yRotateSpeed;
		}
	}

	void setIsWiggleRotateLeft() {
		if (yRotate > yRotateLim && !isWiggleRotateLeft) {
			isWiggleRotateLeft = true;
		} else if (yRotate < -yRotateLim && isWiggleRotateLeft) {
			isWiggleRotateLeft = false;
		}
	}
	
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