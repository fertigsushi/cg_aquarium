#pragma once
#include "token.hpp"

class Plant : public Token {
public:
	Plant(const char* path, const char* imagepath, GLuint progID);
	~Plant(void);
};

