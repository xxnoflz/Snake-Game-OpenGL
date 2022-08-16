#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Utilities/ResourceManager.h"
#include "../Utilities/Texture.h"
#include "../Utilities/Shader.h"

#include <vector>

class GridRenderer {
public:
	GridRenderer();

	void Draw(const uint32_t window_width, uint32_t window_height, Texture& texture, const float block_size, Shader& shader);
	//void Draw(const uint32_t window_width, const uint32_t window_height, const float block_size);
	//void DrawLine(std::vector<float>& vertices);
private:
	GLuint VAO;
	GLuint vertex_VBO;
	GLuint texture_VBO;

	void Init();
};
