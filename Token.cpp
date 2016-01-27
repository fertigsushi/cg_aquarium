#include "Token.hpp"

/**
* Saves important data on this token. It will also load the texture.
*/
Token::Token(const char* path, const char* image_path, SceneControl* scene, bool isRGBA) {
	this->imagepath = image_path;
	this->programID = scene->getProgID();
	this->scene = scene;
	Model = glm::mat4(1.0f);
	res = loadOBJ(path, vertices, uvs, normals);
	if (isRGBA == false) {
		TexturObj = loadBMP_custom(imagepath);
	} else {
		TexturObj = loadDDS(imagepath);
	}
	create();
}

glm::vec3 Token::get_center_of_mass() {
	return center_of_mass;
}

/**
* Fuer dynamische Obj, davor muss obj.wiggle( ... ) ausgefuehrt werden!
*/
void Token::draw() {
	sendMVP();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TexturObj);
	glUniform1i(glGetUniformLocation(programID, "myTexturSampler"), 0);
	glBindVertexArray(VertexArrayIDObj);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

/**
* Fuer Pflanzen
* Siehe Klasse MoveControl
*/
void Token::wiggle(float x, float y, float z) {
	setModelToOrigin();
	Model = rotate(Model, y, vec3(0.0, 1.0, 0.0));
	Model = rotate(Model, z, vec3(0.0, 0.0, 1.0));
	Model = rotate(Model, x, vec3(1.0, 0.0, 0.0));
	sendMVP();
}

/**
* Resets the model.
*/
void Token::setModelToOrigin() {
	Model = mat4(1.0f);
}

/**
* Creates the MVP matrix and sends it.
*/
void Token::sendMVP() {
	MVP = scene->getProj() * scene->getView() * Model; 
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &scene->getView()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
}

/**
* Loads vertex, normals and uvs vectors.
*/
void Token::create() {
	GLuint vertexbuffer;
	GLuint normalbuffer;
	GLuint uvsbuffer;

	glGenVertexArrays(1, &VertexArrayIDObj);
	glBindVertexArray(VertexArrayIDObj);
	// bind vertex
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// bind normals
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// bind uvs
	glGenBuffers(1, &uvsbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvsbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
}
