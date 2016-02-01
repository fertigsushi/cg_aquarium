#ifndef PLANT_H
#define PLANT_H

#pragma once
#include "token.hpp"

class Plant : public Token {
public:
	Plant(const char*, const char*, SceneControl*);
};

#endif// PLANT_H