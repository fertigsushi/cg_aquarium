#include "Artifact.hpp"

/**
* Creates an instance of this object and calls the super constructor.
*/
Artifact::Artifact(const char* path, const char* imagepath, SceneControl* scene, bool isRGBA)
	: Token (path, imagepath, scene, isRGBA) {
}
