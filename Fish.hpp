#pragma once
#include "token.hpp"

class Fish : public Token {
public:
	Fish(const char* path, const char* imagepath, GLuint progID);
	~Fish(void);
	glm::mat4 get_center_of_mass();
	void move();

private:
	float visual_range;
};

