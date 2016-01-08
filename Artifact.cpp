#include "Artifact.hpp"


Artifact::Artifact(const char* path, const char* imagepath, GLuint progID, bool isRGBA)
	: Token (path, imagepath, progID, isRGBA) {
}


Artifact::~Artifact(void)
{
}
