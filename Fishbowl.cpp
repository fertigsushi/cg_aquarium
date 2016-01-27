#include "Fishbowl.hpp"

Fishbowl* Fishbowl::_instance = 0;

/**
* Creates a new instance of this singleton.
*/
Fishbowl::Fishbowl(glm::vec3 min, glm::vec3 max) {
	this->min = min;
	this->max = max;
}

/**
* Creates the fishbowl and returns a pointer to this instance.
*/
Fishbowl* Fishbowl::create(glm::vec3 min, glm::vec3 max) {
	if (Fishbowl::_instance == 0) {
		_instance = new Fishbowl(min, max);
	}
	return _instance;
}

/**
* Returns a pointer to this instance.
*/
Fishbowl* Fishbowl::get_fishbowl() {
	if (Fishbowl::_instance == 0) {
		return NULL;
	} else {
		return _instance;
	}
}

/**
* Returns a pointer to the vector in which the lower borders of this foshbowl are stored.
*/
glm::vec3* Fishbowl::get_min() {
	return &min;
}

/**
* Returns a pointer to the vector in which the upper borders of this fishbowl are stored.
*/
glm::vec3* Fishbowl::get_max() {
	return &max;
}

/**
* Adds a pointer to a fish to the list.
*/
void Fishbowl::add_position(Fish *fish) {
	positions.push_back(fish);
}

/**
* Returns a pointer to a list with pointers to all fishes who are in this fishbowl.
*/
std::vector<Fish *>* Fishbowl::get_positions() {
	return &positions;
}