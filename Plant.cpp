#include "Plant.hpp"

/**
* Creates an instance of this object and calls the super constructor.
*/
Plant::Plant(const char* path, const char* imagepath, SceneControl* scene)
	: Token(path, imagepath, scene) {
}