#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <string>

class Shader {
public:
	Shader();

	void CreateShaderProgram(const std::string& vertexCode, const std::string& fragmentCode);

	void use() const;

	void SetMat4(const std::string& name, glm::mat4 value) const;
private:
	GLuint ID;
};