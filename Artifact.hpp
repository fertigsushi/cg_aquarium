#ifndef ARTIFACT_H
#define ARTIFACT_H

#pragma once
#include "token.hpp"

class Artifact : public Token {
public:
	Artifact(const char* path, const char* imagepath, SceneControl*, bool isRGBA = false);
};

#endif // ARTIFACT_H