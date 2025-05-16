#pragma once

#include "Player.h"
#include "Setup.h"
#include "console_utils.h"
#include <utility>
#include <vector>

enum class Controlls {
	MENU, NAME, FIELDSIZE, SHIPCOUNT, PLACING, SHOOTING
};

void titleBox();

void currentControlls(Controlls UI);

void displayMenu(const std::vector<std::pair<char, std::string>>& options);

void printField(std::vector<std::vector<char>> field, int fieldWidth);

void renderPlace(Player& player, std::pair<int, int> cursor, int length, Rotation rotation);

void renderShoot(Player& target, Player& self, std::pair<int, int> cursor);

void renderMulti(std::vector<Player>& playerVect);

void fieldBox(int x, int y, Player* p);

void goBack();