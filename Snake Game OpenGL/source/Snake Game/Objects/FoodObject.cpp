#include "FoodObject.h"

FoodObject::FoodObject(const glm::vec2 size, const uint32_t window_width, const uint32_t window_height) : GameObject(glm::vec2(), size) {
	this->CreateNewFood(window_width, window_height);
}

void FoodObject::CreateNewFood(const uint32_t window_width, const uint32_t window_height) {
	std::random_device rd;
	std::mt19937 gen{ rd() };
	std::uniform_int_distribution<> randomX{ 0, int(window_width - this->m_size.x) / (int)this->m_size.x };
	std::uniform_int_distribution<> randomY{ 0, int(window_height - this->m_size.y) / (int)this->m_size.y };
	this->m_position = glm::vec2(randomX(gen) * this->m_size.x, randomY(gen) * this->m_size.y);
}