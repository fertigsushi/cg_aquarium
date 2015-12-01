// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// Achtung, die OpenGL-Tutorials nutzen glfw 2.7, glfw kommt mit einem veränderten API schon in der Version 3 

// Befindet sich bei den OpenGL-Tutorials unter "common"
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"

// Ab Uebung5 werden objloader.hpp und cpp benoetigt
#include "objloader.hpp"

// Ab Uebung7 werden texture.hpp und cpp benoetigt
#include "texture.hpp"

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;
float angle2nd = 0.0f;
float angle3rd = 0.0f;
float speed = 1.5;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_W:
		angleX += speed;
		break;
	case GLFW_KEY_S:
		angleX -= speed;
		break;
	case GLFW_KEY_A:
		angleZ -= speed;
		break;
	case GLFW_KEY_D:
		angleZ += speed;
		break;
	case GLFW_KEY_E:
		angleY -= speed;
		break;
	case GLFW_KEY_Q:
		angleY += speed;
		break;
	case GLFW_KEY_R:
		angle2nd += speed;
		break;
	case GLFW_KEY_F:
		angle2nd -= speed;
		break;
	case GLFW_KEY_T:
		angle3rd += speed;
		break;
	case GLFW_KEY_G:
		angle3rd -= speed;
		break;
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
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

int main(void) {
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Fehler werden auf stderr ausgegeben, s. o.
	glfwSetErrorCallback(error_callback);

	// Open a window and create its OpenGL context
	// glfwWindowHint vorher aufrufen, um erforderliche Resourcen festzulegen
	GLFWwindow* window = glfwCreateWindow(1024, // Breite
		768,  // Hoehe
		"CG - Tutorial", // Ueberschrift
		NULL,  // windowed mode
		NULL); // shared windoe

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the window's context current (wird nicht automatisch gemacht)
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	// GLEW ermöglicht Zugriff auf OpenGL-API > 1.1
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Auf Keyboard-Events reagieren
	glfwSetKeyCallback(window, key_callback);

	// Dark blue background
	glClearColor(0.6f, 0.6f, 0.8f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Create and compile our GLSL program from the shaders
	//programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

	// Shader auch benutzen !
	glUseProgram(programID);

	// Jedes Objekt eigenem VAO zuordnen, damit mehrere Objekte moeglich sind
	// VAOs sind Container fuer mehrere Buffer, die zusammen gesetzt werden sollen.
	GLuint VertexArrayIDTeapot;
	glGenVertexArrays(1, &VertexArrayIDTeapot);
	glBindVertexArray(VertexArrayIDTeapot);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; 
	bool res = loadOBJ("teapot.obj", vertices, uvs, normals);

	// Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer); // Kennung erhalten
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
	// Buffer zugreifbar für die Shader machen
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
	glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0 
	glVertexAttribPointer(0,  // location = 0 
		3,  // Datenformat vec3: 3 floats fuer xyz 
		GL_FLOAT, 
		GL_FALSE, // Fixedpoint data normalisieren ?
		0, // Eckpunkte direkt hintereinander gespeichert
		(void*) 0); // abweichender Datenanfang ? 

	GLuint normalbuffer; // Hier alles analog für Normalen in location == 2
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2); // siehe layout im vertex shader 
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	GLuint uvbuffer; // Hier alles analog für Texturkoordinaten in location == 1 (2 floats u und v!)
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // siehe layout im vertex shader 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	// Load the texture
	GLuint Texture = loadBMP_custom("mandrill.bmp");

	// Eventloop
	while (!glfwWindowShouldClose(window))	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		// Projection matrix : 45° Field of View, 4:3 ratio, display range (zNear, zFar): 0.1 unit <-> 100 units
		Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

		// Camera matrix
		View = glm::lookAt(glm::vec3(0,0,-5), // Camera is at (0,0,-5), in World Space
			glm::vec3(0,0,0),  // and looks at the origin
			glm::vec3(0,1,0)); // Head is up (set to 0,-1,0 to look upside-down)
				
		// Model matrix : an identity matrix (model will be at the origin)
		Model = glm::mat4(1.0f);
		
		glm::mat4 Save = Model;

		Model = glm::translate(Model, glm::vec3(1.5, 0.0, 0.0));
		Model = glm::scale(Model, glm::vec3(1.0 / 1000.0, 1.0 / 1000.0, 1.0 / 1000.0));
		sendMVP();

		//drawWireCube();
		//drawCube();
		
		//Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);

		glBindVertexArray(VertexArrayIDTeapot);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		Model = Save;
		
		Model = glm::rotate(Model, angleX, glm::vec3(1.0, 0.0, 0.0));
		Model = glm::rotate(Model, angleY, glm::vec3(0.0, 1.0, 0.0));
		Model = glm::rotate(Model, angleZ, glm::vec3(0.0, 0.0, 1.0));

		drawSeg(1.0);
		Model = glm::translate(Model, glm::vec3(0.0, 1.0, 0.0));
		Model = glm::rotate(Model, angle2nd, glm::vec3(1.0, 0.0, 0.0));
		drawSeg(0.8);
		Model = glm::translate(Model, glm::vec3(0.0, 0.8, 0.0));
		Model = glm::rotate(Model, angle3rd, glm::vec3(1.0, 0.0, 0.0));
		drawSeg(0.6);

		glm::vec4 maVec = Model * glm::vec4(0.0, 0.61, 0.0, 1.0);
		glm::vec3 lightPos = glm::vec3(maVec.x, maVec.y, maVec.z);
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
		
		// Swap buffers
		glfwSwapBuffers(window);

		// Poll for and process events 
		glfwPollEvents();
	} 

	glDeleteProgram(programID);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteTextures(1, &Texture);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

