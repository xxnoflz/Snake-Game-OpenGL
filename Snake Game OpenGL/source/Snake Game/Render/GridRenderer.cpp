#include "GridRenderer.h"

GridRenderer::GridRenderer() {
	this->Init();
}

void GridRenderer::Init() {
	std::vector<float> vertices{
		//Vertices position
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0,

		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};

	glGenBuffers(1, &this->vertex_VBO);
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glGenBuffers(1, &this->texture_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->texture_VBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GridRenderer::Draw(const uint32_t window_width, uint32_t window_height, Texture& texture, const float block_size, Shader& shader) {
	texture.BindTexture();
	std::vector<float> textureArray{ texture.GetFromTextureAtlas(3, 3, 16) };
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->texture_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(float), &textureArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader.use();

	std::vector<float> vertices;
	for (float x{}; x < window_width; x += block_size) {
		for (float y{}; y < window_height; y += block_size) {
			glm::mat4 model{ glm::mat4(1.0)};
			model = glm::translate(model, glm::vec3(x, y, 0.0));
			model = glm::scale(model, glm::vec3(block_size, block_size, 1.0));
			shader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}

//void GridRenderer::Draw(const uint32_t window_width, const uint32_t window_height, const float block_size) {
//	std::vector<float> vertices{};
//
//	ResourceManager::GetShader("gridShader").use();
//	glBindVertexArray(this->VAO);
//	for (float x_axis{ block_size }; x_axis <= window_width; x_axis += block_size) {
//		vertices = {
//			x_axis,				 0.0,
//			x_axis,				 (float)window_height
//		};
//		this->DrawLine(vertices);
//	}
//	for (float y_axis{ block_size }; y_axis <= window_height; y_axis += block_size) {
//		vertices = {
//			0.0,				 y_axis,
//			(float)window_width, y_axis
//		};
//		this->DrawLine(vertices);
//	}
//	glBindVertexArray(0);
//}
//
//void GridRenderer::DrawLine(std::vector<float>& vertices) {
//	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(float), &vertices[0]);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	glDrawArrays(GL_LINES, 0, 2);
//}
