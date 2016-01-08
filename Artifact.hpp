#pragma once
#include "token.hpp"

class Artifact : public Token {
public:
	Artifact(const char* path, const char* imagepath, GLuint progID, bool isRGBA = false);
	~Artifact(void);
};

