#include "ResourceManager.h"

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture> ResourceManager::Textures;

void ResourceManager::LoadShaderFile(const std::string vertexFile, const std::string fragmentFile, const std::string name) {
	std::ifstream vertexFileStream{ vertexFile };
	std::stringstream vertexStream;
	vertexStream << vertexFileStream.rdbuf();
	const std::string vertexCode{ vertexStream.str() };

	std::ifstream fragmentFileStream{ fragmentFile };
	std::stringstream fragmentStream;
	fragmentStream << fragmentFileStream.rdbuf();
	const std::string fragmentCode{ fragmentStream.str() };

	Shader shader;
	shader.CreateShaderProgram(vertexCode, fragmentCode);
	Shaders[name] = shader;
}

Shader& ResourceManager::GetShader(const std::string name) {
	return Shaders[name];
}

void ResourceManager::LoadTextureFile(const std::string path, const std::string name) {
	int width; 
	int height;
	int nrChannels;
	unsigned char* data{ stbi_load(path.c_str(), &width, &height, &nrChannels, 0) };
	Texture texture{ data, width, height };
	Textures[name] = texture;
}

Texture& ResourceManager::GetTexture(const std::string name) {
	return Textures[name];
}