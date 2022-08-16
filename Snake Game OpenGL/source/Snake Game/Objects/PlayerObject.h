#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <vector>
#include <unordered_map>

#include "GameObject.h"
#include "../Render/CubeRenderer.h"
#include "../Utilities/Texture.h"

class PlayerObject {
public:
	const int STANDART_PLAYER_SIZE{ 6 };
	struct PlayerBlock {
		GameObject body;
		glm::vec2 velocity;
		glm::vec2 TexCoords;
	};
	enum PlayerParts {
		PLAYER_HEAD,
		PLAYER_TAIL,
		PLAYER_BODY,
		PLAYER_CURVED_LEFT,
		PLAYER_CURVED_RIGHT,
		PLAYER_CURVED_UP,
		PLAYER_CURVED_DOWN
	};

	PlayerObject(const glm::vec2 first_block_position, const glm::vec2 size, const glm::vec2 start_velocity);

	void CreateStandartPlayer();

	void DrawPlayer(CubeRenderer* renderer, Texture& texture);

	void UpdatePlayer();
	void AssignVelocities();
	void Move();

	void SetNextVelocity(const glm::vec2 NextVelocity);
	void DecideTextureAtlas();

	void AddNewBlock();

	int GetLength() const;
	const std::vector<PlayerBlock>& GetPlayerBlocks() const;
	PlayerBlock& GetPlayerHead();
private:
	std::vector<PlayerBlock> PlayerBlocks;
	std::vector<std::unordered_map<glm::vec2, glm::vec2>> TextureAtlas;
	const glm::vec2 block_size;

	const glm::vec2 VELOCITY_UP{ 0.0, -block_size.x };
	const glm::vec2 VELOCITY_DOWN{ 0.0, block_size.x };
	const glm::vec2 VELOCITY_LEFT{ -block_size.x, 0.0 };
	const glm::vec2 VELOCITY_RIGHT{ block_size.x, 0.0 };
	glm::vec2 NextVelocity;

	void Init(const glm::vec2 first_block_position, const glm::vec2 size, const glm::vec2 start_velocity);
};