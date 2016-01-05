/**
* CG - Aquarium
* =============
* Steuerung ==> Mit den "Pfeiltasten"
* Andere Interaktionsmoeglichkeiten ==> Zum Beenden "ESC"
*...
*
* @author Julian Osmer, 542530; name, matr_nr; 
*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "KeyControl.hpp"
#include "MoveControl.hpp"
#include "SceneControl.hpp"
#include "Token.hpp"
//#include "julianheader.hpp"

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
	//glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	SceneControl SceCont(aspectRatio);

	MoveControl moveFish1(0.0, 6.0, 0.7, 0.6, 0.6);
	MoveControl moveFish2(-3.0, 5.5, 0.7, 0.6, 0.6);
	MoveControl moveFish3(-5.5, 1.0, 0.7, 0.3, 0.6);
	MoveControl moveFish4(0.0, 2.0, 0.4, 0.3, 0.6);
	MoveControl moveFish5(-0.5, 5.0, 0.2, 0.4, 0.6);
	MoveControl moveKugelFish(0.0, 3.0, 0.6, 0.4, 0.6);
	MoveControl movePlant(1.0);
	
	Token fish("obj/fish.obj", "texture/fish.bmp", SceCont.getProgID()); // Schwimmt nach Rechts
	Token fishBack("obj/fishBack.obj", "texture/fish.bmp", SceCont.getProgID()); // Schwimmt nach Links
	
	Token fish2("obj/fish2.obj", "texture/fish2.bmp", SceCont.getProgID()); // Schwimmt nach Rechts
	Token fish2Back("obj/fish2Back.obj", "texture/fish2.bmp", SceCont.getProgID()); // Schwimmt nach Links
	
	Token fish3("obj/fish3.obj", "texture/fish3.bmp", SceCont.getProgID()); // Schwimmt nach Rechts
	Token fish3Back("obj/fish3Back.obj", "texture/fish3.bmp", SceCont.getProgID()); // Schwimmt nach Links
	
	Token fish4("obj/fish4.obj", "texture/fish4.bmp", SceCont.getProgID()); // Schwimmt nach Rechts
	Token fish4Back("obj/fish4Back.obj", "texture/fish4.bmp", SceCont.getProgID()); // Schwimmt nach Links

	Token fish5("obj/fish5.obj", "texture/fish5.bmp", SceCont.getProgID()); // Schwimmt nach Rechts
	Token fish5Back("obj/fish5Back.obj", "texture/fish5.bmp", SceCont.getProgID()); // Schwimmt nach Links
	
	Token kugelFish("obj/kugelFish.obj", "texture/kugelFish.bmp", SceCont.getProgID()); // Schwimmt nach Rechts
	Token kugelFishBack("obj/kugelFishBack.obj", "texture/kugelFish.bmp", SceCont.getProgID()); // Schwimmt nach Links

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
		
		SceCont.setPerspective(45.0f, 0.1f, 100.0f);
		SceCont.setCamPos(contKey.getCamPos());
		SceCont.setLightPos(0.0, contKey.getLightY(), 0.0);

		tisch.draw(SceCont.getProj(), SceCont.getView()); // Vor Glas zeichnen
		
		// Reihenfolge bei Glas koennte wahrsch. auswrikungen haben
		glassB2.draw(/*RGBA*/true, SceCont.getProj(), SceCont.getView());
		glassB1.draw(/*RGBA*/true, SceCont.getProj(), SceCont.getView());
		glassA2.draw(/*RGBA*/true, SceCont.getProj(), SceCont.getView());
		glassA1.draw(/*RGBA*/true, SceCont.getProj(), SceCont.getView());
		
		// Ab hier alles nach Glas zeichnen
		aquar.draw(SceCont.getProj(), SceCont.getView());
		ground.draw(SceCont.getProj(), SceCont.getView()); 
		boot.draw(SceCont.getProj(), SceCont.getView());
		truhe.draw(SceCont.getProj(), SceCont.getView());
		coral.draw(SceCont.getProj(), SceCont.getView());
		huegel.draw(SceCont.getProj(), SceCont.getView());
		
		kugelFishBack.wiggle(moveKugelFish.getX(), moveKugelFish.getY(), moveKugelFish.getZ(), moveKugelFish.getRotateY(), SceCont.getProj(), SceCont.getView());
		kugelFish.wiggle(moveKugelFish.getX(), moveKugelFish.getY(), moveKugelFish.getZ(), moveKugelFish.getRotateY(), SceCont.getProj(), SceCont.getView());
		
		if (moveKugelFish.getIsWiggleLeft()) {
			kugelFishBack.draw(); // Schwimmt nach Links 
		} else { 
			kugelFish.draw(); // Schwimmt nach Rechts
		}

		fish5Back.wiggle(moveFish5.getX(), moveFish5.getY(), moveFish5.getZ(), moveFish5.getRotateY(), SceCont.getProj(), SceCont.getView());
		fish5.wiggle(moveFish5.getX(), moveFish5.getY(), moveFish5.getZ(), moveFish5.getRotateY(), SceCont.getProj(), SceCont.getView());
		
		if (moveFish5.getIsWiggleLeft()) {
			fish5Back.draw(); // Schwimmt nach Links 
		} else { 
			fish5.draw(); // Schwimmt nach Rechts
		}

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

		moveFish5.moveX();
		moveFish5.moveY();
		moveFish5.moveZ();
		moveFish5.rotateY();

		moveKugelFish.moveX();
		moveKugelFish.moveY();
		moveKugelFish.moveZ();
		moveKugelFish.rotateY();

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

