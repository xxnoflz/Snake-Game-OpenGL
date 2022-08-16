#include "Snake Game.h"

Game::SnakeGame::SnakeGame(uint32_t window_width, uint32_t window_height) : m_window(), m_width(window_width), m_height(window_height), 
player(), food(), state(GAME_MENU), renderer(), textRender(), grid(), sound(), time() { }

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Game::SnakeGame* game{ static_cast<Game::SnakeGame*>(glfwGetWindowUserPointer(window)) };
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		game->InputHandler(key);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		game->InputHandler(key);

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		game->InputHandler(key);
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		game->InputHandler(key);
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		game->InputHandler(key);
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		game->InputHandler(key);
}

void Game::SnakeGame::InitOpenGL() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(this->m_width, this->m_height, "Snake Game OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		glfwTerminate();

	glViewport(0, 0, this->m_width, this->m_height);
	glfwSetWindowUserPointer(this->m_window, this);
	glfwSetFramebufferSizeCallback(this->m_window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});
	glfwSetKeyCallback(this->m_window, key_callback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::SnakeGame::Init() {
	this->InitOpenGL();

	glm::mat4 projection{ glm::ortho(0.0f, (float)this->m_width, (float)this->m_height, 0.0f, -1.0f, 1.0f) };
	ResourceManager::LoadShaderFile("source/Snake Game/Shaders/cubeShader.vert", "source/Snake Game/Shaders/cubeShader.frag", "cubeShader");
	ResourceManager::LoadShaderFile("source/Snake Game/Shaders/textShader.vert", "source/Snake Game/Shaders/textShader.frag", "textShader");
	ResourceManager::LoadShaderFile("source/Snake Game/Shaders/gridShader.vert", "source/Snake Game/Shaders/gridShader.frag", "gridShader");
	ResourceManager::GetShader("cubeShader").use();
	ResourceManager::GetShader("cubeShader").SetMat4("projection", projection);
	ResourceManager::GetShader("textShader").use();
	ResourceManager::GetShader("textShader").SetMat4("projection", projection);
	ResourceManager::GetShader("gridShader").use();
	ResourceManager::GetShader("gridShader").SetMat4("projection", projection);

	ResourceManager::LoadTextureFile("source/Snake Game/Textures/Snake.png", "snake");

	time.timeStep = 5 / 60.0f; // 12fps

	renderer = new CubeRenderer();
	textRender = new TextRenderer("source/Snake Game/Fonts/arial.ttf");
	grid = new GridRenderer();
	sound = irrklang::createIrrKlangDevice();

	player = new PlayerObject(glm::vec2(this->m_width / 2.0f, this->m_height / 2.0f), glm::vec2(BLOCK_SIZE), VELOCITY_LEFT);
	food = new FoodObject(glm::vec2(BLOCK_SIZE), this->m_width, this->m_height);
}

void Game::SnakeGame::UpdateTime() {
	time.currentTime = (float)glfwGetTime();
	time.deltaTime = time.currentTime - time.lastTime;
	time.lastTime = time.currentTime;

	time.accumulator += time.deltaTime;
}

void Game::SnakeGame::Render() {
	if (state == GAME_RUN || state == GAME_MENU || state == GAME_END) {
		grid->Draw(this->m_width, this->m_height, ResourceManager::GetTexture("snake"), BLOCK_SIZE, ResourceManager::GetShader("gridShader"));
		player->DrawPlayer(renderer, ResourceManager::GetTexture("snake"));
		food->DrawObject(renderer, ResourceManager::GetTexture("snake"), glm::vec2(2, 3));
	}
	if (state == GAME_MENU) {
		textRender->Draw("Welcome to Snake Game", glm::vec2(0.0, this->m_height / 2.0f - 50.0f), 1, ResourceManager::GetShader("textShader"));
		textRender->Draw("Press SPACE to start", glm::vec2(0.0, this->m_height / 2.0f + 50.0f), 1, ResourceManager::GetShader("textShader"));
	}
	if (state == GAME_END) {
		textRender->Draw("GAME OVER", glm::vec2(0.0, this->m_height / 2.0f - 50.0f), 1, ResourceManager::GetShader("textShader"));
		textRender->Draw("Your high score is - " + std::to_string(this->player->GetLength()),
			glm::vec2(0.0, this->m_height / 2.0f + 50.0f), 1, ResourceManager::GetShader("textShader"));
		textRender->Draw("Press SPACE to restart",
			glm::vec2(0.0, this->m_height / 2.0f + 100.0f), 1, ResourceManager::GetShader("textShader"));
	}
}

void Game::SnakeGame::Update() {
	if (this->state == GAME_RUN) {
		player->UpdatePlayer();
		this->DoCollisions();
	}
}

void Game::SnakeGame::InputHandler(int key) {
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(this->m_window, true);

	if (state == GAME_MENU && key == GLFW_KEY_SPACE)
			state = GAME_RUN;

	if (state == GAME_RUN) {
		if (key == GLFW_KEY_UP)
			player->SetNextVelocity(VELOCITY_UP);
		if (key == GLFW_KEY_DOWN)
			player->SetNextVelocity(VELOCITY_DOWN);
		if (key == GLFW_KEY_LEFT)
			player->SetNextVelocity(VELOCITY_LEFT);
		if (key == GLFW_KEY_RIGHT)
			player->SetNextVelocity(VELOCITY_RIGHT);
	}

	if(state == GAME_END && key == GLFW_KEY_SPACE) {
		player = new PlayerObject(glm::vec2(this->m_width / 2.0f, this->m_height / 2.0f), glm::vec2(BLOCK_SIZE), VELOCITY_LEFT);
		food = new FoodObject(glm::vec2(BLOCK_SIZE), this->m_width, this->m_height);
		state = GAME_MENU;
	}
}

bool Game::SnakeGame::CheckCollision(GameObject* object_one, GameObject* object_two) {
	bool collisionX{object_one->m_position.x + object_one->m_size.x > object_two->m_position.x &&
					object_two->m_position.x + object_two->m_size.x > object_one->m_position.x};
	bool collisionY{ object_one->m_position.y + object_one->m_size.y > object_two->m_position.y &&
					object_two->m_position.y + object_two->m_size.y > object_one->m_position.y };
	return collisionX && collisionY;
}

void Game::SnakeGame::DoCollisions() {
	//Wall collision check
	if (player->GetPlayerHead().body.m_position.x < 0 || player->GetPlayerHead().body.m_position.x + player->GetPlayerHead().body.m_size.x > this->m_width ||
		player->GetPlayerHead().body.m_position.y < 0 || player->GetPlayerHead().body.m_position.y + player->GetPlayerHead().body.m_size.y > this->m_height) {
			state = GAME_END;
			sound->play2D("source/Snake Game/Sounds/gameover.mp3");
	}
	//Fruit collision check
	if (CheckCollision(&player->GetPlayerHead().body, food)) {
		player->AddNewBlock();
		food->CreateNewFood(this->m_width, this->m_height);
		sound->play2D("source/Snake Game/Sounds/food.mp3");
	}
	//Snake collision with itself
	std::vector<PlayerObject::PlayerBlock> PlayerBlock{ this->player->GetPlayerBlocks() };
	std::for_each(PlayerBlock.begin() + 1, PlayerBlock.end(), [=](PlayerObject::PlayerBlock& block) {
		if (CheckCollision(&player->GetPlayerHead().body, &block.body)) {
			state = GAME_END;
			sound->play2D("source/Snake Game/Sounds/gameover.mp3");
		}
	});
}

void Game::SnakeGame::run() {
	while(!glfwWindowShouldClose(this->m_window)) {
		UpdateTime();
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		
		while (time.accumulator >= time.timeStep) {
			Update();
			time.accumulator -= time.timeStep;
		}
		Render();

		glfwSwapBuffers(this->m_window);
	}
	glfwTerminate();
}