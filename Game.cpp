#include "Game.h"
#include "console_utils.h"

Game::Game() : sleepTime(1000), state(GameState::MENU) {
	menuStack.push(MenuID::MAIN);
    console_utils::getConsoleSize(consoleWidth, consoleHeight);
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
        {'1', [this]() {gameSetup.playerCount = 1; state = GameState::GAME_LOOP; }},
        {'2', [this]() {gameSetup.playerCount = 2; state = GameState::GAME_LOOP; }},
        {'3', [this]() {gameSetup.playerCount = 3; state = GameState::GAME_LOOP; }},
        {'4', [this]() {gameSetup.playerCount = 4; state = GameState::GAME_LOOP; }},
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
            console_utils::cursorPosition(consoleHeight * 0.6, consoleWidth * 0.25);
            std::cout << "Current: " << gameSetup.fieldSize << std::endl << std::endl;
            console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "How big should the field be?" << std::endl;
            console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
            console_utils::showCursor();
            int size;
            std::cin >> size;
            if (std::cin.fail()) { //Fehlerbehandlung
                console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
                std::cout << "Invalid Input. Please input a number.";
                Sleep(sleepTime);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                console_utils::hideCursor();
                return;
            }
            console_utils::hideCursor();
            gameSetup.fieldSize = size;
        }},
        {'3', [this]() {
            console_utils::cursorPosition(consoleHeight * 0.6, consoleWidth * 0.25);
            if (gameSetup.shipSetting == Settings::TEN) {
                std::cout << "Current: many" << std::endl << std::endl;
            }
            else if (gameSetup.shipSetting == Settings::FIVE) {
               std::cout << "Current: few" << std::endl << std::endl;
            }
            console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "How many ships?\n\n";
            console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "[1]  few\n";
            console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
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
                console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
               std::cout << "Invalid Input.\n";
               Sleep(sleepTime);
               return;
            }
            gameSetup.shipCount = 0;
            gameSetup.genShips();
            }},
        {'4', [this]() {
            currentControlls(Controlls::SHIPCOUNT);
            console_utils::cursorPosition(consoleHeight * 0.6, consoleWidth * 0.25);
            std::cout << "Current: " << gameSetup.shipCount << std::endl << std::endl;
            console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "How many ships?\t(This setting overwrites the automatic number!)\n";
            console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
            console_utils::showCursor();
            int num;
            std::cin >> num;
            if (std::cin.fail()) { //Fehlerbehandlung
                std::cout << "Invalid Input. Please input a number.\n";
                Sleep(sleepTime);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                console_utils::hideCursor();
                return;
            }
            console_utils::hideCursor();
            gameSetup.shipCount = num;
            gameSetup.genShips();
        }},
        {'5', [this]() {
            console_utils::cursorPosition(consoleHeight * 0.6, consoleWidth * 0.25);
            if (gameSetup.shipSetting == Settings::CLOSE) {
                std::cout << "Current: close\n\n";
            }
            else if (gameSetup.shipSetting == Settings::AWAY) {
                std::cout << "Current: far\n\n";
            }
            console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "What distance should the ships be?\n";
            console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
            std::cout << "[1]  close\n";
            console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
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
               console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
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
	while (true) {
        switch (state) {
        case GameState::MENU:
            handleMenu();
            break;
        case GameState::PLACE:
            placingShips();
            state = GameState::GAME_LOOP;
            break;
        case GameState::GAME_LOOP:
            gameLoop();
            break;
        case GameState::GAME_OVER:
            gameOver();
            break;
        }
	}
}

void Game::initGame() {
    // clear screen
    console_utils::eraseInDisplay(2);

    // 
    fieldDisplayWidth = gameSetup.fieldSize * 2 + 9;
    fieldDisplayHeight = gameSetup.fieldSize + 1 + 4;
    gameSetup.genAxis();
    gameSetup.genShips();
    console_utils::cursorPosition(consoleHeight * 0.45, consoleWidth * 0.25);
    for (int i = 0; i < gameSetup.playerCount; i++) {
        playerVect.push_back(i + 1);
        playerVect[i].askName();
        playerVect[i].genField();
        console_utils::cursorPosition(consoleHeight * 0.65 + i * 2, consoleWidth * 0.25);
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
                    if (playerVect[1].isValidPlacement(lenght, (direction ? Rotation::VER : Rotation::HOR), cursor))) {
                        playerVect[1].placeShip(length, direction, cursor);
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
        playerVect[i].placing();
    }
}

void Game::gameLoop() {
    std::pair<int, int> cursor = {1, 1};
    bool gameWon = false;
    while (!gameWon) {
        if (gameSetup.playerCount == 1) {
            renderShoot(playerVect[1], playerVect[0], cursor);
            int col = cursor.first;  // vertical
            int row = cursor.second; // horizontal
            int input = _getch();
            if (input == 224) input = _getch();
            if (input == static_cast<int> (Input::UP) && col > 0) {
                cursor.first--;
            }
            else if (input == static_cast<int> (Input::DOWN) && col < gameSetup.fieldSize - 1) {
                cursor.first++;
            }
            else if (input == static_cast<int> (Input::LEFT) && row > 0) {
                cursor.second--;
            }
            else if (input == static_cast<int> (Input::RIGHT) && row < gameSetup.fieldSize - 1) {
                cursor.second++;
            }
            else if (input == static_cast<int> (Input::ENTER)) {
                if (playerVect[1].gotShot(cursor)) {

                }
            }
            if (!gameWon) {
                std::pair<int, int> aiPos = getAIPosition(playerVect[1]);
                if (aiPos.first != -1) {
                    bool hit = playerVect[0].gotShot(aiPos);
                    playerVect[1].shots[aiPos.first][aiPos.second] = hit ? 'X' : 'O';
                    if (allShipsSunken(playerVect[0])) {
                        gameWon = true;
                        std::cout << "AI wins!\n";
                    }
                }
            }
        }
        else {
            for (int i = 0; i < playerVect.size(); i++) {
                bool hit = false;
                while (hit) {
                    renderShoot(playerVect[i], cursor);
                    int col = cursor.first;  // vertical
                    int row = cursor.second; // horizontal
                    int input = _getch();
                    if (input == static_cast<int> (Input::UP) && col > 0) {
                        cursor.first--;
                    }
                    else if (input == static_cast<int> (Input::DOWN) && col < gameSetup.fieldSize - 1) {
                        cursor.first++;
                    }
                    else if (input == static_cast<int> (Input::LEFT) && row > 0) {
                        cursor.second--;
                    }
                    else if (input == static_cast<int> (Input::RIGHT) && row < gameSetup.fieldSize - 1) {
                        cursor.second++;
                    }
                    else if (input == static_cast<int> (Input::ENTER)) {
                        bool valid = false;
                        if (playerVect[i].ships[col][row] == '*') {
                            playerVect[i].shots[col][row] = 'X';
                            std::cout << "HIT\n";
                        }
                        else {
                            playerVect[i].shots[col][row] = 'O';
                            std::cout << "MISS\n";
                        }
                    }
                }

            }
        }
    }
}

void Game::handleMenu() {
    MenuID currentMenuID = menuStack.top();
    std::vector<std::pair<char, std::string>> currentMenu = menuData[currentMenuID];
    gameSetup.genShips();
    UI::displayMenu(currentMenu);

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
        console_utils::cursorPosition(consoleHeight * 0.7, consoleWidth * 0.25);
        std::cout << "Wrong Input" << std::endl;
        Sleep(sleepTime);
    }
}

void Game::gameOver() {

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
    if (menuStack.size() > 1) {
        menuStack.pop();
    }
    else {
        std::cout << "Fehler" << std::endl;
    }
}

void Game::setState(GameState newState) {
    state = newState;
}