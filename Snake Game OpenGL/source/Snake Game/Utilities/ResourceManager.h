#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <map>

#include "../vendor/stb_image.h"

#include "Shader.h"
#include "Texture.h"

class ResourceManager {
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;

	static void LoadShaderFile(const std::string vertexFile, const std::string fragmentFile, const std::string name);
	static Shader& GetShader(const std::string name);
	static void LoadTextureFile(const std::string path, const std::string name);
	static Texture& GetTexture(const std::string name);
private:
	ResourceManager();
};
