#pragma once

#include "Player.h"
#include "Setup.h"
#include "console_utils.h"
#include <utility>
#include <vector>

enum class Controlls {
	MENU, NAME, FIELDSIZE, SHIPCOUNT, PLACING, SHOOTING
};

// prints a visual box around the console
void titleBox();
// displayes the current controlls on the bottom left
void currentControlls(Controlls UI);
// displays the current menu
void displayMenu(const std::vector<std::pair<char, std::string>>& options);
// prints the field of a player
void printField(std::vector<std::vector<char>> field, int fieldWidth);
// render function during the placing of ships
void renderPlace(Player& player, std::pair<int, int> cursor, int length, Rotation rotation);
// render function for the shooting aspect 
void renderShoot(Player& target, Player& self, std::pair<int, int> cursor);
// render function for when there are multiple player (not implemented)
void renderMulti(std::vector<Player>& playerVect);
// prints a visual box around the field
void fieldBox(int x, int y, Player* p);