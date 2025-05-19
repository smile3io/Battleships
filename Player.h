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

    // checks if the ship placement is valid
    bool isValidPlacement(int length, Rotation rotation, std::pair<int, int> cord);
    // the general placing function were it goes through every ship 
    void placing();
    // places a ship into the ship "field" and creates a Ship object and inserts it to allShips
    void placeShip(int lenght, Rotation rotation, std::pair<int, int> cord);
    // generates all fields for a player
    void genField();
    // Registers the player and saves the name
    void askName();
    // prints the field of the current player
    void displayField();
    // shoots at a position and gives back if it was a hit
    bool shoot(Player& player, std::pair<int, int> position);
    // same as shoot but sithout the console output
    bool aiShoot(Player& target, std::pair<int, int> position);
    // a way to generate a score at the end of the game
    void scoring();
};