// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <cstdlib>

#include <algorithm>
// sin und cos
#include <math.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Achtung, die OpenGL-Tutorials nutzen glfw 2.7, glfw kommt mit einem veränderten API schon in der Version 3 

// Befindet sich bei den OpenGL-Tutorials unter "common"
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"

// Ab Uebung5 werden objloader.hpp und cpp benoetigt
#include "objloader.hpp"

// Ab Uebung7 werden texture.hpp und cpp benoetigt
#include "texture.hpp"

using namespace glm;
using namespace std;

// Diese Drei Matrizen global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
GLuint programID;

/*
* ...
* int main() {
*	...
*	Token test("xyz.obj", "abc.bmp");
*	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");	
*	glUseProgram(programID);
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

	Token(const char * path, const char * imagepath) {
		path = path;
		imagepath = imagepath;
		Model = mat4(1.0f);
		VertexArrayIDObj = 0;
		res = loadOBJ(path, vertices, uvs, normals);		
		TexturObj = loadBMP_custom(imagepath);
		create();
	}

	/*
	* Fuer Glass
	*/
	Token(const char * path, const char * imagepath, bool isRGBA) {
		path = path;
		imagepath = imagepath;
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
	* Noch etwas verbuggt!
	*/
	void draw(bool isRGBA) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		
		glAlphaFunc(GL_GREATER, 0.1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexturObj);
		glUniform1i(glGetUniformLocation(programID, "myTexturSampler"), 0);
		glBindVertexArray(VertexArrayIDObj);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
	}

	void sendMVP(mat4& Projection , mat4& View ) {
		mat4 MVP = Projection * View * Model; 
		glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	}

	void wiggle(float x, float y, float z, float zRotate) {
		Model = mat4(1.0f);
		wiggleRotate(zRotate);
		Model = translate(Model, vec3(x, y, z));
	}

	void wiggle(float x, float y, float z) {
		Model = mat4(1.0f);
		Model = rotate(Model, y, vec3(0.0, 1.0, 0.0));
		Model = rotate(Model, z, vec3(0.0, 0.0, 1.0));
		Model = rotate(Model, x, vec3(1.0, 0.0, 0.0));
	}

	mat4 getModel() {
		return Model;
	}

private:

	float getRandom() {
		srand ( time(NULL) ); //just seed the generator
		int r = rand() % 16000 - 1000; //this produces numbers between -2000 - +2000
		return r/10000.0;
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
	const char * path; // Obj path
	const char * imagepath; // Textur path
	GLuint VertexArrayIDObj; // Obj
	GLuint TexturObj; // Textur
	bool res; // TexturLoader loadDDS or loadBMP
};

class KeyControl { 

public:
	
	KeyControl() {
		camFraction = 0.1f;
		camWinkel = 0.0;
		camLX = 0.0f;
		camLZ = -1.0;
		camX = 0.0f; 
		camZ = 5.0f;
	}
	
	void pressUP() {
		camX += camLX * camFraction;
		camZ += camLZ * camFraction;
	}

	void pressDOWN() {
		camX -= camLX * camFraction;
		camZ -= camLZ * camFraction;
	}

	void pressLEFT() {
		camWinkel -= 0.01f;
		camLX = sin(camWinkel);
		camLZ = -cos(camWinkel);
	}

	void pressRIGHT() {
		camWinkel += 0.01f;
		camLX = sin(camWinkel);
		camLZ = -cos(camWinkel);
	}

	void pressR() {
		camFraction = 0.1f;
		camWinkel = 0.0;
		camLX = 0.0f;
		camLZ = -1.0;
		camX = 0.0f; 
		camZ = 5.0f;
	}

	float getCamX() {
		return camX;
	}

	float getCamZ() {
		return camZ;
	}

	float getCamLX() {
		return camLX + camX;
	}

	float getCamLZ() {
		return camLZ + camZ;
	}

private:

	float camFraction;
	float camWinkel;
	float camLX;
	float camLZ;
	float camX; 
	float camZ;
};

class SceneControl {
	
public:

	SceneControl() {}

	void setCamPos(mat4& View, KeyControl contKey) {
		View = lookAt(vec3(contKey.getCamX(), 1.0f, contKey.getCamZ()), 
			vec3(contKey.getCamLX(), 1.0f, contKey.getCamLZ()), 
			vec3(0.0f, 1.0f,  0.0f));
	}

	void setOrigin(mat4& Model, float f) {
		Model = mat4(f);
	}

	void setPerspective(mat4& Projection, float x, float y, float z, float w) {
		Projection = perspective(x, y, z, w);
	}

	void setLightPos(float x, float y, float z) {
		vec3 lightPos = vec3(x, y, z);
		vec3 lightColor = vec3(1.0, 0.1, 0.1);
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(programID, "lightColor"), lightColor.r, lightColor.g, lightColor.b);
	}

	void fishControl(bool isWiggleLeft, float wiggle) {

	}
};

class MoveControl {

public:
	
	// fish
	MoveControl(float xLimMin, float xLimMax, float yLim, float zLim, float zRotLim) {
		xLimitMin = xLimMin;
		xLimitMax = xLimMax;
		yLimit = yLim;
		zLimit = zLim;
		zRotateLim = zRotLim;
		x = 0.0;
		y = 0.0;
		z = 0.0;
		zRotate = -1.6;
		xSpeed = 0.0005;
		ySpeed = 0.0001;
		zSpeed = 0.0002;
		zRotateSpeed = 0.1;
		isWiggleLeft = false; // x
		isWiggleDown = false; // y
		isWiggleDepth = false; // z
		isWiggleRotateLeft = false; // zRotate
	}
	// Fish 2
	MoveControl(float xLimMin, float xLimMax, float yLim, float zLim, float zRotLim, float x, float y, float z) {
		xLimitMin = xLimMin;
		xLimitMax = xLimMax;
		yLimit = yLim;
		zLimit = zLim;
		zRotateLim = zRotLim;
		x = x;
		y = y;
		z = z;
		zRotate = -1.6;
		xSpeed = 0.0005;
		ySpeed = 0.0001;
		zSpeed = 0.0002;
		zRotateSpeed = 0.1;
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
		xSpeed = 0.0005;
		ySpeed = 0.0005;
		zSpeed = 0.0001;
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

	void moveRotateZ() {
		setRotateZ();
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

	float getRotateZ() {
		return zRotate;
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

	void setRotateZ() {
		if (isWiggleRotateLeft) {
			zRotate -= zRotateSpeed;
		} else {
			zRotate += zRotateSpeed;
		}
	}

	void setIsWiggleRotateLeft() {
		if (zRotate > zRotateLim && !isWiggleRotateLeft) {
			isWiggleRotateLeft = true;
		} else if (zRotate < -zRotateLim && isWiggleRotateLeft) {
			isWiggleRotateLeft = false;
		}
	}
	
	float x;
	float y;
	float z;
	float xLimitMin;
	float xLimitMax;
	float yLimit;
	float zLimit;
	float zRotateLim;
	float xSpeed;
	float ySpeed;
	float zSpeed;
	float zRotateSpeed;
	float zRotate;
	bool isWiggleLeft; // x
	bool isWiggleDown; // y
	bool isWiggleDepth; // z
	bool isWiggleRotateLeft; // zRotate
};

KeyControl contKey;

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_R:
			contKey.pressR();
			break;
		case GLFW_KEY_LEFT :
			contKey.pressLEFT();
			break;
		case GLFW_KEY_RIGHT :
			contKey.pressRIGHT();
			break;
		case GLFW_KEY_UP :
			contKey.pressUP();
			break;
		case GLFW_KEY_DOWN :
			contKey.pressDOWN();
			break;
		default:
			break;
	}
}

void sendMVP() {
	glm::mat4 MVP = Projection * View * Model; 
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
}

void sendMVP(mat4& Model) {
	mat4 MVP = Projection * View * Model; 
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
}

void drawSeg(float heigth) {
	glm::mat4 Save = Model;
	Model = glm::translate(Model, glm::vec3(0.0, heigth / 2.0, 0.0));
	Model = glm::scale(Model, glm::vec3(0.15, heigth / 2.0, 0.15));
	sendMVP();
	drawSphere(10, 10);
	Model = Save;
}

int main(void) {
	
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(error_callback);
	GLFWwindow* window = glfwCreateWindow(1024, 768,"CG - Tutorial", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
    glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	glfwSetKeyCallback(window, key_callback);
	glClearColor(0.6f, 0.6f, 0.8f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	SceneControl SceCont;

	MoveControl moveFish(0.0, 4.0, 0.4, 0.6, 1.6);
	MoveControl movePlant(1.0);
	
	Token fish("fish.obj", "fish.bmp");
	Token ground("ground.obj", "sand.bmp");
	Token plant("plant.obj", "blatt.bmp");
	Token aquar("aquarium.obj", "aquarium.bmp");
	Token glass("glass.obj", "glass.dds", /*RGBA*/true);

	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	glUseProgram(programID);
	
	// Eventloop
	while (!glfwWindowShouldClose(window))	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		
		SceCont.setPerspective(Projection, 45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		
		SceCont.setCamPos(View, contKey);
		
		SceCont.setLightPos(0.0, 4.0, 1.0);
		
		SceCont.setOrigin(Model, 1.0f);
		
		sendMVP();
		
		ground.draw();

		aquar.draw();

		//glass.draw(/*RGBA*/true);

		fish.wiggle(moveFish.getX(), moveFish.getY(), moveFish.getZ(), moveFish.getRotateZ());
		fish.sendMVP(Projection, View);
		fish.draw();
		
		plant.wiggle(movePlant.getX(), movePlant.getY(), movePlant.getZ());
		plant.sendMVP(Projection, View);
		plant.draw();

		moveFish.moveX();
		moveFish.moveY();
		moveFish.moveZ();
		moveFish.moveRotateZ();

		movePlant.moveX();
		movePlant.moveY();
		movePlant.moveZ();
		
		// Swap buffers
		glfwSwapBuffers(window);

		// Poll for and process events 
		glfwPollEvents();
	} 

	glDeleteProgram(programID);

	// Cleanup VBO and shader
	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &normalbuffer);
	//glDeleteBuffers(1, &uvbuffer);
	//glDeleteTextures(1, &Texture);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

