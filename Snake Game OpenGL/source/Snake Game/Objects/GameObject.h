#pragma once

#include <glm/glm.hpp>

#include "../Render/CubeRenderer.h"
#include "../Utilities/Texture.h"

class GameObject {
public:
	GameObject(const glm::vec2 postion, const glm::vec2 size);
	void DrawObject(CubeRenderer* renderer, Texture& texture, glm::vec2 AtlasPostion);
public:
	glm::vec2 m_position;
	glm::vec2 m_size;
};