#ifndef FISHBOWL_H
#define FISHBOWL_H

class Fish;

#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Fish.hpp"

class Fishbowl {
public:
	static Fishbowl* create(glm::vec3, glm::vec3);
	static Fishbowl* get_fishbowl();
	glm::vec3* get_min();
	glm::vec3* get_max();
	void add_position(Fish *);
	std::vector<Fish *>* get_positions();

private:
	Fishbowl(glm::vec3 min, glm::vec3 max);
	Fishbowl(){}
	Fishbowl(const Fishbowl&){}

	static Fishbowl* _instance;
	glm::vec3 min;
	glm::vec3 max;
	std::vector<Fish *> positions;
};

#endif // FISHBOWL_H