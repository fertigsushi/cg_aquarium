/**
* CG - Aquarium
* =============
* Steuerung ==> Mit den "Pfeiltasten"
* Andere Interaktionsmoeglichkeiten ==> Zum Beenden "ESC"
*...
*
* @author Julian Osmer, 542530; name, matr_nr; 
*/

#define MAX_FPS 30

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "KeyControl.hpp"
#include "MoveControl.hpp"
#include "Fish.hpp"
#include "Artifact.hpp"
#include "Plant.hpp"
#include "Fishbowl.hpp"

using namespace std;

float aspectRatio; // Ubergabeparameter fuer Klasse SceneControl
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
	case GLFW_KEY_1 :
		contKey.press1();
		break;
	case GLFW_KEY_2 :
		contKey.press2();
		break;
	default:
		break;
	}
}

/**
* Div. cout Befehle fuer getWindow()
* @author Julian Osmer
*/
void coutGetWindow() {
	cout << endl << "\tAquarium (16:9 Optimiert)" << endl << endl;
	cout << "Anleitung:" << endl << endl;
	cout << "Mit den <Pfeiltasten> die Kamera bewegen." << endl;
	cout << "Mit den <R> die Kamera reseten." << endl;
	cout << "Mit <ESC> das Programm Beenden." << endl << endl;
	cout << "\t1 = 1280x720 16:9" << endl;
	cout << "\t2 = 1600x900 16:9" << endl;
	cout << "\t3 = 1920x1080 16:9" << endl;
	cout << "\t4 = 1280x800 16:10" << endl;
	cout << "\t5 = 1680x1050 16:10" << endl;
	cout << "\t6 = 1920x1200 16:10" << endl;
	cout << "\t7 = 1024x768 4:3" << endl;
	cout << endl << "\tf = Fullscreen" << endl;
	cout << "\tw = Windowed" << endl;
}

/**
* Getter fur ein GLFWwindow* window was im
* diesem Programm benutzt wird.
* Anwender kann versch. Aufloesungen inkl.
* dessen korrektem Bildformat waehlen.
* Auf das korrekte Bildformat(aspectRatio) sollte beim
* erweitern der Aufloesungen geachtet werden.
* Und der Anwender kann zwischen Vollbild oder Fensterbild
* waehlen.
* @author Julian Osmer
*/
GLFWwindow* getWindow() {
	GLFWwindow* window;
	int h, w;
	char resolution, mode;
	coutGetWindow();
	cout << endl << "Auflosung = ";
	cin >> resolution;
	cout << endl << "Full // Windowed = ";
	cin >> mode;

	switch (resolution) {
	case '1':
		h = 1280, w = 720; aspectRatio = 16.0f/9.0f;
		break;
	case '2':
		h = 1600; w = 900; aspectRatio = 16.0f/9.0f;
		break;
	case '3':
		h = 1920; w = 1080; aspectRatio = 16.0f/9.0f;
		break;
	case '4':
		h = 1280; w = 800; aspectRatio = 16.0f/10.0f;
		break;
	case '5':
		h = 1680; w = 1050; aspectRatio = 16.0f/10.0f;;
		break;
	case '6':
		h = 1920; w = 1200; aspectRatio = 16.0f/10.0f;;
		break;
	case '7':
		h = 1024; w = 768; aspectRatio = 4.0f/3.0f;;
		break;
	default:
		h = 1024; w = 768; aspectRatio = 4.0f/3.0f;;
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

	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SceneControl SceCont(aspectRatio);
	MoveControl movePlant(1.0);
	
	vector<Plant> my_plants;
	my_plants.push_back(Plant("obj/plant.obj", "texture/plant.bmp", &SceCont));
	my_plants.push_back(Plant("obj/plant2.obj", "texture/plant2.bmp", &SceCont));
	my_plants.push_back(Plant("obj/plant3.obj", "texture/plant3.bmp", &SceCont));

	vector<Artifact> my_stuff;
	my_stuff.push_back(Artifact("obj/ground.obj", "texture/sand.bmp", &SceCont));
	my_stuff.push_back(Artifact("obj/boot.obj", "texture/boot1.bmp", &SceCont));
	my_stuff.push_back(Artifact("obj/truhe.obj", "texture/truhe.bmp", &SceCont));
	my_stuff.push_back(Artifact("obj/aquarium.obj", "texture/aquarium.bmp", &SceCont));
	my_stuff.push_back(Artifact("obj/coral.obj", "texture/mandrill.bmp", &SceCont));
	my_stuff.push_back(Artifact("obj/huegel.obj", "texture/kiesel.bmp", &SceCont));
	my_stuff.push_back(Artifact("obj/tisch.obj", "texture/tisch.bmp", &SceCont));

	// Glaeser A: lange Seite (1 vorne, 2 hinten)
	// Glaeser B: kurze Seite (1 rechts, 2 links)
	vector<Artifact> my_glasses;
	my_glasses.push_back(Artifact("obj/glassA1.obj", "texture/glassA.dds", &SceCont, /*use RGBA*/ true));
	my_glasses.push_back(Artifact("obj/glassA2.obj", "texture/glassA.dds", &SceCont, /*use RGBA*/ true));
	my_glasses.push_back(Artifact("obj/glassB1.obj", "texture/glassB.dds", &SceCont, /*use RGBA*/ true));
	my_glasses.push_back(Artifact("obj/glassB2.obj", "texture/glassB.dds", &SceCont, /*use RGBA*/ true));
	
	Fishbowl* fishbowl = Fishbowl::create(glm::vec3(-5.1f, -1.5f, -3.0f), glm::vec3(5.1f, 1.8f, 3.5f));

	vector<Fish> my_fishes;
	my_fishes.push_back(Fish(glm::vec3(-4.0f, -1.0f, -3.0f), 0.1f, fishbowl, "obj/fish_test2.obj", "texture/fish.bmp", &SceCont));
	my_fishes.push_back(Fish(glm::vec3(-2.0f, -0.5f, -2.0f), 0.1f, fishbowl, "obj/fish_test2.obj", "texture/fish2.bmp", &SceCont));
	my_fishes.push_back(Fish(glm::vec3(0.0f, 0.5f, -1.0f), 0.1f, fishbowl, "obj/fish_test2.obj", "texture/fish3.bmp", &SceCont));
	my_fishes.push_back(Fish(glm::vec3(2.0f, 1.0f, 0.0f), 0.1f, fishbowl, "obj/fish_test2.obj", "texture/fish4.bmp", &SceCont));
	my_fishes.push_back(Fish(glm::vec3(4.0f, 0.5f, 1.0f), 0.1f, fishbowl, "obj/fish_test2.obj", "texture/fish5.bmp", &SceCont));
	my_fishes.push_back(Fish(glm::vec3(4.0f, -1.0f, -3.0f), 0.1f, fishbowl, "obj/fish_test2.obj", "texture/fish.bmp", &SceCont));
	my_fishes.push_back(Fish(glm::vec3(2.0f, -0.5f, -2.0f), 0.1f, fishbowl, "obj/fish_test2.obj", "texture/fish2.bmp", &SceCont));
	my_fishes.push_back(Fish(glm::vec3(0.0f, -0.5f, 1.0f), 0.1f, fishbowl, "obj/fish_test2.obj", "texture/fish3.bmp", &SceCont));
	my_fishes.push_back(Fish(glm::vec3(-2.0f, 1.0f, 0.0f), 0.1f, fishbowl, "obj/fish_test2.obj", "texture/fish4.bmp", &SceCont));
	my_fishes.push_back(Fish(glm::vec3(-4.0f, 0.5f, 1.0f), 0.1f, fishbowl, "obj/fish_test2.obj", "texture/fish5.bmp", &SceCont));

	SceCont.useShader();

	// for FPS controlling
	double last_time = glfwGetTime();

	srand(time(NULL));

	// add fishes to fishbowl and save fishbowl in fish instances
	for (std::vector<Fish>::iterator it = my_fishes.begin(); it < my_fishes.end(); it++) {
		Fish& tmp = *it;
		fishbowl->add_position(&tmp);
	}

	while (!glfwWindowShouldClose(window)) {

		// limiting FPS
		bool go = false;
		do {
			double diff = abs(glfwGetTime() - last_time);
			if (diff > (MAX_FPS / 1000.0)) {
				go = true;
			}
		} while (!go);
		last_time = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SceCont.setPerspective(45.0f, 0.1f, 100.0f);
		SceCont.setCamPos(contKey.getCamPos());
		SceCont.setLightPos(0.0, contKey.getLightY(), 0.0);

		// enable transpancy and disable depthtest
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		// drawing glass
		for (std::vector<Artifact>::iterator it = my_glasses.begin(); it < my_glasses.end(); it++) {
			Artifact& tmp = *it;
			tmp.draw();
		}

		// disable transparency and enable depth test
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// drawing all the other stuff
		for (vector<Artifact>::iterator it = my_stuff.begin(); it < my_stuff.end(); it++) {
			Artifact& tmp = *it;
			tmp.draw();
		}

		// calculate new positions for fishes (move) and then draw them
		for (std::vector<Fish>::iterator it = my_fishes.begin(); it < my_fishes.end(); it++) {
			Fish& tmp = *it;
			tmp.move();
			tmp.draw();
		}

		// wiggle and draw plants
		for (vector<Plant>::iterator it = my_plants.begin(); it < my_plants.end(); it++) {
			Plant& tmp = *it;
			tmp.wiggle(movePlant.getX(), movePlant.getY(), movePlant.getZ());
			tmp.draw();
		}

		movePlant.moveX();
		movePlant.moveY();
		movePlant.moveZ();

		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	SceCont.deleteProgramm();
	glfwTerminate();
	return 0;
}