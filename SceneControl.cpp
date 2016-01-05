#include "SceneControl.hpp"


SceneControl::SceneControl(float aspect) {
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	aspectRatio = aspect;
}


SceneControl::~SceneControl(void) {
}

/**
* Pfeiltasten Steuerung
* Uebergabeparameter ist Methode getCamPos()
* aus Klasse SceneControl
*/
void SceneControl::setCamPos(vec3& camPos) {
	View = lookAt(camPos, 
		vec3(0.0f, 0.0f, 0.0f), 
		vec3(0.0f, 1.0f,  0.0f));
}

/**
* Projection matrix : fov Field of View, display range : 0.1 unit <-> 100 units
*/
void SceneControl::setPerspective(float fov, float u1, float u2) {
	Projection = perspective(fov, aspectRatio, u1, u2);
}

/**
* Setzt das Licht an die gewuenschte Position.
* Ohne aufrufen der Methode ist das Licht bei 0, 0, 0
*/
void SceneControl::setLightPos(float x, float y, float z) {
	glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), x, y, z);
}

/**
* Muss direkt vor der While-Schleife in
* der Main ausgefuehrt werden.
*/
void SceneControl::useShader() {
	glUseProgram(programID);
}

/**
* Sollte direkt nach der While-Schleife in
* der Main ausgefuehrt werden.
*/
void SceneControl::deleteProgramm() {
	glDeleteProgram(programID);
}

/**
* Getter Projection Matrix
*/
mat4 SceneControl::getProj() {
	return Projection;
}

/**
* Getter View Matrix
*/
mat4 SceneControl::getView() {
	return View;
}

/**
* Getter GLuint ID
*/
GLuint SceneControl::getProgID() {
	return programID;
}