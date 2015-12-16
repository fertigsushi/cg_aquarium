#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <algorithm>
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

glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 Save;
glm::mat4 SaveKugel; 
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
	*/
	void draw(bool isRGBA) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexturObj);
		glUniform1i(glGetUniformLocation(programID, "myTexturSampler"), 0);
		glBindVertexArray(VertexArrayIDObj);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisable(GL_BLEND);
	}

	void wiggle(float x, float y, float z, float yRotate, mat4& Projection , mat4& View) {
		setModelToOrigin();
		wiggleRotate(yRotate);
		Model = translate(Model, vec3(x, y, z));
		sendMVP(Projection , View);
	}

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

	float getAngle(float RotationAngle) {
		return RotationAngle * sin(RotationAngle / 2);
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
	const char * path; // Obj path
	const char * imagepath; // Textur path
	GLuint VertexArrayIDObj; // Obj
	GLuint TexturObj; // Textur
	bool res; // TexturLoader loadDDS or loadBMP
};

class KeyControl { 

public:
	
	KeyControl() {
		speed = 0.1;
		r = 15.0;
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
		r = 15.0;
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

	SceneControl() {}

	void setCamPos(mat4& View, vec3& camPos) {
		View = lookAt(camPos, 
			vec3(0.0f, 0.0f, 0.0f), 
			vec3(0.0f, 1.0f,  0.0f));
	}

	void setOrigin(mat4& Model) {
		Model = mat4(1.0f);
	}

	void setPerspective(mat4& Projection, float x, float y, float z, float w) {
		Projection = perspective(x, y, z, w);
	}

	void setLightPos(float x, float y, float z) {
		vec3 lightPos = vec3(x, y, z);
		vec3 lightColor = vec3(1.0, 0.1, 0.1);
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
	}

	void sendMVP(mat4& Model) {
		mat4 MVP = Projection * View * Model; 
		glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
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
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	SceneControl SceCont;

	MoveControl moveFish1(0.0, 6.0, 0.7, 0.6, 0.6);
	MoveControl movePlant(1.0);
	MoveControl moveFish2(-3.0, 5.5, 0.7, 0.6, 0.6);
	MoveControl moveFish3(-5.5, 1.0, 0.7, 0.3, 0.6);
	MoveControl moveFish4(0.0, 2.0, 0.4, 0.3, 0.6);
	
	Token fish("obj/fish.obj", "texture/fish.bmp"); // Schwimmt nach Rechts
	Token fishBack("obj/fishBack.obj", "texture/fish.bmp"); // Schwimmt nach Links
	
	Token fish2("obj/fish2.obj", "texture/fish2.bmp"); // Schwimmt nach Rechts
	Token fish2Back("obj/fish2Back.obj", "texture/fish2.bmp"); // Schwimmt nach Links
	
	Token fish3("obj/fish3.obj", "texture/fish3.bmp"); // Schwimmt nach Rechts
	Token fish3Back("obj/fish3Back.obj", "texture/fish3.bmp"); // Schwimmt nach Links
	
	Token fish4("obj/fish4.obj", "texture/fish4.bmp"); // Schwimmt nach Rechts
	Token fish4Back("obj/fish4Back.obj", "texture/fish4.bmp"); // Schwimmt nach Links
	
	Token ground("obj/ground.obj", "texture/sand.bmp");
	Token plant1("obj/plant.obj", "texture/plant.bmp");
	Token plant2("obj/plant2.obj", "texture/plant2.bmp");
	Token plant3("obj/plant3.obj", "texture/plant3.bmp");
	Token boot("obj/boot.obj", "texture/boot1.bmp");
	Token truhe("obj/truhe.obj", "texture/truhe.bmp");
	Token aquar("obj/aquarium.obj", "texture/aquarium.bmp");
	Token glass("obj/glass.obj", "texture/glass.dds", /*use RGBA*/ true);
	Token coral("obj/coral.obj", "texture/mandrill.bmp");
	Token huegel("obj/huegel.obj", "texture/kiesel.bmp");

	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	glUseProgram(programID);

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		SceCont.setPerspective(Projection, 45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

		SceCont.setOrigin(Model);
		
		SceCont.setCamPos(View, contKey.getCamPos());
		
		SceCont.setLightPos(0.0, 12.0, 1.0);
		
		SceCont.sendMVP(Model);

		ground.draw();
		aquar.draw();
		boot.draw();
		truhe.draw();
		coral.draw();
		huegel.draw();

		fish4Back.wiggle(moveFish4.getX(), moveFish4.getY(), moveFish4.getZ(), moveFish4.getRotateY(), Projection, View);
		fish4.wiggle(moveFish4.getX(), moveFish4.getY(), moveFish4.getZ(), moveFish4.getRotateY(), Projection, View);
		
		if (moveFish4.getIsWiggleLeft()) {
			fish4Back.draw(); // Schwimmt nach Links
		} else { 
			fish4.draw(); // Schwimmt nach Rechts
		}

		fish3Back.wiggle(moveFish3.getX(), moveFish3.getY(), moveFish3.getZ(), moveFish3.getRotateY(), Projection, View);
		fish3.wiggle(moveFish3.getX(), moveFish3.getY(), moveFish3.getZ(), moveFish3.getRotateY(), Projection, View);
		
		if (moveFish3.getIsWiggleLeft()) {
			fish3Back.draw(); // Schwimmt nach Links
		} else { 
			fish3.draw(); // Schwimmt nach Rechts
		}

		fish2Back.wiggle(moveFish2.getX(), moveFish2.getY(), moveFish2.getZ(), moveFish2.getRotateY(), Projection, View);
		fish2.wiggle(moveFish2.getX(), moveFish2.getY(), moveFish2.getZ(), moveFish2.getRotateY(), Projection, View);
		
		if (moveFish2.getIsWiggleLeft()) {
			fish2Back.draw(); // Schwimmt nach Links
		} else { 
			fish2.draw(); // Schwimmt nach Rechts
		}
		
		fishBack.wiggle(moveFish1.getX(), moveFish1.getY(), moveFish1.getZ(), moveFish1.getRotateY(), Projection, View);
		fish.wiggle(moveFish1.getX(), moveFish1.getY(), moveFish1.getZ(), moveFish1.getRotateY(), Projection, View);
		
		if (moveFish1.getIsWiggleLeft()) {
		fishBack.draw(); // Schwimmt nach Links
		} else { 
			fish.draw(); // Schwimmt nach Rechts
		}
		
		plant1.wiggle(movePlant.getX(), movePlant.getY(), movePlant.getZ(), Projection, View);
		plant1.draw();

		plant2.wiggle(movePlant.getX(), movePlant.getY(), movePlant.getZ(), Projection, View);
		plant2.draw();
		
		plant3.wiggle(movePlant.getX(), movePlant.getY(), movePlant.getZ(), Projection, View);
		plant3.draw();

		moveFish1.moveX();
		moveFish1.moveY();
		moveFish1.moveZ();
		moveFish1.rotateY();

		moveFish2.moveX();
		moveFish2.moveY();
		moveFish2.moveZ();
		moveFish2.rotateY();

		moveFish3.moveX();
		moveFish3.moveY();
		moveFish3.moveZ();
		moveFish3.rotateY();

		moveFish4.moveX();
		moveFish4.moveY();
		moveFish4.moveZ();
		moveFish4.rotateY();

		movePlant.moveX();
		movePlant.moveY();
		movePlant.moveZ();

		SceCont.setOrigin(Model);
		
		SceCont.sendMVP(Model);
		
		glass.draw(/*RGBA*/true);

		glfwSwapBuffers(window);
        glfwPollEvents();
	} 
	glDeleteProgram(programID);
	glfwTerminate();
	return 0;
}

