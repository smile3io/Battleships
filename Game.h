#pragma once

#include "Player.h"
#include "Setup.h"
#include "UI.h"
#include <stack>
#include <vector>
#include <map>
#include <functional>
#include <conio.h>

enum class GameState {
	MENU, PLACE, GAME_LOOP, GAME_OVER
};

enum class MenuID {
	MAIN, PLAY, PRESETS, OPTIONS, SHIPS, MODE, AI
};

class Game
{
public:
	GameState state;
	std::stack<MenuID> menuStack;
	std::vector<Player> playerVect;
	std::map<MenuID, std::vector<std::pair<char, std::string>>> menuData;
	std::map<MenuID, std::map<char, std::function<void()>>> menuActions;

	Game();


	void run();

	void initGame();

	void placingShips();

	void gameLoop();

	void gameOver();

	void handleMenu();

	bool allShipsSunken(const Player& player);

	std::pair<int, int> getAIPosition(const Player& ai);

	void menuGoBack();

	void setState(GameState newState);
};

