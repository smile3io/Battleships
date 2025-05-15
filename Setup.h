#pragma once

#include <vector>
#include <string>

enum class Settings {
    SMALL, MEDIUM, LARGE,   // size
    EASY, NORMAL, HARD,     // diff
    STANDARD, SALVO, RACE, ABILITIES,  // mode
    ONE, CLOSE,             // dist
    FIVE = 17, TEN = 30,    // ship
};

class Setup
{
private:
    const int lengthShipsFIVE[5] = { 2,3,3,4,5 };
    const int lengthShipsTEN[10] = { 2,2,2,2,3,3,3,4,4,5, };
public:
    const int lenghts[4] = { 2,3,4,5 };
    int lenghtsCount[4];
    int playerCount;        //  easy    normal      hard                custom
    Settings mode;          //  normal / salvo / race?                  
    Settings distance;      //  normal ohne abstand / mit abstand
    Settings shipSetting;   //  5 schiffe auf 10 (17 aka 0.17) / 10 auf 10 (30 aka 0.3)
    int fieldSize;          //  5       10          15                  1** 2*** 1**** 1***** 17Felder / 4** 3*** 2**** 1***** 30Felder
    int shipCount;          //  2       5           12                  0,17 / 0,30
    std::vector<int> shipsLenght;//  2*2     2,3*2,4,5   2*4,3*4,4*2,5*2
    Settings aiDifficulty;  //  easy    normal      hard
    std::vector<std::string> xAxisLabel;
    std::vector<int> yAxisLabel;

    // standard constructor
    Setup();
    // 
    void setPreset(Settings difficulty);
    // 
    void genAxis();
    // 
    void genAxis();
    // 
    const void displayRules();
};