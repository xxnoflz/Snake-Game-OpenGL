#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <irrklang/irrKlang.h>

#include "Utilities/ResourceManager.h"
#include "Render/CubeRenderer.h"
#include "Render/TextRenderer.h"
#include "Render/GridRenderer.h"
#include "Objects/PlayerObject.h"
#include "Objects/FoodObject.h"

#include <algorithm>
#include <string>

namespace Game {

	class SnakeGame {
		struct TimeLogic {
			float deltaTime;
			float currentTime;
			float lastTime;

			float timeStep;

			float accumulator;
		};

		enum GameState {
			GAME_MENU,
			GAME_RUN,
			GAME_END
		};
		const float BLOCK_SIZE{ 20.0f };

		const glm::vec2 VELOCITY_UP{ 0.0, -BLOCK_SIZE};
		const glm::vec2 VELOCITY_DOWN{ 0.0, BLOCK_SIZE};
		const glm::vec2 VELOCITY_LEFT{ -BLOCK_SIZE, 0.0 };
		const glm::vec2 VELOCITY_RIGHT{ BLOCK_SIZE, 0.0 };

	public:
		SnakeGame(uint32_t window_width, uint32_t window_height);

		void InitOpenGL();
		void Init();

		void Render();
		void Update();
		void InputHandler(int key);

		bool CheckCollision(GameObject* object_one, GameObject* object_two);
		void DoCollisions();

		void UpdateTime();

		void run();
	private:
		GLFWwindow* m_window;
		uint32_t m_width;
		uint32_t m_height;
		GameState state;
		TimeLogic time;

		PlayerObject* player;
		FoodObject* food;

		CubeRenderer* renderer;
		TextRenderer* textRender;
		GridRenderer* grid;
		irrklang::ISoundEngine* sound;
	};
}