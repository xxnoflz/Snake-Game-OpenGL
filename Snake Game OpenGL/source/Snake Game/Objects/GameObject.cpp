#include "GameObject.h"

GameObject::GameObject(const glm::vec2 postion, const glm::vec2 size) : m_position(postion), m_size(size) {}

void GameObject::DrawObject(CubeRenderer* renderer, Texture& texture, glm::vec2 AtlasPosition) {
	texture.BindTexture();
	renderer->DrawCube(this->m_position, this->m_size, texture.GetFromTextureAtlas(AtlasPosition.x, AtlasPosition.y, 16));
}