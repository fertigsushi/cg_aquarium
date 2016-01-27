#pragma once
#include "token.hpp"

class Plant : public Token {
public:
	Plant(const char*, const char*, SceneControl*);
};