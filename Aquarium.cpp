﻿/**
CG - Aquarium
=============
Steuerung ==> Mit den "Pfeiltasten"
Andere Interaktionsmoeglichkeiten ==> Zum Beenden "ESC"


@author Julian Osmer, 542530; name, matr_nr; 
*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "julianheader.hpp"

using namespace std;

KeyControl contKey; // Wichtig, vor key_callback(...) {}

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

/**
*
*@author Julian Osmer
*/
GLFWwindow* getWindow() {
	GLFWwindow* window;
	int h, w;
	char resolution, mode;
	cout << endl << "\tAquarium (16:9 Optimiert)" << endl << endl;
	cout << "Anleitung:" << endl << endl;
	cout << "Mit den <Pfeiltasten> die Kamera bewegen." << endl;
	cout << "Mit <ESC> das Programm Beenden." << endl << endl;
	cout << "1 = 1280x720" << endl;
	cout << "2 = 1366x768" << endl;
	cout << "3 = 1920x1080" << endl;
	cout << endl << "Auswahl = ";
	cin >> resolution;
	cout << endl << "f = Fullscreen" << endl;
	cout << "w = Windows" << endl;
	cout << endl << "Auswahl = ";
	cin >> mode;

	switch (resolution) {
		case '1':
			h = 1280, 
			w = 720;
			break;
		case '2':
			h = 1366;
			w = 768;
			break;
		case '3':
			h = 1920;
			w = 1080;
			break;
		default:
			h = 1024, 
			w = 768;
			break;
	}

	switch (mode) {
		case 'f':
			window = glfwCreateWindow(h, w,"CG - Aquarium", glfwGetPrimaryMonitor(), NULL);
			break;
		case 'w':
			window = glfwCreateWindow(h, w,"CG - Aquarium", NULL, NULL);
			break;
		default:
			window = glfwCreateWindow(h, w,"CG - Aquarium", NULL, NULL);
			break;
	}

	return window;
}

int main(void) {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}
	
	glfwSetErrorCallback(error_callback);
	
	GLFWwindow* window = getWindow();

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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	SceneControl SceCont; 

	MoveControl moveFish1(0.0, 6.0, 0.7, 0.6, 0.6);
	MoveControl moveFish2(-3.0, 5.5, 0.7, 0.6, 0.6);
	MoveControl moveFish3(-5.5, 1.0, 0.7, 0.3, 0.6);
	MoveControl moveFish4(0.0, 2.0, 0.4, 0.3, 0.6);
	MoveControl movePlant(1.0);
	
	Token fish("obj/fish.obj", "texture/fish.bmp", SceCont.getProgID()); // Schwimmt nach Rechts
	Token fishBack("obj/fishBack.obj", "texture/fish.bmp", SceCont.getProgID()); // Schwimmt nach Links
	
	Token fish2("obj/fish2.obj", "texture/fish2.bmp", SceCont.getProgID()); // Schwimmt nach Rechts
	Token fish2Back("obj/fish2Back.obj", "texture/fish2.bmp", SceCont.getProgID()); // Schwimmt nach Links
	
	Token fish3("obj/fish3.obj", "texture/fish3.bmp", SceCont.getProgID()); // Schwimmt nach Rechts
	Token fish3Back("obj/fish3Back.obj", "texture/fish3.bmp", SceCont.getProgID()); // Schwimmt nach Links
	
	Token fish4("obj/fish4.obj", "texture/fish4.bmp", SceCont.getProgID()); // Schwimmt nach Rechts
	Token fish4Back("obj/fish4Back.obj", "texture/fish4.bmp", SceCont.getProgID()); // Schwimmt nach Links

	Token plant1("obj/plant.obj", "texture/plant.bmp", SceCont.getProgID());
	Token plant2("obj/plant2.obj", "texture/plant2.bmp", SceCont.getProgID());
	Token plant3("obj/plant3.obj", "texture/plant3.bmp", SceCont.getProgID());
	
	Token ground("obj/ground.obj", "texture/sand.bmp", SceCont.getProgID());
	Token boot("obj/boot.obj", "texture/boot1.bmp", SceCont.getProgID());
	Token truhe("obj/truhe.obj", "texture/truhe.bmp", SceCont.getProgID());
	Token aquar("obj/aquarium.obj", "texture/aquarium.bmp", SceCont.getProgID());
	Token coral("obj/coral.obj", "texture/mandrill.bmp", SceCont.getProgID());
	Token huegel("obj/huegel.obj", "texture/kiesel.bmp", SceCont.getProgID());
	Token tisch("obj/tisch.obj", "texture/tisch.bmp", SceCont.getProgID());

	Token glassA1("obj/glassA1.obj", "texture/glassA.dds", /*use RGBA*/ true, SceCont.getProgID());
	Token glassA2("obj/glassA2.obj", "texture/glassA.dds", /*use RGBA*/ true, SceCont.getProgID());

	Token glassB1("obj/glassB1.obj", "texture/glassB.dds", /*use RGBA*/ true, SceCont.getProgID());
	Token glassB2("obj/glassB2.obj", "texture/glassB.dds", /*use RGBA*/ true, SceCont.getProgID());
	
	SceCont.useShader();

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Projection matrix : 45° Field of View, 16:9 ratio, display range : 0.1 unit <-> 100 units
		SceCont.setPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
		SceCont.setOrigin();
		SceCont.setCamPos(contKey.getCamPos());
		SceCont.setLightPos(0.0, 6.0, 0.0);
		SceCont.sendMVP();

		tisch.draw(); // Vor Glas zeichnen
		
		// Reihenfolge bei Glas koennte wahrsch. auswrikungen haben
		glassB2.draw(/*RGBA*/true);
		glassB1.draw(/*RGBA*/true);
		glassA2.draw(/*RGBA*/true);
		glassA1.draw(/*RGBA*/true);
		
		// Ab hier alles nach Glas zeichnen
		aquar.draw();
		ground.draw(); 
		boot.draw();
		truhe.draw();
		coral.draw();
		huegel.draw();

		fish4Back.wiggle(moveFish4.getX(), moveFish4.getY(), moveFish4.getZ(), moveFish4.getRotateY(), SceCont.getProj(), SceCont.getView());
		fish4.wiggle(moveFish4.getX(), moveFish4.getY(), moveFish4.getZ(), moveFish4.getRotateY(), SceCont.getProj(), SceCont.getView());
		
		if (moveFish4.getIsWiggleLeft()) {
			fish4Back.draw(); // Schwimmt nach Links
		} else { 
			fish4.draw(); // Schwimmt nach Rechts
		}

		fish3Back.wiggle(moveFish3.getX(), moveFish3.getY(), moveFish3.getZ(), moveFish3.getRotateY(), SceCont.getProj(), SceCont.getView());
		fish3.wiggle(moveFish3.getX(), moveFish3.getY(), moveFish3.getZ(), moveFish3.getRotateY(), SceCont.getProj(), SceCont.getView());
		
		if (moveFish3.getIsWiggleLeft()) {
			fish3Back.draw(); // Schwimmt nach Links
		} else { 
			fish3.draw(); // Schwimmt nach Rechts
		}

		fish2Back.wiggle(moveFish2.getX(), moveFish2.getY(), moveFish2.getZ(), moveFish2.getRotateY(), SceCont.getProj(), SceCont.getView());
		fish2.wiggle(moveFish2.getX(), moveFish2.getY(), moveFish2.getZ(), moveFish2.getRotateY(), SceCont.getProj(), SceCont.getView());
		
		if (moveFish2.getIsWiggleLeft()) {
			fish2Back.draw(); // Schwimmt nach Links
		} else { 
			fish2.draw(); // Schwimmt nach Rechts
		}
		
		fishBack.wiggle(moveFish1.getX(), moveFish1.getY(), moveFish1.getZ(), moveFish1.getRotateY(), SceCont.getProj(), SceCont.getView());
		fish.wiggle(moveFish1.getX(), moveFish1.getY(), moveFish1.getZ(), moveFish1.getRotateY(), SceCont.getProj(), SceCont.getView());
		
		if (moveFish1.getIsWiggleLeft()) {
		fishBack.draw(); // Schwimmt nach Links
		} else { 
			fish.draw(); // Schwimmt nach Rechts
		}
		
		plant1.wiggle(movePlant.getX(), movePlant.getY(), movePlant.getZ(), SceCont.getProj(), SceCont.getView());
		plant1.draw();

		plant2.wiggle(movePlant.getX(), movePlant.getY(), movePlant.getZ(), SceCont.getProj(), SceCont.getView());
		plant2.draw();
		
		plant3.wiggle(movePlant.getX(), movePlant.getY(), movePlant.getZ(), SceCont.getProj(), SceCont.getView());
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

		SceCont.setOrigin();
		SceCont.sendMVP();
		
		glfwSwapBuffers(window);
        glfwPollEvents();
	} 
	SceCont.deleteProgramm();
	glfwTerminate();
	return 0;
}

