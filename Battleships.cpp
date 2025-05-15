#include "Game.h"
#include "console_utils.h"

int main() {
    console_utils::enableVirtualTerminalProcessing();
    Game game;
    game.run();
    return 0;
}

/*

#include "Player.h"
#include <iostream>
#include <string>
#include <vector>
#include <conio.h> // for _getch
#include <map>
#include <stack>
#include <functional>
#include <limits> // for numeric_limits cin
#include <utility> // for std::pair


void titleBox();
void currentControlls(int UI);

//---------------------------------------ENUM--------------------------------------------------

//---------------------------------------Struct------------------------------------------------

//--------------------------------------KLASSEN----------------------------------------------

// . ~ nichts  * Schiff  O kein Treffer  X Treffer  # Cursor  | = 124  – = 150  — = 151 versenkt

Setup gameSetup;
// time in ms
const int sleepTime = 1000;
// console dimensions
int consoleWidth, consoleHeight;


//--------------------------------------GLOBAL--------------------------------------------

GameState state = GameState::MENU; 
std::stack<MenuID> menuStack;

int fieldDisplayWidth;
int fieldDisplayHeight;

//-----------------------------------Deklaration--------------------------------------

void displayMenu(const std::vector<std::pair<char, std::string>>& options);
MenuID goback(std::stack<MenuID>& stack);
void clearConsole();
void genFields(std::vector<Player>& players);
void initGame(std::vector<Player>& players);

void placing(std::vector<Player>& players);
void render(Player& player, Settings setting = Settings::ALL, int UI = 0, std::pair<bool, std::pair<int, int>> cursor = { false, {0,0} }, int size = 0, int rotation = HOR);

void gameLoop(std::vector<Player>& players);
void titleBox();

void currentControlls(int UI);

void fieldBox(int x, int y, Player* p);
void printField(std::vector<std::vector<char>> field, int fieldWidth);

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
        {'2', []() {}},
        {'3', []() {gameSetup.displayRules(); }},
        {'4', []() {menuStack.push(MenuID::OPTIONS); }},
        {'0', []() {}}}},
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
            currentControlls(FIELDSIZE);
            cursorPosition(consoleHeight * 0.6, consoleWidth * 0.25);
            std::cout << "Current: " << gameSetup.fieldSize << std::endl << std::endl;
            cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "How big should the field be?" << std::endl;
            cursorHorizontalAbsolute(consoleWidth * 0.25);
            showCursor();
            int size;
            std::cin >> size;
            if (std::cin.fail()) { //Fehlerbehandlung
                cursorHorizontalAbsolute(consoleWidth * 0.25);
                std::cout << "Invalid Input. Please input a number.";
                Sleep(sleepTime);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                hideCursor();
                return;
            }
            hideCursor();
            gameSetup.fieldSize = size;
        }},
        {'3', []() {
            cursorPosition(consoleHeight * 0.6, consoleWidth * 0.25);
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
            currentControlls(SHIPCOUNT);
            cursorPosition(consoleHeight * 0.6, consoleWidth * 0.25);
            std::cout << "Current: " << gameSetup.shipCount << std::endl << std::endl;
            cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "How many ships?\t(This setting overwrites the automatic number!)\n";
            cursorHorizontalAbsolute(consoleWidth * 0.25);
            showCursor();
            int num;
            std::cin >> num;
            if (std::cin.fail()) { //Fehlerbehandlung
                std::cout << "Invalid Input. Please input a number.\n";
                Sleep(sleepTime);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                hideCursor();
                return;
            }
            hideCursor();
            gameSetup.shipCount = num;
            gameSetup.genShips();
        }},
        {'5', []() {
            cursorPosition(consoleHeight * 0.6, consoleWidth * 0.25);
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
                cursorPosition(consoleHeight * 0.7, consoleWidth * 0.25);
                std::cout << "Wrong Input" << std::endl;
                Sleep(sleepTime);
                continue;
            }
        }else if (state == GameState::GAME_LOOP){
            initGame(players);
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

    // 
    fieldDisplayWidth = gameSetup.fieldSize * 2 + 9;
    fieldDisplayHeight = gameSetup.fieldSize + 1 + 4;
    gameSetup.genAxis();
    gameSetup.genShips();
    cursorPosition(consoleHeight * 0.45, consoleWidth * 0.25);
    for (int i = 0; i < gameSetup.playerCount; i++) {
        players.push_back(i + 1);
        players[i].askName();
        cursorPosition(consoleHeight * 0.65 + i * 2, consoleWidth * 0.25);
    }
    if (gameSetup.playerCount == 1) { players.push_back(2); players[1].name = "AI"; }
    genFields(players);
    // placing ships for AI
    if (gameSetup.playerCount == 1) {
        for (int lenght : gameSetup.lenghts) {
            for (int count = gameSetup.lenghtsCount[lenght - 2]; count > 0; count--) {
                bool placed = false;
                while (!placed) {
                    int direction = rand() % 2;
                    int col = rand() % (direction == VER ? gameSetup.fieldSize - lenght + 1 : gameSetup.fieldSize);
                    int row = rand() % (direction == HOR ? gameSetup.fieldSize - lenght + 1 : gameSetup.fieldSize);
                    std::pair<int, int> cursor = { col, row };
                    bool valid = true;
                    if (direction == HOR) {   // horizontal
                        for (int j = 0; j < lenght; j++) {
                            if (players[1].ships[col][row + j] == '*') {
                                valid = false;  // already a ship
                                break;
                            }
                        }
                    }
                    else if (direction == VER) {  // vertical
                        for (int j = 0; j < lenght; j++) {
                            if (players[1].ships[col + j][row] == '*') {
                                valid = false;  // already a ship
                                break;
                            }
                        }
                    }
                    if (valid && gameSetup.distance == Settings::AWAY) {}
                    if (valid) {
                        if (direction == HOR) {   // 
                            for (int j = 0; j < lenght; j++) {
                                players[1].ships[col][row + j] = '*';
                            }
                            players[1].allShips.emplace_back(lenght, direction, cursor);
                        }
                        else if (direction == VER) {
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
    cursorPosition(2, 2);
    for (int i = 0; i < players.size(); i++) {
        cursorHorizontalAbsolute(2);
        std::cout << players[i].name << std::endl;
        for (const auto& ship : players[i].allShips) {
            cursorHorizontalAbsolute(2);
            std::cout << "Ship: size=" << ship.size << ", dir=" << (ship.rotation == HOR ? "HOR" : "VER")
                << ", pos=(" << ship.cord.first << "," << ship.cord.second << "), sunken=" << ship.sunken << std::endl;
        }
    }
}

void placing(std::vector<Player>& players) {
    for (int i = 0; i < players.size(); i++) {
        if (gameSetup.playerCount == 1 && i == 1) break;
        titleBox();
        cursorPosition(consoleHeight * 0.3, consoleWidth * 0.2);
        std::cout << players[i].name << ", your turn\n";
        // cursor start position in center of field
        std::pair<bool, std::pair<int, int>> cursor = { true, {1, 1} };
        for (int lenght : gameSetup.lenghts) {
            for (int count = gameSetup.lenghtsCount[lenght - 2]; count > 0; count--) {
                bool placed = false;
                int direction = HOR;
                while (!placed) {
                    render(players[i], Settings::SHIPS, PLACING, cursor, lenght, direction);
                    int col = cursor.second.first;  // vertical
                    int row = cursor.second.second; // horizontal
                    int size = gameSetup.fieldSize;
                    //cout << "lenght: " << lenght << endl << "size: " << size << endl << "row: " << row << "  col: " << col << endl;
                    int input = _getch();
                    if (input == 224) input = _getch();
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
                    else if (input == static_cast<int> (Settings::ENTER)) {   // placing ships
                        bool valid = true;
                        if (direction == HOR) {   // horizontal
                            for (int j = 0; j < lenght; j++) {
                                if (players[i].ships[col][row + j] == '*') {
                                    valid = false;  // already a ship
                                    break;
                                }
                            }
                        }
                        else if (direction == VER) {  // vertical
                            for (int j = 0; j < lenght; j++) {
                                if (players[i].ships[col + j][row] == '*') {
                                    valid = false;  // already a ship
                                    break;
                                }
                            }
                        }
                        if (valid && gameSetup.distance == Settings::AWAY) {

                        }
                        cursorPosition(consoleHeight * 0.4, consoleWidth * 0.05);
                        if (valid) {

                            if (direction == HOR) {   // 
                                for (int j = 0; j < lenght; j++) {
                                    players[i].ships[col][row + j] = '*';
                                }
                                players[i].allShips.emplace_back(lenght, direction, cursor.second);
                            }
                            else if (direction == VER) {
                                for (int j = 0; j < lenght; j++) {
                                    players[i].ships[col + j][row] = '*';
                                }
                                players[i].allShips.emplace_back(lenght, direction, cursor.second);
                            }
                            std::cout << "Placed a " << lenght << " long ship\n";
                            cursorHorizontalAbsolute(consoleWidth * 0.05);
                            std::cout << (direction == HOR ? "horizontally" : "vertically") << "\n";
                            placed = true;
                        }
                        else {
                            std::cout << "Invalid placement! Try again.\n";
                        }
                        Sleep(1000);
                    }
                }
            }
        }
    }
    debugShips(players);
    Sleep(1000);
}

void newRender(Player& players) {
    // overworked render fuction that saves on runtime and loops 
}


void render(Player& player, Settings setting, int UI, std::pair<bool, std::pair<int, int>> cursor, int size, int rotation) {
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
        if (rotation == HOR) {
            if (gameSetup.distance == Settings::CLOSE) {
                for (int i = 0; i < size; i++) {
                    renderField[col][row + i] = player.ships[col][row + i] == '*' ? 'X' : '#';
                }
            }else if (gameSetup.distance == Settings::AWAY) {
                
            }
            
        }else if (rotation == VER) {
            if (gameSetup.distance == Settings::CLOSE) {
                for (int i = 0; i < size; i++) {
                    renderField[col + i][row] = player.ships[col + i][row] == '*' ? 'X' : '#';
                }
            }else if (gameSetup.distance == Settings::AWAY) {

            }
        }
    }else if (cursor.first) {
        renderField[col][row] = '#';    // cursor
    }
    titleBox();
    currentControlls(PLACING);
    cursorPosition(consoleHeight * 0.2, consoleWidth * 0.05);
    if (size > 0) {
        std::cout << player.name << " your turn\n\n";
        cursorHorizontalAbsolute(consoleWidth * 0.05);
        std::cout << "Placing a " << size << " long ship\n"; 
        cursorHorizontalAbsolute(consoleWidth * 0.05);
        std::cout << "(" << (rotation == HOR ? "horizontal" : "vertical") << ")\n";
    }
    else {
        std::cout << player.name << " take your shot\n";
    }

    // printing different UI's
    if (UI == PLACE) {
        int fieldHeight = (consoleHeight + 2 - fieldDisplayHeight) / 2 + 1;
        int fieldWidth = (consoleWidth + 2 - fieldDisplayWidth) / 2 + 1;

        cursorPosition(fieldHeight, fieldWidth);
        printField(renderField, fieldWidth);
        fieldBox(fieldHeight, fieldWidth, &player);
        cursorPosition(consoleHeight + 2, 1);
    }
    else if (UI == SHOOT) {
        int fieldHeight = (consoleHeight + 2 - fieldDisplayHeight) / 2 + 1;
        int fieldWidth = (consoleWidth + 2 - fieldDisplayWidth) / 3 + 1;

        cursorPosition(fieldHeight, fieldWidth);
        printField(renderField, fieldWidth);
        fieldBox(fieldHeight, fieldWidth, &player);

        cursorPosition(fieldHeight, 2 * fieldWidth);
        printField(renderField, 2 * fieldWidth);
        fieldBox(fieldHeight, 2 * fieldWidth, &player);

        cursorPosition(consoleHeight + 2, 1);
    }
    else if (UI == MULTI) {
        for (int i = 1; i <= gameSetup.playerCount; i++)
        {
            int fieldY = (consoleHeight + 2 - fieldDisplayHeight) / 2 + 1;
            int fieldX = ((consoleWidth + 2 - fieldDisplayWidth) * (i / (gameSetup.playerCount + 1.))) + 1;

            cursorPosition(fieldY, fieldX);
            printField(renderField, fieldX);
            fieldBox(fieldY, fieldX, &player);
        }
    }
}


void printField(std::vector<std::vector<char>> field, int fieldWidth) {
    // x-axis
    std::cout << "  ";
    for (int k = 0; k <= gameSetup.fieldSize; k++) {
        k < 26 && gameSetup.fieldSize > 26 ? std::cout << gameSetup.xAxisLabel[k] << "  " : std::cout << gameSetup.xAxisLabel[k] << " ";
    }
    std::cout << std::endl;
    cursorHorizontalAbsolute(fieldWidth);
    // y-axis and field
    for (int i = 0; i < gameSetup.fieldSize; i++) {
        if (gameSetup.yAxisLabel[i] < 10) std::cout << " ";
        std::cout << gameSetup.yAxisLabel[i] << " ";
        for (int j = 0; j < gameSetup.fieldSize; j++) {
            switch (field[i][j])
            {
            case '~':
                backgroundColor(21);
                break;
            case 'O':
                backgroundColor(255);
                break;
            case 'X':
                backgroundColor(196);
                break;
            case '*':
                backgroundColor(245);
                break;
            default:
                resetBackgroundColor();
                break;
            }
            std::cout << field[i][j] << (gameSetup.fieldSize > 26 ? "  " : " ");
            resetBackgroundColor();
        }
        cursorBack(fieldDisplayWidth * 2);
        cursorDown();
        cursorHorizontalAbsolute(fieldWidth);
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
    std::pair<bool, std::pair<int, int>> cursor = { true, {1, 1} };
    bool gameWon = false;
    while (!gameWon) {
        if (gameSetup.playerCount == 1) {
            render(players[1], Settings::SHOTS, SHOOTING, cursor);
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
                if (players[1].ships[col][row] == '*') {
                    players[1].shots[col][row] = 'X';
                    std::cout << "HIT\n";
                }
                else {
                    players[1].shots[col][row] = 'O';
                    std::cout << "MISS\n";
                }
            }
        }else{
            for (int i = 0; i < players.size(); i++) {
                bool hit = false;
                while (hit) {
                    render(players[i], Settings::SHOTS, SHOOTING, cursor);
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
    currentControlls(MENU);
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

//-------------------------------------CONSOLE-----------------------------------------------

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

void currentControlls(int UI) {

    // String operation add to string
    //      or
    // pre plan for each UI

    cursorPosition(consoleHeight - 2, 3);
    std::cout << "CONTROLLS:\n";
    cursorHorizontalAbsolute(3);
    switch (UI)
    {
    case 0: // MENU
        std::cout << "0-9 - choose option | ESC - go back";
        break;
    case 1: // NAME
        std::cout << "A-Z - write name | ESC - go back";
        break;
    case 2: // FIELDSIZE
        std::cout << "0-50 - choose option | ESC - go back";    // change max fieldsize by testing
        break;
    case 3: // SHIPCOUNT
        std::cout << "NUMBER - choose option | ESC - go back";    // change max fieldsize by testing
        break;
    case 4: // PLACING
        std::cout << "ARROWS - move arround | R - rotate | ENTER - place ship | ESC - go back";
        break;
    case 5: // SHOOTING
        std::cout << "ARROWS - move arround | ENTER - shoot | ESC - go back";
        break;
    default:
        break;
    }
}

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