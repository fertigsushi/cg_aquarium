#include "Token.hpp"

/*
* Fuer nicht transparente Texturen
*/
Token::Token(const char* path, const char* imagepath, GLuint progID, bool isRGBA) {
	programID = progID;
	Model = mat4(1.0f);
	VertexArrayIDObj = 0;
	res = loadOBJ(path, vertices, uvs, normals);
	loadTexture(isRGBA, imagepath);
	create();
}

Token::~Token(void) {
}

/**
* Fuer statische Obj
*/
void Token::draw(mat4& Projection , mat4& View ) {
	setModelToOrigin();
	sendMVP(Projection, View);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexturObj);
	glUniform1i(glGetUniformLocation(programID, "myTexturSampler"), 0);
	glBindVertexArray(VertexArrayIDObj);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

/**
* Fuer statische transparente Obj
*/
void Token::draw(bool isRGBA, mat4& Projection , mat4& View ) {
	setModelToOrigin();
	sendMVP(Projection, View);

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

/**
* Fuer dynamische Obj, davor muss obj.wiggle( ... ) ausgefuehrt werden!
*/
void Token::draw() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexturObj);
	glUniform1i(glGetUniformLocation(programID, "myTexturSampler"), 0);
	glBindVertexArray(VertexArrayIDObj);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

/**
* Fuer Fische
* Siehe Klasse MoveControl
*/
void Token::wiggle(float x, float y, float z, float yRotate, mat4& Projection , mat4& View) {
	setModelToOrigin();
	wiggleRotate(yRotate);
	Model = translate(Model, vec3(x, y, z));
	sendMVP(Projection , View);
}

/**
* Fuer Pflanzen
* Siehe Klasse MoveControl
*/
void Token::wiggle(float x, float y, float z, mat4& Projection, mat4& View) {
	setModelToOrigin();
	Model = rotate(Model, y, vec3(0.0, 1.0, 0.0));
	Model = rotate(Model, z, vec3(0.0, 0.0, 1.0));
	Model = rotate(Model, x, vec3(1.0, 0.0, 0.0));
	sendMVP(Projection , View);
}

/**
* Getter fuer Model
*/
mat4 Token::getModel() {
	return Model;
}

void Token::setModelToOrigin() {
	Model = mat4(1.0f);
}

void Token::sendMVP(mat4& Projection, mat4& View ) {
	MVP = Projection * View * Model; 
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
}

void Token::wiggleRotate(float yRotate) {
	Model = rotate(Model, yRotate, vec3(0.0, 1.0, 0.0));
}

void Token::create() {
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

void Token::loadTexture(bool isRGBA, const char* imagepath) {
	if (isRGBA == false) {
		TexturObj = loadBMP_custom(imagepath);
	} else {
		TexturObj = loadDDS(imagepath);
	}
}
