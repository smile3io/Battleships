#include "Setup.h"
#include "UI.h"
#include <algorithm>

Setup::Setup() :
    playerCount(1),
    mode(Settings::STANDARD),
    distance(Settings::CLOSE),
    shipSetting(Settings::FIVE),
    fieldSize(10),
    shipCount(0),
    aiDifficulty(Settings::NORMAL) {}


// diff between presets 
void Setup::setPreset(Settings difficulty) {
    switch (difficulty) {
    case Settings::SMALL:
        mode = Settings::STANDARD;
        fieldSize = 5;
        shipCount = 2;
        aiDifficulty = Settings::EASY;
        break;
    case Settings::MEDIUM:
        mode = Settings::STANDARD;
        fieldSize = 10;
        shipCount = 5;
        aiDifficulty = Settings::NORMAL;
        break;
    case Settings::LARGE:
        mode = Settings::STANDARD;
        fieldSize = 15;
        shipCount = 12;
        aiDifficulty = Settings::HARD;
        break;
    default:
        break;
    }
}
// generates the axis at the start of game
void Setup::genAxis() {
    xAxisLabel.clear();
    yAxisLabel.clear();
    // generating the alphabet on the x-axis
    for (int i = 0; i <= fieldSize; i++) {
        int columns = i;
        std::string label = "";
        while (columns > 0) {
            columns--; // Adjust to 0-25 range.
            label = (char)('A' + (columns % 26)) + label;
            columns /= 26;
        }
        xAxisLabel.push_back(label);
        // filling in the y-axis with numbers
        yAxisLabel.push_back(i);
    }
}
// generates num and lenght of ships
void Setup::genShips() {
    shipsLenght.clear();
    if (!shipCount) {
        if (shipSetting == Settings::FIVE) {
            int shipFields = fieldSize * fieldSize * (static_cast<int>(shipSetting) / 100.);
            while (shipFields >= 2) {
                for (int num : lengthShipsFIVE) {
                    if (num > shipFields) break;
                    shipFields -= num;
                    shipsLenght.push_back(num);
                }
            }
        }
        else if (shipSetting == Settings::TEN) {
            int shipFields = fieldSize * fieldSize * (static_cast<int>(shipSetting) / 100.);
            while (shipFields >= 2) {
                for (int num : lengthShipsTEN) {
                    if (num > shipFields) break;
                    shipFields -= num;
                    shipsLenght.push_back(num);
                }
            }
        }
    }
    else {
        int temp = shipCount;
        while (temp) {
            for (int num : lengthShipsFIVE) {
                if (!temp) break;
                temp--;
                shipsLenght.push_back(num);
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        lenghtsCount[i] = count(shipsLenght.begin(), shipsLenght.end(), i + 2);
    }
}
// displays the current rules
const void Setup::displayRules() {
    titleBox();
    std::cout << "Mode: ";
    switch (mode) {
    case Settings::STANDARD: std::cout << " Classic\n"; break;
    case Settings::SALVO: std::cout << " Five shots\n"; break;
    case Settings::RACE: std::cout << " Race to the end\n"; break;
    }
    std::cout << "Distance: ";
    switch (distance) {
    case Settings::AWAY: std::cout << "One apart\n"; break;
    case Settings::CLOSE: std::cout << "Next to another\n"; break;
    }
    int num;
    (shipCount) ? num = shipCount : num = shipsLenght.size();
    std::cout << "Num of ships: " << num << std::endl;
    std::cout << "Everone has:\n";
    for (int i = 2; i <= 5; i++) {
        int amount = count(shipsLenght.begin(), shipsLenght.end(), i);
        std::cout << amount << "x" << i << "er ships" << std::endl;
    }
    if (shipCount == 0) {
        std::cout << "Ship amount: ";
        switch (shipSetting) {
        case Settings::FIVE: std::cout << "low\n"; break;
        case Settings::TEN: std::cout << "high\n"; break;
        }
    }
    std::cout << "Fieldsize: " << fieldSize << "x" << fieldSize << std::endl;
    std::cout << "AI diff: ";
    switch (aiDifficulty) {
    case Settings::EASY: std::cout << "easy\n"; break;
    case Settings::NORMAL: std::cout << "normal\n"; break;
    case Settings::HARD: std::cout << "hard\n"; break;
    }

    /*
    UI::titleBox();
    std::cout << "Battleships Rules:\n";
    std::cout << "- Place ships on your " << fieldSize << "x" << fieldSize << " grid.\n";
    std::cout << "- Shoot at opponent's grid to hit ships.\n";
    std::cout << "- Mode: " << (mode == Settings::STANDARD ? "Standard" : mode == Settings::SALVO ? "Salvo" : "Race") << "\n";
    std::cout << "- Distance: " << (distance == Settings::AWAY ? "Away (1-cell gap)" : "Close") << "\n";
    */
}
