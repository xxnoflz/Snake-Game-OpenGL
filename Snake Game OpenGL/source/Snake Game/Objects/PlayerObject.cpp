#include "PlayerObject.h"

PlayerObject::PlayerObject(const glm::vec2 first_block_position, const glm::vec2 size, const glm::vec2 start_velocity) 
: NextVelocity(start_velocity), block_size(size) {
	this->Init(first_block_position, size, start_velocity);
}

void PlayerObject::Init(const glm::vec2 first_block_position, const glm::vec2 size, const glm::vec2 start_velocity) {
	this->TextureAtlas.push_back({			//HEAD
		{VELOCITY_UP, glm::vec2(0, 0)},			//UP
		{VELOCITY_DOWN, glm::vec2(2, 0)},		//DOWN
		{VELOCITY_LEFT, glm::vec2(3, 0)},		//LEFT
		{VELOCITY_RIGHT, glm::vec2(1, 0)}		//RIGHT
	});
	this->TextureAtlas.push_back({			//TAIL
		{VELOCITY_UP, glm::vec2(0, 1)},			//UP
		{VELOCITY_DOWN, glm::vec2(2, 1)},		//DOWN
		{VELOCITY_LEFT, glm::vec2(3, 1)},		//LEFT
		{VELOCITY_RIGHT, glm::vec2(1, 1)}		//RIGHT
	});
	this->TextureAtlas.push_back({			//BODY
		{VELOCITY_UP, glm::vec2(0, 3)},			//UP
		{VELOCITY_DOWN, glm::vec2(0, 3)},		//DOWN
		{VELOCITY_LEFT, glm::vec2(1, 3)},		//LEFT
		{VELOCITY_RIGHT, glm::vec2(1, 3)}		//RIGHT
	});

	this->TextureAtlas.push_back({			//BODY CURVED LEFT
		{VELOCITY_UP, glm::vec2(0, 2)},			//UP
		{VELOCITY_DOWN, glm::vec2(1, 2)}		//DOWN
	});
	this->TextureAtlas.push_back({			//BODY CURVED RIGHT
		{VELOCITY_UP, glm::vec2(3, 2)},			//UP
		{VELOCITY_DOWN, glm::vec2(2, 2)}		//DOWN
	});
	this->TextureAtlas.push_back({			//BODY CURVED UP
		{VELOCITY_LEFT, glm::vec2(2, 2)},		//LEFT
		{VELOCITY_RIGHT, glm::vec2(1, 2)}		//RIGHT
	});
	this->TextureAtlas.push_back({			//BODY CURVED DOWN
		{VELOCITY_LEFT, glm::vec2(3, 2)},		//LEFT
		{VELOCITY_RIGHT, glm::vec2(0, 2)}		//RIGHT
	});

	PlayerBlocks.push_back(PlayerBlock{
		GameObject(first_block_position, size),
		start_velocity,
		this->TextureAtlas[PLAYER_HEAD][start_velocity]
	});
	this->CreateStandartPlayer();
}

void PlayerObject::CreateStandartPlayer() {
	for (int iter{ 1 }; iter < STANDART_PLAYER_SIZE; ++iter) {
		if(iter == STANDART_PLAYER_SIZE - 1)
			PlayerBlocks.push_back(PlayerBlock{
				GameObject(PlayerBlocks[iter - 1].body.m_position , block_size),
				PlayerBlocks[iter - 1].velocity,
				this->TextureAtlas[PLAYER_TAIL][PlayerBlocks[iter - 1].velocity]
			});
		else
			PlayerBlocks.push_back(PlayerBlock{
				GameObject(PlayerBlocks[iter - 1].body.m_position , block_size),
				PlayerBlocks[iter - 1].velocity,
				this->TextureAtlas[PLAYER_BODY][PlayerBlocks[iter - 1].velocity]
			});
		PlayerBlocks[iter].body.m_position += -PlayerBlocks[iter].velocity;
	}
}

void PlayerObject::DrawPlayer(CubeRenderer* renderer, Texture& texture) {
	for (PlayerBlock& block : this->PlayerBlocks)
		block.body.DrawObject(renderer, texture, block.TexCoords);
}

void PlayerObject::UpdatePlayer() {
	this->AssignVelocities();
	this->PlayerBlocks[0].velocity = this->NextVelocity;
	this->DecideTextureAtlas();
	this->Move();
}

void PlayerObject::DecideTextureAtlas() {
	for (int iter{}; iter < this->PlayerBlocks.size(); ++iter) {
		if (iter == 0) //Head
			PlayerBlocks[iter].TexCoords = this->TextureAtlas[PLAYER_HEAD][PlayerBlocks[iter].velocity];
		else if (iter == this->PlayerBlocks.size() - 1) // Tail
			PlayerBlocks[iter].TexCoords = this->TextureAtlas[PLAYER_TAIL][PlayerBlocks[iter - 1].velocity];
		else { //Body
			if (PlayerBlocks[iter].velocity == VELOCITY_LEFT && this->TextureAtlas[PLAYER_CURVED_LEFT].contains(PlayerBlocks[iter - 1].velocity))
				PlayerBlocks[iter].TexCoords = this->TextureAtlas[PLAYER_CURVED_LEFT][PlayerBlocks[iter - 1].velocity];
			else if (PlayerBlocks[iter].velocity == VELOCITY_RIGHT && this->TextureAtlas[PLAYER_CURVED_RIGHT].contains(PlayerBlocks[iter - 1].velocity))
				PlayerBlocks[iter].TexCoords = this->TextureAtlas[PLAYER_CURVED_RIGHT][PlayerBlocks[iter - 1].velocity];
			else if (PlayerBlocks[iter].velocity == VELOCITY_UP && this->TextureAtlas[PLAYER_CURVED_UP].contains(PlayerBlocks[iter - 1].velocity))
				PlayerBlocks[iter].TexCoords = this->TextureAtlas[PLAYER_CURVED_UP][PlayerBlocks[iter - 1].velocity];
			else if (PlayerBlocks[iter].velocity == VELOCITY_DOWN && this->TextureAtlas[PLAYER_CURVED_DOWN].contains(PlayerBlocks[iter - 1].velocity))
				PlayerBlocks[iter].TexCoords = this->TextureAtlas[PLAYER_CURVED_DOWN][PlayerBlocks[iter - 1].velocity];
			else
				PlayerBlocks[iter].TexCoords = this->TextureAtlas[PLAYER_BODY][PlayerBlocks[iter].velocity];
		}	
	}
}

void PlayerObject::AssignVelocities() {
	for (std::vector<PlayerBlock>::iterator iter{ this->PlayerBlocks.end() - 1 }; iter != this->PlayerBlocks.begin(); --iter)
		iter->velocity = (iter - 1)->velocity;
}

void PlayerObject::Move() {
	for (std::vector<PlayerBlock>::iterator iter{ this->PlayerBlocks.begin() }; iter != this->PlayerBlocks.end(); ++iter)
		iter->body.m_position += iter->velocity;
}

void PlayerObject::SetNextVelocity(const glm::vec2 NextVelocity) {
	if (NextVelocity.x != -this->NextVelocity.x && NextVelocity.y != -this->NextVelocity.y)
		this->NextVelocity = NextVelocity;
}

void PlayerObject::AddNewBlock() {
	PlayerBlock last{ this->PlayerBlocks.back() };
	this->PlayerBlocks.push_back(PlayerBlock{
		GameObject(last.body.m_position, last.body.m_size),
		last.velocity,
		this->TextureAtlas[PLAYER_TAIL][last.velocity]
	});
	this->PlayerBlocks.back().body.m_position += -this->PlayerBlocks.back().velocity;
	DecideTextureAtlas();
}

int PlayerObject::GetLength() const {
	return (int)this->PlayerBlocks.size();
}

const std::vector<PlayerObject::PlayerBlock>& PlayerObject::GetPlayerBlocks() const {
	return PlayerBlocks;
}

PlayerObject::PlayerBlock& PlayerObject::GetPlayerHead() {
	return this->PlayerBlocks[0];
}