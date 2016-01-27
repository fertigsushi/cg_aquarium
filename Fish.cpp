#include <iostream>
#include "Fish.hpp"

/**
* Creates an instance of this object and calls the super constructor. Saves also the spwaning point and the visual range.
*/
Fish::Fish(glm::vec3 spawn, float visual_range, Fishbowl* fishbowl, const char* path, const char* imagepath, SceneControl* scene)
	: Token(path, imagepath, scene) {
		this->visual_range = visual_range;
		this->speed = 0.01f;
		center_of_mass = spawn;
		this->bowl = fishbowl;
		this->new_target();
}

/**
* Calculates the next position of this fish.
*/
void Fish::move() {
	setModelToOrigin();
	check_for_other();
	// check if target is nearly reached. the right term means 30 percent of visual range
	if ((glm::distance(target, center_of_mass) / visual_range) < 0.3) {
		this->new_target();
	}
	center_of_mass += movement;
	Model = translate(Model, center_of_mass);
	Model = rotate(Model, angle_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
}

/**
* Checks if there is another fish in front of this one. If yes, this fish will get a new target.
*/
void Fish::check_for_other() {
	vector<Fish *>* fishes = bowl->get_positions();
	for(vector<Fish *>::iterator it = fishes->begin(); it < fishes->end(); it++) {
		Fish* fish = *it;
		glm::vec3 pos = fish->get_center_of_mass();
		if (pos != center_of_mass) {
			bool collision = true;
			do {
				glm::vec3 max_view = center_of_mass + (movement / glm::vec3(visual_range));
				collision = abs(glm::distance(pos, max_view)) < visual_range ? true : false;
				if (collision) {
					this->new_target();
				}
			} while (collision);
		}
	}
}

/**
* Breaks the movementvector down, so that it's length is the value of speed.
*/
void Fish::normalize_movement_vec() {
	float len = sqrt(pow(movement.x, 2) + pow(movement.y, 2) + pow(movement.z, 2));
	movement = glm::vec3(movement.x / len, movement.y / len, movement.z / len);
	movement *= speed;
}

/**
* Gives a random float value between the two given back.
*/
float Fish::random_float(float min, float max) {
	return min + ((float) rand()) / ((float) RAND_MAX / (max - min));    
}

/**
* Generates a new target and calculates the movementdirection and it's angle.
*/
void Fish::new_target() {
	target = glm::vec3(
		random_float(-5.1f, 5.1f),
		random_float(-1.5f, 1.8f),
		random_float(-3.0f, 3.5f)
		);
	movement = target - center_of_mass;
	normalize_movement_vec();
	angle_rotation = atan(-1 * movement.z / movement.x) * 57.2958f;
	angle_rotation += movement.x < 0 ? 180.0f : 0.0f; // for heading left, dont knwo, why tangens doesnt work there :/
	printf("new target for %s\n\tx: %.4f,\n\ty: %.4f,\n\tz: %.4f\n", imagepath, target.x, target.y, target.z);
}