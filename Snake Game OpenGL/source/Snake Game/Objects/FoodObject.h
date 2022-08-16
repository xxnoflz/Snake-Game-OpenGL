#pragma once

#include "GameObject.h"

#include <glm/glm.hpp>

#include <random>

class FoodObject : public GameObject {
public:
	FoodObject(const glm::vec2 size, const uint32_t window_width, const uint32_t window_height);

	void CreateNewFood(uint32_t window_width, uint32_t window_height);
};