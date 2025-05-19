#include "Game.h"
#include "console_utils.h"


Setup gameSetup;
int fieldDisplayWidth;
int fieldDisplayHeight;
int consoleWidth;
int consoleHeight;
int sleepTime = 1000;

Game::Game() : state(GameState::MENU) {
	menuStack.push(MenuID::MAIN);
    getConsoleSize(consoleWidth, consoleHeight);
    menuData = {
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
    menuActions = {
    {MenuID::MAIN, {
        {'1', [this]() {menuStack.push(MenuID::PRESETS); }},
        {'2', [this]() {/* TODO: Rangliste SQL?*/}},
        {'3', [this]() {gameSetup.displayRules(); }},
        {'4', [this]() {menuStack.push(MenuID::OPTIONS); }},
        {'0', [this]() {/*return*/}}}},
    {MenuID::PLAY, {
        {'1', [this]() {gameSetup.playerCount = 1; state = GameState::PLACE; }},
        {'2', [this]() {gameSetup.playerCount = 2; state = GameState::PLACE; }},
        {'3', [this]() {gameSetup.playerCount = 3; state = GameState::PLACE; }},
        {'4', [this]() {gameSetup.playerCount = 4; state = GameState::PLACE; }},
        {'0', [this]() {menuGoBack(); }}}},
    {MenuID::PRESETS, {
        {'1', [this]() {gameSetup.setPreset(Settings::SMALL); menuStack.push(MenuID::PLAY); gameSetup.genShips(); }},
        {'2', [this]() {gameSetup.setPreset(Settings::MEDIUM); menuStack.push(MenuID::PLAY); gameSetup.genShips(); }},
        {'3', [this]() {gameSetup.setPreset(Settings::LARGE); menuStack.push(MenuID::PLAY); gameSetup.genShips(); }},
        {'4', [this]() {menuStack.push(MenuID::PLAY); }},
        {'0', [this]() {menuGoBack(); }}}},
    {MenuID::OPTIONS, {
        {'1', [this]() {menuStack.push(MenuID::MODE); }},
        {'2', [this]() {
            currentControlls(Controlls::FIELDSIZE);
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
        {'3', [this]() {
            cursorPosition(consoleHeight * 0.6, consoleWidth * 0.25);
            if (gameSetup.shipSetting == Settings::TEN) {
                std::cout << "Current: many" << std::endl << std::endl;
            }
            else if (gameSetup.shipSetting == Settings::FIVE) {
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
            }
            else if (num == '2') {
              gameSetup.shipSetting = Settings::TEN;
            }
            else {
                cursorHorizontalAbsolute(consoleWidth * 0.25);
               std::cout << "Invalid Input.\n";
               Sleep(sleepTime);
               return;
            }
            gameSetup.shipCount = 0;
            gameSetup.genShips();
            }},
        {'4', [this]() {
            currentControlls(Controlls::SHIPCOUNT);
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
        {'5', [this]() {
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
            }
            else if (num == '2') {
               gameSetup.distance = Settings::AWAY;
            }
            else {
               std::cout << std::endl;
               cursorHorizontalAbsolute(consoleWidth * 0.25);
               std::cout << "Invalid Input.\n";
               Sleep(sleepTime);
              return;
            }
            }},
        {'6', [this]() {menuStack.push(MenuID::AI); }},
        {'0', [this]() {menuGoBack(); }}}},
    {MenuID::MODE, {
        {'1', [this]() {gameSetup.mode = Settings::NORMAL; menuGoBack(); }},
        {'2', [this]() {gameSetup.mode = Settings::SALVO; menuGoBack(); }},
        {'3', [this]() {gameSetup.mode = Settings::RACE; menuGoBack(); }},
        {'0', [this]() {menuGoBack(); }}}},
    {MenuID::AI, {
        {'1', [this]() {gameSetup.aiDifficulty = Settings::EASY; menuGoBack(); }},
        {'2', [this]() {gameSetup.aiDifficulty = Settings::NORMAL; menuGoBack(); }},
        {'3', [this]() {gameSetup.aiDifficulty = Settings::HARD; menuGoBack(); }},
        {'0', [this]() {menuGoBack(); }}}}
    };
}

void Game::run() {
    bool game = true;
	while (game) {
        switch (state) {
        case GameState::MENU:
            handleMenu();
            break;
        case GameState::PLACE:
            initGame();
            placingShips();
            state = GameState::GAME_LOOP;
            break;
        case GameState::GAME_LOOP:
            gameLoop();
            break;
        case GameState::GAME_OVER:
            gameOver();
            game = false;
            break;
        }
	}
}

void Game::initGame() {
    // clear screen
    eraseInDisplay(2);
    srand(time(0));
    // 
    fieldDisplayWidth = gameSetup.fieldSize * 2 + 9;
    fieldDisplayHeight = gameSetup.fieldSize + 1 + 4;
    gameSetup.genAxis();
    gameSetup.genShips();
    cursorPosition(consoleHeight * 0.45, consoleWidth * 0.25);
    for (int i = 0; i < gameSetup.playerCount; i++) {
        playerVect.push_back(i + 1);
        playerVect[i].askName();
        playerVect[i].genField();
        cursorPosition(consoleHeight * 0.65 + i * 2, consoleWidth * 0.25);
    }
    if (gameSetup.playerCount == 1) { playerVect.push_back(2); playerVect[1].name = "AI"; }
    // placing ships for AI
    if (gameSetup.playerCount == 1) {
        for (int lenght : gameSetup.lenghts) {
            for (int count = gameSetup.lenghtsCount[lenght - 2]; count > 0; count--) {
                bool placed = false;
                while (!placed) {
                    int direction = rand() % 2;
                    int col = rand() % (direction == 1 ? gameSetup.fieldSize - lenght + 1 : gameSetup.fieldSize);
                    int row = rand() % (direction == 0 ? gameSetup.fieldSize - lenght + 1 : gameSetup.fieldSize);
                    std::pair<int, int> cursor = { col, row };
                    if (playerVect[1].isValidPlacement(lenght, (direction ? Rotation::VER : Rotation::HOR), cursor)) {
                        playerVect[1].placeShip(lenght, (direction ? Rotation::VER : Rotation::HOR), cursor);
                        placed = true;
                    }
                }
            }
        }
    }
}

void Game::placingShips() {
    for (int i = 0; i < playerVect.size(); i++) {
        if (gameSetup.playerCount == 1 && i == 1) break;
        titleBox();
        cursorPosition(consoleHeight * 0.3, consoleWidth * 0.2);
        playerVect[i].placing();
    }
}

void Game::gameLoop() {
    std::pair<int, int> cursor = { 1, 1 };
    bool gameWon = false;
    while (!gameWon) {
        //singleplayer vs AI
        if (gameSetup.playerCount == 1) {
            // Human turn
            bool shotFired = false;
            while (!shotFired) {
                bool hit = false;
                do {
                    renderShoot(playerVect[1], playerVect[0], cursor);
                    int input = _getch();
                    if (input == 224) input = _getch(); // Handle arrow key prefix
                    if (input == static_cast<int>(Input::UP) && cursor.first > 0) {
                        cursor.first--;
                    }
                    else if (input == static_cast<int>(Input::DOWN) && cursor.first < gameSetup.fieldSize - 1) {
                        cursor.first++;
                    }
                    else if (input == static_cast<int>(Input::LEFT) && cursor.second > 0) {
                        cursor.second--;
                    }
                    else if (input == static_cast<int>(Input::RIGHT) && cursor.second < gameSetup.fieldSize - 1) {
                        cursor.second++;
                    }
                    else if (input == static_cast<int>(Input::ENTER)) {
                        cursorPosition(consoleHeight * 0.5, consoleWidth * 0.05);
                        if (playerVect[0].shots[cursor.first][cursor.second] == 'X' || playerVect[0].shots[cursor.first][cursor.second] == 'O') {
                            std::cout << "Invalid shot! Try again.\n";
                            Sleep(sleepTime);
                            continue;
                        }
                        hit = playerVect[0].shoot(playerVect[1], cursor);
                        shotFired = true;
                        if (allShipsSunken(playerVect[1])) {
                            gameWon = true;
                            titleBox();
                            cursorPosition(consoleHeight * 0.5, consoleWidth * 0.5 - 5);
                            std::cout << playerVect[0].name << " wins!\n";
                            state = GameState::GAME_OVER;
                        }
                    }
                } while (hit);
            }
            // AI turn
            if (!gameWon) {
                bool hit = false;
                do {
                    std::pair<int, int> aiPos = getAIPosition(playerVect[1]);
                    if (aiPos.first != -1) {
                        hit = playerVect[1].aiShoot(playerVect[0], aiPos);
                        if (allShipsSunken(playerVect[0])) {
                            gameWon = true;
                            titleBox();
                            cursorPosition(consoleHeight * 0.5, consoleWidth * 0.5 - 5);
                            std::cout << "AI wins!\n";
                            state = GameState::GAME_OVER;
                        }
                    }
                } while (hit);
            }
        }
        // 1 vs 1
        else if (gameSetup.playerCount == 2) {
            // Multi-player (2 or more players)
            for (int i = 0; i < playerVect.size() && !gameWon; i++) {
                bool shotFired = false;
                while (!shotFired) {
                    bool hit = false;
                    do {
                        // Choose target: next player in sequence (wrap around)
                        int targetIndex = (i + 1) % playerVect.size();
                        renderShoot(playerVect[targetIndex], playerVect[i], cursor);
                        int input = _getch();
                        if (input == 224) input = _getch();
                        if (input == static_cast<int>(Input::UP) && cursor.first > 0) {
                            cursor.first--;
                        }
                        else if (input == static_cast<int>(Input::DOWN) && cursor.first < gameSetup.fieldSize - 1) {
                            cursor.first++;
                        }
                        else if (input == static_cast<int>(Input::LEFT) && cursor.second > 0) {
                            cursor.second--;
                        }
                        else if (input == static_cast<int>(Input::RIGHT) && cursor.second < gameSetup.fieldSize - 1) {
                            cursor.second++;
                        }
                        else if (input == static_cast<int>(Input::ENTER)) {
                            cursorPosition(consoleHeight * 0.5, consoleWidth * 0.05);
                            if (playerVect[i].shots[cursor.first][cursor.second] == 'X' || playerVect[i].shots[cursor.first][cursor.second] == 'O') {
                                std::cout << "Invalid shot! Try again.\n";
                                Sleep(sleepTime);
                                continue;
                            }
                            hit = playerVect[i].shoot(playerVect[targetIndex], cursor);
                            shotFired = true;
                            if (allShipsSunken(playerVect[targetIndex])) {
                                gameWon = true;
                                titleBox();
                                cursorPosition(consoleHeight * 0.5, consoleWidth * 0.5 - 5);
                                std::cout << playerVect[i].name << " wins!\n";
                                state = GameState::GAME_OVER;
                            }
                        }
                    } while (hit);
                }
            }
        }
    }
}

void Game::handleMenu() {
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
    }
    else {
        cursorPosition(consoleHeight * 0.7, consoleWidth * 0.25);
        std::cout << "Wrong Input" << std::endl;
        Sleep(sleepTime);
    }
}

void Game::gameOver() {
    _getch();
}

bool Game::allShipsSunken(const Player& player) {
    for (const auto& ship : player.allShips) {
        if (!ship.sunken) return false;
    }
    return true;
}

std::pair<int, int> Game::getAIPosition(const Player& ai) {
    std::vector<std::pair<int, int>> available;
    for (int i = 0; i < gameSetup.fieldSize; i++) {
        for (int j = 0; j < gameSetup.fieldSize; j++) {
            if (ai.shots[i][j] == ' ') {
                available.emplace_back(i, j);
            }
        }
    }
    if (available.empty()) return { -1, -1 };
    int index = rand() % available.size();
    return available[index];
}

void Game::menuGoBack() {
    if (menuStack.size() > 1) 
        menuStack.pop();
    else
        std::cout << "Fehler" << std::endl;
}

void Game::setState(GameState newState) {
    state = newState;
}
