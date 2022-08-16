#include "Texture.h"

Texture::Texture(unsigned char* data, int width, int height, int format) : width(width), height(height), format(format) {
	this->Init(data);
}

void Texture::Init(unsigned char* data) {
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);

	glTexImage2D(GL_TEXTURE_2D, 0, this->format, this->width, this->height, 0, this->format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::BindTexture() const {
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

std::vector<float> Texture::GetFromTextureAtlas(float x, float y, float SpriteSize) {
	return std::vector<float> {
		(x * SpriteSize) / this->width, (y * SpriteSize) / this->height,
		((x + 1) * SpriteSize) / this->width, (y * SpriteSize) / this->height,
		(x * SpriteSize) / this->width, ((y + 1) * SpriteSize) / this->height,

		((x + 1) * SpriteSize) / this->width, (y * SpriteSize) / this->height,
		((x + 1) * SpriteSize) / this->width, ((y + 1) * SpriteSize) / this->height,
		(x * SpriteSize) / this->width, ((y + 1) * SpriteSize) / this->height,
	};
}