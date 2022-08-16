#include "TextRenderer.h"

TextRenderer::TextRenderer(const std::string fontPath) : VAO(), VBO() {
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		return;
	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
		return;

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c{}; c < 128; ++c) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			continue;

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character{
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(uint32_t)face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	this->Init();
}

void TextRenderer::Init() {
	glGenBuffers(1, &this->VBO);
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextRenderer::Draw(const std::string text, glm::vec2 position, const float scale, Shader& shader) {
	shader.use();
	glBindVertexArray(this->VAO);
	glActiveTexture(GL_TEXTURE0);
	for (std::string::const_iterator iter{ text.begin() }; iter != text.end(); ++iter) {
		Character c{ Characters[*iter] };

		float xPos{ position.x + c.Bearing.x * scale };
		float yPos{ position.y + (this->Characters['H'].Bearing.y - c.Bearing.y) * scale};

		float width{ c.Size.x * scale };
		float height{ c.Size.y * scale };

		std::vector<float> vertices{
			xPos,			yPos + height,	0.0, 1.0,
			xPos + width,	yPos,			1.0, 0.0,
			xPos,			yPos,			0.0, 0.0,

			xPos,			yPos + height,	0.0, 1.0,
			xPos + width,	yPos + height,	1.0, 1.0,
			xPos + width,	yPos,			1.0, 0.0
		};

		glBindTexture(GL_TEXTURE_2D, c.TextureID);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 6 * 4, &vertices[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		position.x += (c.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}