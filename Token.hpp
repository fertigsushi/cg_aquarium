#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "objloader.hpp"
#include "texture.hpp"
#include "SceneControl.hpp"

using namespace glm;
using namespace std;

/**
* Klasse zum einlesen von Obj-Files und Texturen
* Hat versch. draw Methoden fuer Obj inkl.
* vorherige Model transformationen, rotationen.
* Beinhaltet die mat4 Model, MVP und die
* sendMVP() Methode fuer jedes Obj.
*/
class Token {
public:
	Token(const char*, const char*, SceneControl*, bool isRGBA = false);
	void draw();
	void wiggle(float x, float y, float z);
	glm::vec3 get_center_of_mass();
	void sendMVP();

protected:
	void setModelToOrigin();
	void create();
	
	SceneControl* scene;
	mat4 MVP;
	mat4 Model;
	vector<vec3> vertices; // Buffer
	vector<vec2> uvs; // Buffer
	vector<vec3> normals; // Buffer
	GLuint programID;
	GLuint VertexArrayIDObj; // Obj
	GLuint TexturObj; // Textur
	const char* imagepath; // Textur path
	bool res; // TexturLoader loadDDS or loadBMP
	glm::vec3 center_of_mass;
};

#endif // TOKEN_H