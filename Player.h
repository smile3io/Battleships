#pragma once

#include "Ships.h"
#include "Setup.h"
#include <vector>
#include <string>
#include <utility>

enum class CellState {
    WATER, SHIP, HIT, MISS // determines color
};

class Player {
public:
    class Ships;
    std::vector<Ships> allShips;    // list of all ships of a player 

    int number;         // player number
    std::string name;   // player name
    std::vector<std::vector<char>> field; // own player field (z.B. '.', 'O', 'X', '#')
    std::vector<std::vector<char>> ships; // ship position for display
    std::vector<std::vector<char>> shots; // shot position

    int shotsFired;
    int score;      // session score calc -> win, ships sunken points

    // create the player with a number
    Player(int n);

    // generates the field for a player
    void genField();
    // Registers the player and saves the name
    void askName();
    // prints the field of the current player
    void displayField();
    //
    void placeShip(int l, Rotation r, std::pair<int, int> c);
    //
    void shoot(std::pair<int, int> c);
    //
    void scoring();
};