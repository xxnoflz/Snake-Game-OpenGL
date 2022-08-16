#include "CubeRenderer.h"

CubeRenderer::CubeRenderer() {
	this->Init();
}

void CubeRenderer::Init() {
	std::vector<float> vertices{
		//Vertices position
		0.0, 0.0,																																			
		1.0, 0.0,
		0.0, 1.0,

		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);
	//vertices buffer
	glGenBuffers(1, &vertex_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	//texture buffer
	glGenBuffers(1, &this->texture_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->texture_VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CubeRenderer::DrawCube(const glm::vec2 position, const glm::vec2 size, const std::vector<float> textureCoords) const{
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->texture_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(float), &textureCoords[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glm::mat4 model{ glm::mat4(1.0) };
	model = glm::translate(model, glm::vec3(position, 0.0));
	model = glm::scale(model, glm::vec3(size, 1.0));

	ResourceManager::GetShader("cubeShader").use();
	ResourceManager::GetShader("cubeShader").SetMat4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}