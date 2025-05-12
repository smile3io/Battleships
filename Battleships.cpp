#include "console_utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <conio.h> // for _getch
#include <map>
#include <stack>
#include <functional>
#include <limits> // for numeric_limits cin
#include <utility> // for std::pair

using namespace console_utils;


void titleBox();

//---------------------------------------ENUM--------------------------------------------------

enum class GameState {
    MENU, PLACE, GAME_LOOP, GAME_OVER
};

enum class MenuID {
    MAIN, PLAY, PRESETS, OPTIONS, SHIPS, MODE, AI
};

enum class Settings {
    SMALL, MEDIUM, LARGE,   // size
    EASY, NORMAL, HARD,     // diff
    STANDARD, SALVO, RACE,  // mode
    AWAY, CLOSE,             // dist
    FIVE = 17, TEN = 30,    // ship
    UP = 72, DOWN = 80,     // arrow
    LEFT = 75, RIGHT = 77,  // arrow
    ROTATE = 114, ENTER = 13,// rotate R  ENTER 
    HOR = 0, VER = 1,       // rotation
    SHIPS, SHOTS, ALL       // render 
};

enum CellState {
    WATER, SHIP, HIT, MISS, // determines color
    PLACE, SHOOT, MULTI    // decides which UI 
};

//---------------------------------------Struct------------------------------------------------

//--------------------------------------KLASSEN----------------------------------------------

// . ~ nichts  * Schiff  O kein Treffer  X Treffer  # Cursor  | = 124  – = 150  — = 151 versenkt

class Setup {
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
    Setup() :
        playerCount(1),
        mode(Settings::STANDARD),
        distance(Settings::CLOSE),
        shipSetting(Settings::FIVE),
        fieldSize(10),
        shipCount(0),
        aiDifficulty(Settings::NORMAL) {}

    // diff between presets 
    void setPreset(Settings difficulty) {
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
    void genAxis() {
        xAxisLabel.clear();
        yAxisLabel.clear();
        // generating the alphabet on the x-axis
        for (int i = 1; i <= fieldSize; i++) {
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
    void genShips() {
        shipsLenght.clear();
        if (!shipCount) {
            if (shipSetting == Settings::FIVE){
                int shipFields = fieldSize * fieldSize * (static_cast<int>(shipSetting) / 100.);
                while (shipFields >= 2) {
                    for (int num : lengthShipsFIVE) {
                        if (num > shipFields) break;
                        shipFields -= num;
                        shipsLenght.push_back(num);
                    }
                }
            }else if (shipSetting == Settings::TEN){
                int shipFields = fieldSize * fieldSize * (static_cast<int>(shipSetting) / 100.);
                while (shipFields >= 2) {
                    for (int num : lengthShipsTEN) {
                        if (num > shipFields) break;
                        shipFields -= num;
                        shipsLenght.push_back(num);
                    }
                }
            }
        }else {
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
            lenghtsCount[i] = count(shipsLenght.begin(), shipsLenght.end(), i+2);
        }
    }
    // displays the current rules
    const void displayRules() {
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
        (shipCount) ? num = shipCount : num = shipsLenght.size() ;
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
    }
};

Setup gameSetup;
// time in ms
int sleepTime = 1000;
// console dimensions
int consoleWidth, consoleHeight;

class Player {
public:
    class Ships;
    int number;         // player number
    std::string name;        // player name
    int score = 0;      // session score calc -> win, ships sunken points
    std::vector<std::vector<char>> field; // own player field (z.B. '.', 'O', 'X', '#')
    std::vector<std::vector<char>> ships; // ship position for display
    std::vector<std::vector<char>> shots; // shot position
    std::vector<Ships> allShips;     // list of all ships of a player 
    int shotsFired = 0;

    // create the player with a number
    Player(int number) : number(number) {}
    // prints the field of the current player
    void displayField() {
        //render(*this);  // displayes only field
    }
    //
    void shoot() {}
    //
    void placeShips() {
        std::cout << name << ", your turn\n";
        // cursor start position in center of field
        std::pair<bool, std::pair<int, int>> cursor = { true, {gameSetup.fieldSize / 2, gameSetup.fieldSize / 2} };
        for (int lenght : gameSetup.lenghts) {
            for (int count = gameSetup.lenghtsCount[lenght - 2]; count > 0; count--) {
                bool placed = false;
                Settings direction = Settings::HOR;  // 0 = horizontal  1 = vertical
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
                    else if (input == static_cast<int> (Settings::DOWN) && ((direction == Settings::VER) ? col + lenght < size : col < size - 1)) {
                        cursor.second.first++;
                    }
                    else if (input == static_cast<int> (Settings::LEFT) && row > 0) {
                        cursor.second.second--;
                    }
                    else if (input == static_cast<int> (Settings::RIGHT) && ((direction == Settings::HOR) ? row + lenght < size : row < size - 1)) {
                        cursor.second.second++;
                    }
                    else if (input == static_cast<int> (Settings::ROTATE) && row + lenght <= size && col + lenght <= size) {
                        direction = (direction == Settings::HOR) ? Settings::VER : Settings::HOR;
                    }
                    else if (input == 13/*ENTER*/) {   // placing ships
                        bool valid = true;
                        if (direction == Settings::HOR) {   // horizontal
                            for (int j = 0; j < lenght; j++) {
                                if (ships[col][row + j] == '*') {
                                    valid = false;  // already a ship
                                    break;
                                }
                            }
                        }
                        else if (direction == Settings::VER) {  // vertical
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

                            if (direction == Settings::HOR) {   // 
                                for (int j = 0; j < lenght; j++) {
                                    ships[col][row + j] = '*';
                                }
                            }
                            else if (direction == Settings::VER) {
                                for (int j = 0; j < lenght; j++) {
                                    ships[col + j][row] = '*';
                                }
                            }
                            std::cout << "Placed a " << lenght << " long ship " << (direction == Settings::HOR ? "horizontally" : "vertically") << "\n";
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
    // generates the field for every player 
    void genFields() {
        field.clear();
        field.resize(gameSetup.fieldSize);
        for (int i = 0; i < gameSetup.fieldSize; ++i) {
            field[i].resize(gameSetup.fieldSize, '~');   // inserting . as place holder
        }
        ships.clear();
        ships.resize(gameSetup.fieldSize);
        for (int i = 0; i < gameSetup.fieldSize; ++i) {
            ships[i].resize(gameSetup.fieldSize, ' ');   // inserting " " as place holder
        }
        shots.clear();
        shots.resize(gameSetup.fieldSize);
        for (int i = 0; i < gameSetup.fieldSize; ++i) {
            shots[i].resize(gameSetup.fieldSize, ' ');   // inserting " " as place holder
        }
    }
    // Registers the player and saves the name
    void askName() {
        titleBox();
        cursorPosition(consoleHeight * 0.45, consoleWidth * 0.25);
        std::cout << "Player " << number << " whats your name?\n";
        cursorPosition(consoleHeight * 0.45 + 2, consoleWidth * 0.25);
        std::cin >> name;
        cursorPosition(consoleHeight * 0.45 + 4, consoleWidth * 0.25);
        std::cout << "Hello " << name << " welcome to Battleships!\n";
        Sleep(sleepTime);
    }
    // score considers win and sunken ships and hits (and total shots)
    void /*update?*/scoring() {} // Diese Funktion sollte aufgerufen werden, wenn ein Spieler ein Schiff versenkt oder das Spiel gewinnt.(Verhältnis von Treffern zu Schüssen)

    class Ships {
    public:
        int size;
        bool sunken;
        Settings rotation;
        std::pair<int, int> cord;
        std::vector<bool> hits;

        Ships(int s, Settings r, std::pair<int, int> c) : size(s), rotation(r), cord(c), sunken(false){
            hits.resize(s, false);
        }
    };
};

//--------------------------------------GLOBAL--------------------------------------------

GameState state = GameState::MENU; 
std::stack<MenuID> menuStack;



// 
int fieldDisplayWidth = gameSetup.fieldSize * 2 + 6;
int fieldDisplayHeight = gameSetup.fieldSize + 1 + 4;

//-----------------------------------Deklaration--------------------------------------

void displayMenu(const std::vector<std::pair<char, std::string>>& options);
MenuID goback(std::stack<MenuID>& stack);
void clearConsole();
void genFields(std::vector<Player>& players);
void initGame(std::vector<Player>& players);

void placing(std::vector<Player>& players);
void render(Player& player, Settings setting = Settings::ALL, std::pair<bool, std::pair<int, int>> cursor = { false, {0,0} }, int size = 0, Settings rotation = Settings::HOR);

void gameLoop(std::vector<Player>& players);

void titleBox();

//-------------------------------MAPS--------------------------------------------

// data map for the menu text
std::map<MenuID, std::vector<std::pair<char, std::string>>> menuData = {
    {MenuID::MAIN, {
        {'1', "Play"},
        {'2', "Leaderboard"},
        {'3', "Rules"},
        {'4', "Options"},
        {'0', "Close"}}},
    {MenuID::PLAY, {
        {'1', "AI"},
        {'2', "2 Player"},
        {'3', "3 Player"},
        {'4', "4 Player"},
        {'0', "Back"}}},
    {MenuID::PRESETS, {
        {'1', "Small"},
        {'2', "Medium"},
        {'3', "Big"},
        {'4', "Custom"},
        {'0', "Back"}}},
    {MenuID::OPTIONS, {
        {'1', "Mode"},
        {'2', "Fieldsize"},
        {'3', "Ships count(automatisch)"},
        {'4', "Ships count (manuell)"},
        {'5', "Distance"},
        {'6', "AI Difficulty"},
        {'0', "Back"}}},
    {MenuID::MODE, {
        {'1', "Normal"},
        {'2', "Salvo"},
        {'3', "Race"},
        {'0', "Back"}}},
    {MenuID::AI, {
        {'1', "Easy"},
        {'2', "Normal"},
        {'3', "Hard"},
        {'0', "Back"}}}
};

// map for the coresponding actions for the choices in menus 
std::map<MenuID, std::map<char, std::function<void()>>> menuActions = {
    {MenuID::MAIN, {
        {'1', []() {menuStack.push(MenuID::PRESETS); }},
        {'2', []() {/* TODO: Rangliste SQL?*/}},
        {'3', []() {gameSetup.displayRules(); }},
        {'4', []() {menuStack.push(MenuID::OPTIONS); }},
        {'0', []() {/*return*/}}}},
    {MenuID::PLAY, {
        {'1', []() {gameSetup.playerCount = 1; state = GameState::GAME_LOOP; }},
        {'2', []() {gameSetup.playerCount = 2; state = GameState::GAME_LOOP; }},
        {'3', []() {gameSetup.playerCount = 3; state = GameState::GAME_LOOP; }},
        {'4', []() {gameSetup.playerCount = 4; state = GameState::GAME_LOOP; }},
        {'0', []() {goback(menuStack); }}}},
    {MenuID::PRESETS, {
        {'1', []() {gameSetup.setPreset(Settings::SMALL); menuStack.push(MenuID::PLAY); gameSetup.genShips(); }},
        {'2', []() {gameSetup.setPreset(Settings::MEDIUM); menuStack.push(MenuID::PLAY); gameSetup.genShips(); }},
        {'3', []() {gameSetup.setPreset(Settings::LARGE); menuStack.push(MenuID::PLAY); gameSetup.genShips(); }},
        {'4', []() {menuStack.push(MenuID::PLAY); }},
        {'0', []() {goback(menuStack); }}}},
    {MenuID::OPTIONS, {
        {'1', []() {menuStack.push(MenuID::MODE); }},
        {'2', []() {
            cursorPosition(consoleHeight * 0.7, consoleWidth * 0.25);
            std::cout << "Current: " << gameSetup.fieldSize << std::endl << std::endl;
            cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "How big should the field be?" << std::endl;
            cursorHorizontalAbsolute(consoleWidth * 0.25);
            int size;
            std::cin >> size;
            if (std::cin.fail()) { //Fehlerbehandlung
                std::cout << "Invalid Input. Please input a number.";
                Sleep(sleepTime);
                std::cin.clear();
                return;
            }
            gameSetup.fieldSize = size;
        }},
        {'3', []() {
            cursorPosition(consoleHeight * 0.65, consoleWidth * 0.25);
            if (gameSetup.shipSetting == Settings::TEN){
                std::cout << "Current: many" << std::endl << std::endl;
            }else if (gameSetup.shipSetting == Settings::FIVE){
                std::cout << "Current: few" << std::endl << std::endl;
            }
            cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "How many ships?\n\n";
            cursorHorizontalAbsolute(consoleWidth * 0.25); 
            std::cout << "[1]  few\n";
            cursorHorizontalAbsolute(consoleWidth * 0.25); 
            std::cout << "[2]  many\n";
            int num = _getch();
            if (0 < num < 3) { num == 1 ? gameSetup.shipSetting = Settings::FIVE : gameSetup.shipSetting = Settings::TEN; };
            if (num == '1') {
                gameSetup.shipSetting = Settings::FIVE;
            }else if (num == '2') {
               gameSetup.shipSetting = Settings::TEN;
            }else {
                cursorHorizontalAbsolute(consoleWidth * 0.25);
                std::cout << "Invalid Input.\n";
                Sleep(sleepTime);
                return;
            }
            gameSetup.shipCount = 0;
            gameSetup.genShips();
        }},
        {'4', []() {
            cursorPosition(consoleHeight * 0.65, consoleWidth * 0.25);
            std::cout << "Current: " << gameSetup.shipCount << std::endl << std::endl;
            cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "How many ships?\t(This setting overwrites the automatic number!)\n";
            cursorHorizontalAbsolute(consoleWidth * 0.25);
            int num;
            std::cin >> num;
            if (std::cin.fail()) { //Fehlerbehandlung
                std::cout << "Invalid Input. Please input a number.\n";
                Sleep(sleepTime);
                std::cin.clear();
                return;
            }
            gameSetup.shipCount = num;
            gameSetup.genShips();
        }},
        {'5', []() {
            cursorPosition(consoleHeight * 0.65, consoleWidth * 0.25);
            if (gameSetup.shipSetting == Settings::CLOSE) {
                std::cout << "Current: close\n\n";
            }
            else if (gameSetup.shipSetting == Settings::AWAY) {
                std::cout << "Current: far\n\n";
            }
            cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "What distance should the ships be?\n";
                cursorHorizontalAbsolute(consoleWidth * 0.25); 
            std::cout << "[1]  close\n";
                cursorHorizontalAbsolute(consoleWidth * 0.25); 
            std::cout << "[2]  far\n";
            int num = _getch();
            if (num == '1') {
                gameSetup.distance = Settings::CLOSE;
            }else if (num == '2') {
                gameSetup.distance = Settings::AWAY;
            }else {
                std::cout << std::endl;
                cursorHorizontalAbsolute(consoleWidth * 0.25);
                std::cout << "Invalid Input.\n";
                Sleep(sleepTime);
               return;
            }
        }},
        {'6', []() {menuStack.push(MenuID::AI); }},
        {'0', []() {goback(menuStack); }}}},
    {MenuID::MODE, {
        {'1', []() {gameSetup.mode = Settings::NORMAL; goback(menuStack); }},
        {'2', []() {gameSetup.mode = Settings::SALVO; goback(menuStack); }},
        {'3', []() {gameSetup.mode = Settings::RACE; goback(menuStack); }},
        {'0', []() {goback(menuStack); }}}},
    {MenuID::AI, {
        {'1', []() {gameSetup.aiDifficulty = Settings::EASY; goback(menuStack); }},
        {'2', []() {gameSetup.aiDifficulty = Settings::NORMAL; goback(menuStack); }},
        {'3', []() {gameSetup.aiDifficulty = Settings::HARD; goback(menuStack); }},
        {'0', []() {goback(menuStack); }}}}
};

//----------------------------------------MAIN--------------------------------------------------

int main() {
    // enables console manipulation
    enableVirtualTerminalProcessing();
    // getting console dimensions
    getConsoleSize(consoleWidth, consoleHeight);
    // starting with main menu
    menuStack.push(MenuID::MAIN);
    // saving all players
    std::vector<Player> players;

    hideCursor();

    while (true) {
        if (state == GameState::MENU) {
            MenuID currentMenuID = menuStack.top();
            std::vector<std::pair<char, std::string>> currentMenu = menuData[currentMenuID];
            gameSetup.genShips();
            displayMenu(currentMenu);

            char choice = _getch();
            bool validInput = false;
            for (const auto& option : currentMenu) {
                if (option.first == choice) {
                    validInput = true;
                    break;
                }
            }
            if (validInput) {
                menuActions[currentMenuID][choice]();  // perform specific action
            }else {
                std::cout << "Wrong Input" << std::endl;
                Sleep(sleepTime);
                continue;
            }
        }else if (state == GameState::GAME_LOOP){
            initGame(players);
            //render(players[2], Settings::ALL);
            placing(players);
            gameLoop(players);
        }else if (state == GameState::GAME_OVER){

        }
    }
    return 0;
}

//---------------------------------------GAME--------------------------------------------------

void initGame(std::vector<Player>& players) {
    // clear screen
    eraseInDisplay(2);
    gameSetup.genAxis();
    gameSetup.genShips();
    cursorPosition(consoleHeight * 0.45, consoleWidth * 0.25);
    for (int i = 0; i < gameSetup.playerCount; i++) {
        players.push_back(i + 1);
        players[i].askName();
        cursorPosition(consoleHeight * 0.65 + i*2, consoleWidth * 0.25);
    }
    if (gameSetup.playerCount == 1) { players.push_back(2); players[1].name = "AI"; }
    genFields(players);
    // placing ships for AI
    if (gameSetup.playerCount == 1) {
        for (int lenght : gameSetup.lenghts) {
            for (int count = gameSetup.lenghtsCount[lenght - 2]; count > 0; count--) {
                bool placed = false;
                while (!placed) {
                    Settings direction;
                    (rand() % 1) ? direction = Settings::HOR : direction = Settings::VER;
                    int col = rand() % (direction == Settings::VER ? gameSetup.fieldSize - lenght + 1 : gameSetup.fieldSize);
                    int row = rand() % (direction == Settings::HOR ? gameSetup.fieldSize - lenght + 1 : gameSetup.fieldSize);
                    std::pair<int, int> cursor = { col, row };
                    bool valid = true;
                    if (direction == Settings::HOR) {   // horizontal
                        for (int j = 0; j < lenght; j++) {
                            if (players[1].ships[col][row + j] == '*') {
                                valid = false;  // already a ship
                                break;
                            }
                        }
                    }
                    else if (direction == Settings::VER) {  // vertical
                        for (int j = 0; j < lenght; j++) {
                            if (players[1].ships[col + j][row] == '*') {
                                valid = false;  // already a ship
                                break;
                            }
                        }
                    }
                    if (valid && gameSetup.distance == Settings::AWAY) {}
                    if (valid) {
                        if (direction == Settings::HOR) {   // 
                            for (int j = 0; j < lenght; j++) {
                                players[1].ships[col][row + j] = '*';
                            }
                            players[2].allShips.emplace_back(lenght, direction, cursor);
                        }
                        else if (direction == Settings::VER) {
                            for (int j = 0; j < lenght; j++) {
                                players[1].ships[col + j][row] = '*';
                            }
                            players[1].allShips.emplace_back(lenght, direction, cursor);
                        }
                        placed = true;
                    }
                }
            }
        }
    }
    //TODO: Schiffe platzieren.  Die Schiffe müssen jetzt platziert werden, entweder
    // automatisch oder durch den Spieler.  Dies ist ein komplexer Teil des Spiels,
    // der die Interaktion mit dem Spieler und die Überprüfung der Platzierung
    // beinhaltet.
}

void debugShips(std::vector<Player>& players)  {
    for (int i = 0; i < players.size(); i++) {
        for (const auto& ship : players[i].allShips) {
            std::cout << "Ship: size=" << ship.size << ", dir=" << (ship.rotation == Settings::HOR ? "HOR" : "VER")
                << ", pos=(" << ship.cord.first << "," << ship.cord.second << "), sunken=" << ship.sunken << "\n";
        }
    }
}

void placing(std::vector<Player>& players) {
    for (int i = 0; i < gameSetup.playerCount; i++) {
        clearConsole();
        std::cout << players[i].name << ", your turn\n";
        // cursor start position in center of field
        std::pair<bool, std::pair<int, int>> cursor = { true, {gameSetup.fieldSize / 2, gameSetup.fieldSize / 2} };
        for (int lenght : gameSetup.lenghts) {
            for (int count = gameSetup.lenghtsCount[lenght - 2]; count > 0; count--) {
                bool placed = false;
                Settings direction = Settings::HOR;  // 0 = horizontal  1 = vertical
                while (!placed) {
                    render(players[i], Settings::SHIPS, cursor, lenght, direction);
                    int col = cursor.second.first;  // vertical
                    int row = cursor.second.second; // horizontal
                    int size = gameSetup.fieldSize;
                    //cout << "lenght: " << lenght << endl << "size: " << size << endl << "row: " << row << "  col: " << col << endl;
                    int input = _getch();
                    if (input == 224) input = _getch();
                    if (input == static_cast<int> (Settings::UP) && col > 0) {
                        cursor.second.first--;
                    }
                    else if (input == static_cast<int> (Settings::DOWN) && ((direction == Settings::VER) ? col + lenght < size : col < size - 1)) {
                        cursor.second.first++;
                    }
                    else if (input == static_cast<int> (Settings::LEFT) && row > 0) {
                        cursor.second.second--;
                    }
                    else if (input == static_cast<int> (Settings::RIGHT) && ((direction == Settings::HOR) ? row + lenght < size : row < size - 1)) {
                        cursor.second.second++;
                    }
                    else if (input == static_cast<int> (Settings::ROTATE) && row + lenght <= size && col + lenght <= size) {
                        direction = (direction == Settings::HOR) ? Settings::VER : Settings::HOR;
                    }
                    else if (input == static_cast<int> (Settings::ENTER)) {   // placing ships
                        bool valid = true;
                        if (direction == Settings::HOR) {   // horizontal
                            for (int j = 0; j < lenght; j++) {
                                if (players[i].ships[col][row + j] == '*') {
                                    valid = false;  // already a ship
                                    break;
                                }
                            }
                        }
                        else if (direction == Settings::VER) {  // vertical
                            for (int j = 0; j < lenght; j++) {
                                if (players[i].ships[col + j][row] == '*') {
                                    valid = false;  // already a ship
                                    break;
                                }
                            }
                        }
                        if (valid && gameSetup.distance == Settings::AWAY) {

                        }
                        if (valid) {

                            if (direction == Settings::HOR) {   // 
                                for (int j = 0; j < lenght; j++) {
                                    players[i].ships[col][row + j] = '*';
                                }
                                players[i].allShips.emplace_back(lenght, direction, cursor.second);
                            }
                            else if (direction == Settings::VER) {
                                for (int j = 0; j < lenght; j++) {
                                    players[i].ships[col + j][row] = '*';
                                }
                                players[i].allShips.emplace_back(lenght, direction, cursor.second);
                            }
                            std::cout << "Placed a " << lenght << " long ship " << (direction == Settings::HOR ? "horizontally" : "vertically") << "\n";
                            placed = true;
                        }
                        else {
                            std::cout << "Invalid placement! Try again.\n";
                        }
                    }
                }
            }
        }
    }
}

void render(Player& player, Settings setting, std::pair<bool, std::pair<int, int>> cursor, int size, Settings rotation) {
    std::vector<std::vector<char>> renderField = player.field;

    int col = cursor.second.first;
    int row = cursor.second.second;

    // setting what to render
    for (int i = 0; i < gameSetup.fieldSize; i++) { 
        for (int j = 0; j < gameSetup.fieldSize; j++) {
            if (setting == Settings::SHIPS) {
                if (player.ships[i][j] != ' ') {
                    renderField[i][j] = player.ships[i][j];
                }
            }
            else if (setting == Settings::SHOTS) {
                if (player.shots[i][j] != ' ') {
                    renderField[i][j] = player.shots[i][j];
                }
            }
            else if (setting == Settings::ALL) {
                if (player.ships[i][j] != ' ') {
                    renderField[i][j] = player.ships[i][j];
                }
                if (player.shots[i][j] != ' ') {
                    renderField[i][j] = player.shots[i][j];
                }
            }
        }
    }
    if (cursor.first && size > 0) {     // placing ships
        if (rotation == Settings::HOR) {
            if (gameSetup.distance == Settings::CLOSE) {
                for (int i = 0; i < size; i++) {
                    renderField[col][row + i] = player.ships[col][row + i] == '*' ? 'X' : '#';
                }
            }else if (gameSetup.distance == Settings::AWAY) {
                
            }
            
        }else if (rotation == Settings::VER) {
            if (gameSetup.distance == Settings::CLOSE) {
                for (int i = 0; i < size; i++) {
                    renderField[col + i][row] = player.ships[col + i][row] == '*' ? 'X' : '#';
                }
            }else if (gameSetup.distance == Settings::AWAY) {

            }
        }
    }else if (cursor.first/*&& !size*/) {
        renderField[col][row] = '#';    // cursor
    }

    clearConsole();

    if (size > 0) {
        std::cout << player.name << " your turn\n";
        std::cout << "Placing a " << size << " long ship (" << (rotation == Settings::HOR ? "horizontal" : "vertical") << ")\n";
    }
    else {
        std::cout << player.name << " take your shot\n";
    }
    // x-axis
    std::cout << "   ";
    for (int k = 0; k < gameSetup.fieldSize; k++) {
        k < 26 && gameSetup.fieldSize > 26 ? std::cout << gameSetup.xAxisLabel[k] << "  " : std::cout << gameSetup.xAxisLabel[k] << " ";
    }
    std::cout << std::endl;
    // y-axis and field
    for (int i = 0; i < gameSetup.fieldSize; i++) {
        if (gameSetup.yAxisLabel[i] < 10) std::cout << " ";
        std::cout << gameSetup.yAxisLabel[i] << " ";
        for (int j = 0; j < gameSetup.fieldSize; j++) std::cout << renderField[i][j] << (gameSetup.fieldSize > 26 ? "  " : " ");
        std::cout << std::endl;
    }
}

// generates the field for every player 
void genFields(std::vector<Player>& players) {
    for (Player& player : players) {
        player.field.clear();
        player.field.resize(gameSetup.fieldSize);
        for (int i = 0; i < gameSetup.fieldSize; ++i) {
            player.field[i].resize(gameSetup.fieldSize, '~');   // inserting . as place holder
        }
        player.ships.clear();
        player.ships.resize(gameSetup.fieldSize);
        for (int i = 0; i < gameSetup.fieldSize; ++i) {
            player.ships[i].resize(gameSetup.fieldSize, ' ');   // inserting " " as place holder
        }
        player.shots.clear();
        player.shots.resize(gameSetup.fieldSize);
        for (int i = 0; i < gameSetup.fieldSize; ++i) {
            player.shots[i].resize(gameSetup.fieldSize, ' ');   // inserting " " as place holder
        }
    }
    
}

// game function
void game() {
}

// 
void gameLoop(std::vector<Player>& players) {
    std::pair<bool, std::pair<int, int>> cursor = { true, {gameSetup.fieldSize / 2, gameSetup.fieldSize / 2} };
    bool gameWon = false;
    while (!gameWon) {
        if (gameSetup.playerCount == 1) {
            render(players[2], Settings::SHOTS, cursor);
            int col = cursor.second.first;  // vertical
            int row = cursor.second.second; // horizontal
            int input = _getch();
            if (input == 224) input = _getch();
            if (input == static_cast<int> (Settings::UP) && col > 0) {
                cursor.second.first--;
            }
            else if (input == static_cast<int> (Settings::DOWN) && col < gameSetup.fieldSize - 1) {
                cursor.second.first++;
            }
            else if (input == static_cast<int> (Settings::LEFT) && row > 0) {
                cursor.second.second--;
            }
            else if (input == static_cast<int> (Settings::RIGHT) && row < gameSetup.fieldSize - 1) {
                cursor.second.second++;
            }
            else if (input == static_cast<int> (Settings::ENTER)) {
                bool valid = false;
                if (players[2].ships[col][row] == '*') {
                    players[2].shots[col][row] = 'X';
                    std::cout << "HIT\n";
                }
                else {
                    players[2].shots[col][row] = 'O';
                    std::cout << "MISS\n";
                }
            }
        }else{
            for (int i = 0; i < gameSetup.playerCount; i++) {
                render(players[i], Settings::SHOTS, cursor);
                int col = cursor.second.first;  // vertical
                int row = cursor.second.second; // horizontal
                int input = _getch();
                if (input == static_cast<int> (Settings::UP) && col > 0) {
                    cursor.second.first--;
                }
                else if (input == static_cast<int> (Settings::DOWN) && col < gameSetup.fieldSize - 1) {
                    cursor.second.first++;
                }
                else if (input == static_cast<int> (Settings::LEFT) && row > 0) {
                    cursor.second.second--;
                }
                else if (input == static_cast<int> (Settings::RIGHT) && row < gameSetup.fieldSize - 1) {
                    cursor.second.second++;
                }
                else if (input == static_cast<int> (Settings::ENTER)) {
                    bool valid = false;
                    if (players[i].ships[col][row] == '*') {
                        players[i].shots[col][row] = 'X';
                        std::cout << "HIT\n";
                    }
                    else {
                        players[i].shots[col][row] = 'O';
                        std::cout << "MISS\n";
                    }
                }
            }
        }
    }

    //TODO: Implementiere die Spiellogik.  Dies ist das Herz des Spiels.  Es beinhaltet:
    // 1.  Den Spielern abwechselnd das Schießen zu ermöglichen.
    // 2.  Die Eingabe des Spielers zu verarbeiten (z.B. welcher Punkt beschossen wird).
    // 3.  Zu überprüfen, ob ein Schiff getroffen wurde.
    // 4.  Den Zustand des Spielfelds zu aktualisieren.
    // 5.  Zu überprüfen, ob ein Schiff versenkt wurde.
    // 6.  Zu überprüfen, ob das Spiel vorbei ist (alle Schiffe eines Spielers versenkt).
    // 7.  Die Punktzahl zu aktualisieren.
    // 8.  Das Spielfeld anzuzeigen.
}

//-------------------------------------FUNKTIONEN-----------------------------------------------

// displays the selected option menu
void displayMenu(const std::vector<std::pair<char, std::string>>& options) {
    titleBox();
    cursorPosition(consoleHeight * 0.35, consoleWidth * 0.25);
    for (auto& pair : options) {
        std::cout << "[" << pair.first << "]  " << pair.second << std::endl;
        cursorHorizontalAbsolute(consoleWidth * 0.25);
    }
}

// go one menu back (pop top menu from stack)
MenuID goback(std::stack<MenuID>& stack) {
    if (stack.size() > 1) {
        stack.pop();
        return stack.top();
    }else {
        std::cout << "Fehler" << std::endl;
        return stack.top();
    }
}

// clears the console with a command
void clearConsole() {
    // more efficeint clear where only inside of the box is erased not the box itself
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

//------------------------------------------------------------------------------------

void titleBox() {
    // clear screen
    eraseInDisplay(2);
    // visual box around the console
    cursorPosition(1, 1);
    for (int i = 0; i < (consoleWidth - 17) / 2; i++) std::cout << "=";
    selectGraphicRendition(1);
    std::cout << " [ BATTLESHIPS ] ";
    selectGraphicRendition(0);
    for (int i = 0; i < (consoleWidth - 16) / 2; i++) std::cout << "=";
    std::cout << "\n";
    for (int i = 0; i < consoleHeight - 2; i++) std::cout << "|\n";
    for (int i = 0; i < consoleWidth; i++) std::cout << "=";
    cursorPosition(2, consoleWidth);
    for (int i = 0; i < consoleHeight - 2; i++) { cursorHorizontalAbsolute(consoleWidth); std::cout << "|\n"; }
}

void currentControlls() {

    // String operation add to string
    //      or
    // pre plan for each UI

    int width, height;
    getConsoleSize(width, height);
    cursorPosition(height - 2, 3);
    std::cout << "CONTROLLS:\n";
    cursorHorizontalAbsolute(3);
    std::cout << "ENTER - change Display | ESC - go back | ARROWS - move";
}

/*

void fieldBox(int x, int y, Player* p) {
    // title or who's board 
    cursorPosition(x - 3, y - 1);
    selectGraphicRendition(1);
    std::cout << (p ? p->name + " Board" : " Your Board");
    selectGraphicRendition(0);
    std::cout.flush();
    // box 
    int width = fieldDisplayWidth - 1;
    cursorPosition(x - 2, y - 3);
    std::cout << "+";
    for (int i = 0; i < width; ++i) std::cout << "-";
    std::cout << "+";
    for (int i = 0; i <= fieldDisplayHeight - 2; ++i)
    {
        cursorPosition(x - 1 + i, y - 3);
        std::cout << "|";
        cursorPosition(x - 1 + i, y + width - 2);
        std::cout << "|";
    }
    cursorPosition(x + fieldDisplayHeight - 3, y - 3);
    std::cout << "+";
    for (int i = 0; i < width; ++i) std::cout << "-";
    std::cout << "+";
    std::cout.flush();

}

void displayField(int size) {
    size++;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // field
            if (i == 0 && j == 0) std::cout << " ";
            else if (i == 0 && 1 < j < size) std::cout << " " << j - 1;
            else if (j == 0 && 0 < i < size) std::cout << " " << i - 1;
            else std::cout << " ~";

        }
        cursorBack(size * 2);
        cursorDown();
    }
}

void renderUI(int UI) {

    eraseInDisplay(2);
    // get console size
    int width, height;
    getConsoleSize(width, height);

    // visual box around the console
    cursorPosition(1, 1);
    for (int i = 0; i < (width - 17) / 2; i++) std::cout << "=";
    selectGraphicRendition(1);
    std::cout << " [ BATTLESHIPS ] ";
    selectGraphicRendition(0);
    for (int i = 0; i < (width - 16) / 2; i++) std::cout << "=";
    std::cout << "\n";
    for (int i = 0; i < height - 2; i++) std::cout << "|\n";
    for (int i = 0; i < width; i++) std::cout << "=";
    cursorPosition(2, width);
    for (int i = 0; i < height - 2; i++) { cursorHorizontalAbsolute(width); std::cout << "|\n"; }

    // printing different UI's
    if (UI == PLACE) {
        int fieldHeight = (height + 2 - fieldDisplayHeight) / 2 + 1;
        int fieldWidth = (width + 2 - fieldDisplayWidth) / 2 + 1;
        cursorPosition(fieldHeight, fieldWidth);
        displayField(gameSetup.fieldSize);
        fieldBox(fieldHeight, fieldWidth, &enemy);
        cursorPosition(height + 2, 1);
    }
    else if (UI == SHOOT) {
        int fieldHeight = (height + 2 - fieldDisplayHeight) / 2 + 1;
        int fieldWidth = (width + 2 - fieldDisplayWidth) / 3 + 1;
        cursorPosition(fieldHeight, fieldWidth);

        displayField(gameSetup.fieldSize);
        fieldBox(fieldHeight, fieldWidth, &enemy);
        cursorPosition(fieldHeight, 2 * fieldWidth);
        displayField(gameSetup.fieldSize);
        fieldBox(fieldHeight, 2 * fieldWidth, &enemy);
        cursorPosition(height + 2, 1);
    }
    else if (UI == MULTI) {
        for (int i = 1; i <= gameSetup.playerCount; i++)
        {
            int fieldY = (height + 2 - fieldDisplayHeight) / 2 + 1;
            int fieldX = ((width + 2 - fieldDisplayWidth) * (i / (gameSetup.playerCount + 1.))) + 1;
            cursorPosition(fieldY, fieldX);
            displayField(gameSetup.fieldSize);
            fieldBox(fieldY, fieldX, &enemy);
        }
    }
}

*/



/*
void placeShips(vector<Player>& players) {
    pair<int, int> cursor(gameSetup.fieldSize / 2, gameSetup.fieldSize / 2);
    for (int i = 0; i < gameSetup.playerCount; i++) {
        clearConsole();
        cout << players[i].name << " your turn\n";
        for (int num : gameSetup.lenghts){
            for (int count = gameSetup.lenghtsCount[num - 2]; count > 0; count--) {
                bool selected = false;
                bool placed = false;
                while (!selected){
                    if (1 < cursor.first < gameSetup.fieldSize && 1 < cursor.second < gameSetup.fieldSize){
                        int input = _getch();
                        if (input == static_cast<int> (Settings::UP)) {
                            cursor.first--;
                            select(players[i], cursor, num);
                        }else if (input == static_cast<int> (Settings::DOWN)) {
                            cursor.first++;
                            select(players[i], cursor, num);
                        }else if (input == static_cast<int> (Settings::LEFT)) {
                            cursor.second--;
                            select(players[i], cursor, num);
                        }else if (input == static_cast<int> (Settings::RIGHT)) {
                            cursor.second++;
                            select(players[i], cursor, num);
                        }else if (input == 13) {
                            selected = 1;
                            cout << "Choose a direction [h] = horizontal [v] = vertical\n";
                            while (!placed){
                                int dir = _getch();
                                if (dir == 118 || dir == 86){
                                    cout << "Placed a " << num << " long ship verticaly\n";
                                    placed = 1;
                                }else if (dir == 104 || dir == 72) {
                                    cout << "Placed a " << num << " long ship horizontaly\n";
                                    placed = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
*/