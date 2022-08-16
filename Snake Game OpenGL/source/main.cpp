#include "Snake Game/Snake Game.h"

int main() {
	Game::SnakeGame game{ 800, 600 };
	game.Init();
	game.run();
	return 0;
}