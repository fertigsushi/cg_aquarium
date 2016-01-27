#ifndef SCENECONTROL_H
#define SCENECONTROL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"

using namespace glm;

/**
* Klasse zum Setzen und Steuern der Lichtquelle
* und Kamera.
* Beinhaltet die mat4 View, Projection und 
* GLuint programID.
*/
class SceneControl {

public:
	SceneControl(float aspect);
	~SceneControl();
	void setCamPos(vec3& camPos);
	void setPerspective(float fov, float u1, float u2);
	void setLightPos(float x, float y, float z);
	void useShader();
	void deleteProgramm();
	glm::mat4 getProj();
	glm::mat4 getView();
	GLuint getProgID();

protected:
	glm::mat4 Projection;
	glm::mat4 View;
	GLuint programID;
	float aspectRatio;
};

#endif // SCENECONTROL_H