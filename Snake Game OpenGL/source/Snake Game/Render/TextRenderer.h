#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <vector>
#include <map>

#include "../Utilities/Shader.h"

class TextRenderer {
public:
	struct Character {
		GLuint TextureID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		uint32_t Advance;
	};

	TextRenderer(const std::string fontPath);

	void Init();

	void Draw(const std::string text, glm::vec2 position, const float scale, Shader& shader);
private:
	GLuint VAO;
	GLuint VBO;

	std::map<char, Character> Characters;
};