#pragma once

#include "Ships.h"
#include "Setup.h"
#include <vector>
#include <string>
#include <utility>

enum class Input {
    ENTER = 13,
    R = 114,
    UP = 72,
    DOWN = 80,
    LEFT = 75,
    RIGHT = 77
};

class Player {
public:
    std::vector<Ships> allShips;    // list of all ships of a player 

    int number;         // player number
    std::string name;   // player name
    std::vector<std::vector<char>> field; // own player field (z.B. '.', 'O', 'X', '#')
    std::vector<std::vector<char>> ships; // ship position for display
    std::vector<std::vector<char>> shots; // shot position
    std::vector<std::vector<char>> opponentShots;

    int shotsFired;
    int score;      // session score calc -> ships sunken points

    // create the player with a number
    Player(int number);

    //
    bool isValidPlacement(int length, Rotation rotation, std::pair<int, int> cord);
    //
    void placing();
    //
    void placeShip(int lenght, Rotation rotation, std::pair<int, int> cord);
    // generates the field for a player
    void genField();
    // Registers the player and saves the name
    void askName();
    // prints the field of the current player
    void displayField();
    //
    bool shoot(Player& player, std::pair<int, int> position);
    //
    void scoring();
};