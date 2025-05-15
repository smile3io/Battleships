#pragma once

#include "Player.h"
#include "Setup.h"
#include "Menu.h"
#include <stack>

enum class GameState {
	MENU, PLACE, GAME_LOOP, GAME_OVER
};

enum class MenuID {
	MAIN, PLAY, PRESETS, OPTIONS, SHIPS, MODE, AI
};

class Game
{
public:
	Setup gameSetup;
	GameState state;
	std::stack<MenuID> menuStack;
	std::vector<Player> playerVect;
	std::map<MenuID, std::vector<std::pair<char, std::string>>> menuData;
	std::map<MenuID, std::map<char, std::function<void()>>> menuActions;
	int fieldDisplayWidth;
	int fieldDisplayHeight;

	Game();

	void run();
	void initGame();
	void placingShips();
	void gameLoop();
	void handleMenu();
};

