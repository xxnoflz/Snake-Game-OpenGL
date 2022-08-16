#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "../Utilities/ResourceManager.h"

class CubeRenderer {
public:
	CubeRenderer();

	void DrawCube(const glm::vec2 position, const glm::vec2 size, const std::vector<float> textureCoords) const;
private:
	GLuint VAO;
	GLuint vertex_VBO;
	GLuint texture_VBO;

	void Init();
};