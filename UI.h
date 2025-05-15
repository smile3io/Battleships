#pragma once

enum class Controlls {
	MENU, NAME, FIELDSIZE, SHIPCOUNT, PLACING, SHOOTING
};

class UI
{
public:
	static void titleBox();
	static void currentControlls(Controlls UI);
	static void displayMenu(const std::vector<std::pair<char, std::string>>& options);
	static void render();
	static void fieldBox(int x, int y, Player* p);

};