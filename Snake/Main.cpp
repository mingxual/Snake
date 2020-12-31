#include "Game.h"

int main(int argc, char** argv)
{
	Game snake;
	bool isInit = snake.Initialize();
	if (!isInit) return 1;
	snake.RunLoop();
	snake.Shutdown();

	return 0;
}
