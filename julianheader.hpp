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

/*
* ...
* int main() {
*	...
*	Token test("xyz.obj", "abc.bmp");
*	while(...) {
*	...
*	test.draw();
*	...
*	}
*	...
*}
*/
class Token {

public:

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

	/*
	* Fuer Glass
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

	void draw() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexturObj);
		glUniform1i(glGetUniformLocation(programID, "myTexturSampler"), 0);
		glBindVertexArray(VertexArrayIDObj);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}

	/*
	* Fuer Glass
	*/
	void draw(bool isRGBA) {
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

	// Fuer Fish
	void wiggle(float x, float y, float z, float yRotate, mat4& Projection , mat4& View) {
		setModelToOrigin();
		wiggleRotate(yRotate);
		Model = translate(Model, vec3(x, y, z));
		sendMVP(Projection , View);
	}
	
	// Fuer Plant
	void wiggle(float x, float y, float z, mat4& Projection , mat4& View) {
		setModelToOrigin();
		Model = rotate(Model, y, vec3(0.0, 1.0, 0.0));
		Model = rotate(Model, z, vec3(0.0, 0.0, 1.0));
		Model = rotate(Model, x, vec3(1.0, 0.0, 0.0));
		sendMVP(Projection , View);
	}

private:

	void setModelToOrigin() {
		Model = mat4(1.0f);
	}

	void sendMVP(mat4& Projection , mat4& View ) {
		mat4 MVP = Projection * View * Model; 
		glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	}

	void wiggleRotate(float zRotate) {
		Model = rotate(Model, zRotate, vec3(0.0, 1.0, 0.0));
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

class KeyControl { 

public:
	
	KeyControl() {
		speed = 0.1;
		r = 18.0;
		y = 0.0f;
		rotate = 0.0;
		camPos = vec3(sin(rotate), y, cos(rotate));
	}
	
	void pressUP() {
		if(r > 1.0) {
		r -= speed;
		}
	}

	void pressDOWN() {
		r += speed;
	}

	void pressLEFT() {
		rotate -= speed;
		camPos = vec3(sin(rotate), y, cos(rotate));
	}

	void pressRIGHT() {
		rotate += 0.3;
		camPos = vec3(sin(rotate), y, cos(rotate));
	}

	void pressR() {
		r = 18.0;
		y = 0.0f;
		rotate = 0.0;
		camPos = vec3(sin(rotate), y, cos(rotate));
	}

	vec3 getCamPos() {
		return camPos * r;
	}

private:
	vec3 camPos;
	float r;
	float rotate;
	float y;
	float speed;
};

class SceneControl {
	
public:

	SceneControl(float aspect) {
		programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
		aspectRatio = aspect;
	}

	void setCamPos(vec3& camPos) {
		View = lookAt(camPos, 
			vec3(0.0f, 0.0f, 0.0f), 
			vec3(0.0f, 1.0f,  0.0f));
	}

	void setOrigin() {
		Model = mat4(1.0f);
	}

	void setPerspective(float x, float y, float z, float w) {
		Projection = perspective(x, y, z, w);
	}

	void setLightPos(float x, float y, float z) {
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), x, y, z);
	}

	void sendMVP() {
		MVP = Projection * View * Model; 
		glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	}

	void useShader() {
		glUseProgram(programID);
	}

	mat4 getProj() {
		return Projection;
	}

	mat4 getView() {
		return View;
	}

	mat4 getModel() {
		return Model;
	}

	GLuint getProgID() {
		return programID;
	}

	float getAspectRatio() {
		return aspectRatio;
	}

	void deleteProgramm() {
		glDeleteProgram(programID);
	}

private:
	mat4 MVP;
	mat4 Projection;
	mat4 View;
	mat4 Model;
	GLuint programID;
	float aspectRatio;
};

class MoveControl {

public:
	
	// fish
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
		xSpeed = 0.0055;
		ySpeed = 0.0015;
		zSpeed = 0.0025;
		yRotateSpeed = 0.2;
		isWiggleLeft = false; // x
		isWiggleDown = false; // y
		isWiggleDepth = false; // z
		isWiggleRotateLeft = false; // zRotate
	}

	// Fish 2
	MoveControl(float xLimMin, float xLimMax, float yLim, float zLim, float yRotLim, float x, float y, float z) {
		xLimitMin = xLimMin;
		xLimitMax = xLimMax;
		yLimit = yLim;
		zLimit = zLim;
		yRotateLim = yRotLim;
		x = x;
		y = y;
		z = z;
		yRotate = -1.6;
		xSpeed = 0.0005;
		ySpeed = 0.0001;
		zSpeed = 0.0002;
		yRotateSpeed = 0.1;
		isWiggleLeft = false; // x
		isWiggleDown = false; // y
		isWiggleDepth = false; // z
		isWiggleRotateLeft = false; // zRotate
	}

	// plant
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

	void moveX() {
		setX();
		setIsWiggleLeft();
	}

	void moveY() {
		setY();
		setIsWiggleDown();
	}

	void moveZ() {
		setZ();
		setIsWiggleDepth();
	}

	void rotateY() {
		setRotateY();
		setIsWiggleRotateLeft();
	}

	float getX() {
		return x;
	}

	float getY() {
		return y;
	}

	float getZ() {
		return z;
	}

	float getRotateY() {
		return yRotate;
	}

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