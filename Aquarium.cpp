// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

// Achtung, die OpenGL-Tutorials nutzen glfw 2.7, glfw kommt mit einem veränderten API schon in der Version 3 

// Befindet sich bei den OpenGL-Tutorials unter "common"
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"

// Ab Uebung5 werden objloader.hpp und cpp benoetigt
#include "objloader.hpp"

// Ab Uebung7 werden texture.hpp und cpp benoetigt
#include "texture.hpp"

struct controlVar { 
	float aquaX_Angle;
	float aquaY_Angle;
	float aquaZ_Angle;
	float camFraction;
	float camWinkel;
	float camLX;
	float camLZ;
	float camX; 
	float camZ;
} conVar = {0.0, 0.0, 0.0, 0.1f, 0.0, 0.0f, -1.0f, 0.0f, 5.0f};

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_Y:
			conVar.aquaY_Angle += 5.0;
			break;
		case GLFW_KEY_X:
			conVar.aquaX_Angle += 5.0;
			break;
		case GLFW_KEY_Z:
			conVar.aquaZ_Angle += 5.0;
			break;
		case GLFW_KEY_R:
			conVar.aquaX_Angle = 0.0;
			conVar.aquaY_Angle = 0.0;
			conVar.aquaZ_Angle = 0.0;
			break;
		case GLFW_KEY_LEFT :
			conVar.camWinkel -= 0.01f;
			conVar.camLX = sin(conVar.camWinkel);
			conVar.camLZ = -cos(conVar.camWinkel);
			break;
		case GLFW_KEY_RIGHT :
			conVar.camWinkel += 0.01f;
			conVar.camLX = sin(conVar.camWinkel);
			conVar.camLZ = -cos(conVar.camWinkel);
			break;
		case GLFW_KEY_UP :
			conVar.camX += conVar.camLX * conVar.camFraction;
			conVar.camZ += conVar.camLZ * conVar.camFraction;
			break;
		case GLFW_KEY_DOWN :
			conVar.camX -= conVar.camLX * conVar.camFraction;
			conVar.camZ -= conVar.camLZ * conVar.camFraction;
			break;
		default:
			break;
	}
}

// Diese Drei Matrizen global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
GLuint programID;

void sendMVP() {
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; 
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform, konstant fuer alle Eckpunkte
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
}

void drawSeg(float heigth) {
	glm::mat4 Save = Model;
	Model = glm::translate(Model, glm::vec3(0.0, heigth / 2.0, 0.0));
	Model = glm::scale(Model, glm::vec3(0.15, heigth / 2.0, 0.15));
	sendMVP();
	drawSphere(10, 10);
	Model = Save;
}

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
		VertexArrayIDObj = 0;
		res = loadOBJ(path, vertices, uvs, normals);		
		TexturObj = loadBMP_custom(imagepath);
		create();
	}
	
	/*
	* Fuer Glas Obj
	*/
	Token(const char * path, const char * imagepath, bool UseRGBA) {
		path = path;
		imagepath = imagepath;
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
	* Fuer Glas Obj
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
	
private:
	
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

	vector<vec3> vertices; // Buffer
	vector<vec2> uvs; // Buffer
	vector<vec3> normals; // Buffer
	const char * path; // Obj path
	const char * imagepath; // Textur path
	GLuint VertexArrayIDObj; // Obj
	GLuint TexturObj; // Textur
	bool res; // TexturLoader loadDDS or loadBMP
};

/**
*
*
*/
class Control {
	
public:

	Control() {}

	void setCamPos(mat4& View, controlVar& conVar) {
		View = lookAt(vec3(conVar.camX, 1.0f, conVar.camZ), vec3(conVar.camX + conVar.camLX , 1.0f, conVar.camZ + conVar.camLZ), vec3(0.0f, 1.0f,  0.0f));
	}

	void rotateAqua(mat4& Model, controlVar& conVar) {
		Model = rotate(Model, conVar.aquaY_Angle, vec3(0.0, 1.0, 0.0));
		Model = rotate(Model, conVar.aquaZ_Angle, vec3(0.0, 0.0, 1.0));
		Model = rotate(Model, conVar.aquaX_Angle, vec3(1.0, 0.0, 0.0));
	}

	void setOrigin(mat4& Model, float f) {
		Model = mat4(f);
	}

	void setPerspective(mat4& Projection, float x, float y, float z, float w) {
		Projection = perspective(x, y, z, w);
	}

	void setLightPos(float x, float y, float z) {
		vec3 lightPos = vec3(x, y, z);
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
	}
};


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
	
	Control control;
	
	Token fish("fish.obj", "mandrill.bmp");

	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	glUseProgram(programID);

	// Eventloop
	while (!glfwWindowShouldClose(window))	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		
		control.setPerspective(Projection, 45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		
		control.setCamPos(View, conVar);
		
		control.setLightPos(4, 4, -4);
		
		control.setOrigin(Model, 1.0f);
		
		control.rotateAqua(Model, conVar);
		
		sendMVP();

		fish.draw();
		
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

