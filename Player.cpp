#include "Player.h"
#include "UI.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>

Player::Player(int number) : number(number), score(0), shotsFired(0){
    genField();
}

void Player::genField() {
    field.assign(gameSetup.FieldSize, std::vector<char>(gameSetup.Fieldsize, '~'));
    ships.assign(gameSetup.FieldSize, std::vector<char>(gameSetup.Fieldsize, ' '));
    shots.assign(gameSetup.FieldSize, std::vector<char>(gameSetup.Fieldsize, ' '));
}
void Player::askName() {
    UI::titleBox();
    UI::currentControlls(Controlls::NAME);
    console_utils::showCursor();
}
void Player::displayField() {
    
}
void Player::placeShip(int l, Rotation r, std::pair<int, int> c) {

}
void Player::shoot(std::pair<int, int> c) {

}
void Player::scoring() {

}

void Player::askName() {
    titleBox();
    currentControlls(NAME);
    showCursor();
    cursorPosition(consoleHeight * 0.45, consoleWidth * 0.25);
    std::cout << "Player " << number << " whats your name?\n";
    cursorPosition(consoleHeight * 0.45 + 2, consoleWidth * 0.25);
    std::cin >> name;
    cursorPosition(consoleHeight * 0.45 + 4, consoleWidth * 0.25);
    std::cout << "Hello " << name << " welcome to Battleships!\n";
    hideCursor();
    Sleep(sleepTime);
}

void placeShips() {
    std::cout << name << ", your turn\n";
    // cursor start position in center of field
    std::pair<bool, std::pair<int, int>> cursor = { true, {1, 1} };
    for (int lenght : gameSetup.lenghts) {
        for (int count = gameSetup.lenghtsCount[lenght - 2]; count > 0; count--) {
            bool placed = false;
            int direction = HOR;  // 0 = horizontal  1 = vertical
            while (!placed) {
                //render(*this, cursor, lenght, direction);
                int col = cursor.second.first;  // vertical
                int row = cursor.second.second; // horizontal
                int size = gameSetup.fieldSize;
                //cout << "lenght: " << lenght << endl << "size: " << size << endl << "row: " << row << "  col: " << col << endl;
                int input = _getch();
                if (input == static_cast<int> (Settings::UP) && col > 0) {
                    cursor.second.first--;
                }
                else if (input == static_cast<int> (Settings::DOWN) && ((direction == VER) ? col + lenght < size : col < size - 1)) {
                    cursor.second.first++;
                }
                else if (input == static_cast<int> (Settings::LEFT) && row > 0) {
                    cursor.second.second--;
                }
                else if (input == static_cast<int> (Settings::RIGHT) && ((direction == HOR) ? row + lenght < size : row < size - 1)) {
                    cursor.second.second++;
                }
                else if (input == static_cast<int> (Settings::ROTATE) && row + lenght <= size && col + lenght <= size) {
                    direction = (direction == HOR) ? VER : HOR;
                }
                else if (input == 13/*ENTER*/) {   // placing ships
                    bool valid = true;
                    if (direction == HOR) {   // horizontal
                        for (int j = 0; j < lenght; j++) {
                            if (ships[col][row + j] == '*') {
                                valid = false;  // already a ship
                                break;
                            }
                        }
                    }
                    else if (direction == VER) {  // vertical
                        for (int j = 0; j < lenght; j++) {
                            if (ships[col + j][row] == '*') {
                                valid = false;  // already a ship
                                break;
                            }
                        }
                    }
                    if (valid && gameSetup.distance == Settings::AWAY) {

                    }
                    if (valid) {

                        if (direction == HOR) {   // 
                            for (int j = 0; j < lenght; j++) {
                                ships[col][row + j] = '*';
                            }
                        }
                        else if (direction == VER) {
                            for (int j = 0; j < lenght; j++) {
                                ships[col + j][row] = '*';
                            }
                        }
                        std::cout << "Placed a " << lenght << " long ship " << (direction == HOR ? "horizontally" : "vertically") << "\n";
                        placed = true;
                    }
                    else {
                        std::cout << "Invalid placement! Try again.\n";
                        Sleep(sleepTime);
                    }
                }

            }
        }
    }
}

// score considers win and sunken ships and hits (and total shots)
void /*update?*/scoring() {} // Diese Funktion sollte aufgerufen werden, wenn ein Spieler ein Schiff versenkt oder das Spiel gewinnt.(Verhältnis von Treffern zu Schüssen)
