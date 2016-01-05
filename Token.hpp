#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "objloader.hpp"
#include "texture.hpp"

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
	Token(const char * path, const char * imagepath, GLuint progID);
	Token(const char * path, const char * imagepath, bool isRGBA, GLuint progID);
	~Token();
	void draw(mat4& Projection , mat4& View );
	void draw(bool isRGBA, mat4& Projection , mat4& View );
	void draw();
	void wiggle(float x, float y, float z, float yRotate, mat4& Projection , mat4& View);
	void wiggle(float x, float y, float z, mat4& Projection , mat4& View);
	mat4 getModel();

private:
	void setModelToOrigin();
	void sendMVP(mat4& Projection , mat4& View );
	void wiggleRotate(float yRotate);
	void create();
	
	mat4 MVP;
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

