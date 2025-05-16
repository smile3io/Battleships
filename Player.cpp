#include "Player.h"
#include "Setup.h"
#include "UI.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>


extern Setup gameSetup;
extern int consoleWidth, consoleHeight;
extern int sleepTime;

Player::Player(int number) : number(number), score(0), shotsFired(0){
    genField();
}

void Player::genField() {
    field.assign(gameSetup.fieldSize, std::vector<char>(gameSetup.fieldSize, '~'));
    ships.assign(gameSetup.fieldSize, std::vector<char>(gameSetup.fieldSize, ' '));
    shots.assign(gameSetup.fieldSize, std::vector<char>(gameSetup.fieldSize, ' '));
    opponentShots.assign(gameSetup.fieldSize, std::vector<char>(gameSetup.fieldSize, ' '));
}

void Player::askName() {
    titleBox();
    currentControlls(Controlls::NAME);
    showCursor();
    cursorPosition(consoleHeight * 0.45, consoleWidth * 0.25);
    std::cout << "Player " << number << " what's your name?\n";
    cursorPosition(consoleHeight * 0.45 + 2, consoleWidth * 0.25);
    std::cin >> name;
    cursorPosition(consoleHeight * 0.45 + 4, consoleWidth * 0.25);
    std::cout << "Hello " << name << " welcome to Battleships!\n";
    hideCursor();
    Sleep(sleepTime);
}

void Player::displayField() {
    //render(*this, Settings::ALL, PLACE);
}

bool Player::isValidPlacement(int length, Rotation rotation, std::pair<int, int> cord) {
    if (rotation == Rotation::HOR) {
        if (cord.first + length > gameSetup.fieldSize) return false;
        for (int j = 0; j < length; j++) {
            if (ships[cord.second][cord.first + j] != ' ') return false;
        }
        if (gameSetup.distance == Settings::AWAY) {
            for (int j = cord.first - 1; j <= cord.first + length; j++) {
                for (int i = cord.second - 1; i <= cord.second + 1; i++) {
                    if (i >= 0 && i < gameSetup.fieldSize && j >= 0 && j < gameSetup.fieldSize) {
                        if (ships[i][j] == '*') return false;
                    }
                }
            }
        }
    }
    else {
        if (cord.second + length > gameSetup.fieldSize) return false;
        for (int j = 0; j < length; j++) {
            if (ships[cord.second + j][cord.first] != ' ') return false;
        }
        if (gameSetup.distance == Settings::AWAY) {
            for (int i = cord.second - 1; i <= cord.second + length; i++) {
                for (int j = cord.first - 1; j <= cord.first + 1; j++) {
                    if (i >= 0 && i < gameSetup.fieldSize && j >= 0 && j < gameSetup.fieldSize) {
                        if (ships[i][j] == '*') return false;
                    }
                }
            }
        }
    }
    return true;
}

void Player::placing() {
    std::pair<int, int> cursor = {1, 1};
    for (int length : gameSetup.lenghts) {
        for (int count = gameSetup.lenghtsCount[length - 2]; count > 0; count--) {
            bool placed = false;
            Rotation direction = Rotation::HOR;
            while (!placed) {
                renderPlace(*this, cursor, length, direction);
                int col = cursor.first; // Row index (y)
                int row = cursor.second; // Column index (x)
                int input = _getch();
                if (input == 224) input = _getch(); // Handle arrow key prefix
                if (input == static_cast<int>(Input::UP) && col > 0) {
                    cursor.first--;
                }
                else if (input == static_cast<int>(Input::DOWN)) {
                    if (direction == Rotation::VER && col + length <= gameSetup.fieldSize - 1) {
                        cursor.first++;
                    }
                    else if (direction == Rotation::HOR && col < gameSetup.fieldSize - 1) {
                        cursor.first++;
                    }
                }
                else if (input == static_cast<int>(Input::LEFT) && row > 0) {
                    cursor.second--;
                }
                else if (input == static_cast<int>(Input::RIGHT)) {
                    if (direction == Rotation::HOR && row + length <= gameSetup.fieldSize - 1) {
                        cursor.second++;
                    }
                    else if (direction == Rotation::VER && row < gameSetup.fieldSize - 1) {
                        cursor.second++;
                    }
                }
                else if (input == static_cast<int>(Input::R)) {
                    Rotation newDirection = (direction == Rotation::HOR) ? Rotation::VER : Rotation::HOR;
                    if ((newDirection == Rotation::HOR && row + length <= gameSetup.fieldSize) || (newDirection == Rotation::VER && col + length <= gameSetup.fieldSize)) direction = newDirection;
                }
                else if (input == static_cast<int>(Input::ENTER)) {
                    cursorPosition(consoleHeight * 0.3, consoleWidth * 0.05);
                    if (isValidPlacement(length, direction, cursor)) {
                        placeShip(length, direction, cursor);
                        std::cout << "Placed a " << length << " long ship\n";
                        cursorHorizontalAbsolute(consoleWidth * 0.05);
                        std::cout << "(" << (direction == Rotation::HOR ? "horizontally" : "vertically") << ")\n";
                        Sleep(sleepTime);
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

void Player::placeShip(int length, Rotation rotation, std::pair<int, int> cord) {
    if (rotation == Rotation::HOR) {
        for (int i = 0; i < length; i++) {
            ships[cord.first][cord.second + i] = '*';
        }
        allShips.emplace_back(length, Rotation::HOR, std::make_pair(cord.first, cord.second));
    }else if (rotation == Rotation::VER) {
        for (int i = 0; i < length; i++) {
            ships[cord.first + i][cord.second] = '*';
        }
        allShips.emplace_back(length, Rotation::VER, std::make_pair(cord.first, cord.second));
    }
}

bool Player::shoot(Player& target, std::pair<int, int> position) {
    int row = position.first; // y-coordinate
    int col = position.second; // x-coordinate
    if (shots[row][col] == 'X' || shots[row][col] == 'O') {
        std::cout << "Invalid shot! Try again.\n";
        Sleep(sleepTime);
        return false;
    }
    shotsFired++;
    for (Ships& ship : target.allShips) {
        if (ship.isHit(position)) {
            shots[row][col] = 'X';
            target.opponentShots[row][col] = 'X';
            std::cout << "HIT\n";
            if (ship.isSunken()) {
                std::cout << "Ship sunken!\n";
                score += ship.size * 10;
            }
            return true;
        }
    }
    shots[row][col] = 'O';
    target.opponentShots[row][col] = 'O';
    std::cout << "MISS\n";
    return false;
}

void Player::scoring() {
    int hits = 0;
    int sunkenShips = 0;
    for (Ships& ship : allShips) {
        if (ship.isSunken()) sunkenShips++;
        for (const auto& segment : ship.segments) {
            if (segment.first) hits++;
        }
    }
    score = hits * 10 + sunkenShips * 50 - shotsFired * 2;
}
