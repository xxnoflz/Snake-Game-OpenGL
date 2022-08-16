#pragma once
#include <glad/glad.h>

#include <vector>

class Texture {
public:
	Texture(unsigned char* data = nullptr, int width = 0, int height = 0, int format = GL_RGBA);

	void BindTexture() const;
	std::vector<float> GetFromTextureAtlas(float x, float y, float SpriteSize);
private:
	GLuint ID;
	int width;
	int height;
	int format;

	void Init(unsigned char* data);
};