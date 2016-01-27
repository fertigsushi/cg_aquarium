#ifndef FISH_H
#define FISH_H

class Fishbowl;

#pragma once
#include "token.hpp"
#include "Fishbowl.hpp"

class Fish : public Token {
public:
	Fish(glm::vec3, float visual_range, Fishbowl*, const char* path, const char* imagepath, SceneControl*);
	void move();

protected:
	void normalize_movement_vec();
	float random_float(float, float);
	void new_target();
	void check_for_other();

	Fishbowl* bowl;
	float visual_range;
	float speed;
	float angle_rotation;
	glm::vec3 movement;
	glm::vec3 target;
};

#endif // FISH_H