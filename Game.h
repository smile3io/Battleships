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

	// starts the game
	void run();
	// initalizes game with current settings
	void initGame();
	// placing ships off all players
	void placingShips();
	// gameloop conists off shooting between player
	void gameLoop();
	// maybe later score display etc
	void gameOver();
	// menu navigation and code calling
	void handleMenu();
	// check if all ships of a player has sunken
	bool allShipsSunken(const Player& player);
	// generates a valid position for the ai to shoot
	std::pair<int, int> getAIPosition(const Player& ai);
	// goes one menu back
	void menuGoBack();
	// sets current gamestate (not used)
	void setState(GameState newState);
};