#include <iostream>
#include <string>
#include <vector>
#include <conio.h> // for _getch
#include <map>
#include <stack>
#include <functional>
#include <limits> // for numeric_limits cin
//#include <sqlite3.h>
#include <optional> // optional parameter or return value
#include <utility> // Für std::pair

using namespace std;

/*
- zwei windows für jeden eins
- leaderboard kann auf zb sql oder exel oder sheets datei zugreifen und diese umschreiben bzw lesen und dieses dann ausgeben
- Salvo mode 5 shots at once
- uboot -> kann mit anderem schiff überlappen oder kann nach treffer
- unterwasser gehen oder position ändern Leaderboard KI Schwierigkeiten
X Feldgrößen - Schiffs anzahl
- Mehrspieler zB 2-4
- Schiffe automatisch plazieren
X Regeln ändern (Platz zwischen Schiffen)
- Unterschiedliche Schiffs größen  2345(6789)->wenn map größer als 10
- für jedes schiff eine fähigkeit
- (Abilities wie "Sonar"->deckt fläche auf, Unterwasser Minen -> sagen dass
- gehitet wurde als ablenkung, torpedo -> schießt in einer linie) Anzahl an
X schiffen berechnung -> (size*size)*0.17 -> anzahl an schiff feldern -> ans -
X 2(3,3,4,5)   -> periodisch
*/

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
    FAR, CLOSE,             // dist
    FIVE = 17, TEN = 30,    // ship
    UP = 72, DOWN = 80,     // arrow
    LEFT = 75, RIGHT = 77   // arrow
};

//---------------------------------------Struct------------------------------------------------

//--------------------------------------KLASSEN----------------------------------------------

// . nichts  X kein Treffer  O Treffer  #  --- ||| versenkt



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
    vector<int> shipsLenght;//  2*2     2,3*2,4,5   2*4,3*4,4*2,5*2
    Settings aiDifficulty;  //  easy    normal      hard
    vector<string> xAxisLabel;
    vector<int> yAxisLabel;

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
            string label = "";
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
        cout << "Mode: ";
        switch (mode) {
        case Settings::STANDARD: cout << " Classic\n"; break;
        case Settings::SALVO: cout << " Five shots\n"; break;
        case Settings::RACE: cout << " Race to the end\n"; break;
        }
        cout << "Distance: ";
        switch (distance) {
        case Settings::FAR: cout << "One apart\n"; break;
        case Settings::CLOSE: cout << "Next to another\n"; break;
        }
        int num;
        (shipCount) ? num = shipCount : num = shipsLenght.size() ;
        cout << "Num of ships: " << num << endl;
        cout << "Everone has:\n";
        for (int i = 2; i <= 5; i++) {
            int amount = count(shipsLenght.begin(), shipsLenght.end(), i);
            cout << amount << "x" << i << "er ships" << endl;
        }
        if (shipCount == 0) {
            cout << "Ship amount: ";
            switch (shipSetting) {
            case Settings::FIVE: cout << "low\n"; break;
            case Settings::TEN: cout << "high\n"; break;
            }
        }
        cout << "Fieldsize: " << fieldSize << "x" << fieldSize << endl;
        cout << "AI diff: ";
        switch (aiDifficulty) {
        case Settings::EASY: cout << "easy\n"; break;
        case Settings::NORMAL: cout << "normal\n"; break;
        case Settings::HARD: cout << "hard\n"; break;
        }
    }
};

Setup gameSetup;

class Player {
public:
    int number;         // player number
    string name;        // player name
    int score = 0;      // session score calc -> win, ships sunken points
    vector<vector<char>> field; // own player field // Das Spielfeld des Spielers.  Hier ist ein 'char' sinnvoll, um verschiedene Zustände des Feldes darzustellen (z.B. '.', 'O', 'X', '#').
    int shotsFired = 0;     // shot count // Dies hilft, die Effizienz des Spielers zu bewerten (Verhältnis von Treffern zu Schüssen).

    // create the player with a number
    Player(int number) : number(number) {}
    // prints the field of the current player
    void displayField() {
        // x-axis
        for (int k = 0; k < gameSetup.fieldSize; k++) {
            if (!k) cout << "   ";
            k < 26 && gameSetup.fieldSize > 26 ? cout << gameSetup.xAxisLabel[k] << "  " : cout << gameSetup.xAxisLabel[k] << " ";
        }
        cout << endl;
        // y-axis
        for (int i = 0; i < gameSetup.fieldSize; i++) {
            if (i + 1 < 10) cout << " ";
            cout << gameSetup.yAxisLabel[i] << " ";
            for (int j = 0; j < gameSetup.fieldSize; j++) {
                gameSetup.fieldSize > 26 ? cout << field[i][j] << "  " : cout << field[i][j] << " ";
            }
            cout << endl;
        }
    }
    //
    void shoot() {}
    //
    void placeShips() {
        int input = _getch();
        if (input == static_cast<int> (Settings::UP)) {
            cout << "UP";
        }
        else if (input == static_cast<int> (Settings::DOWN)) {
            cout << "DOWN";
        }
        else if (input == static_cast<int> (Settings::LEFT)) {
            cout << "LEFT";
        }
        else if (input == static_cast<int> (Settings::RIGHT)) {
            cout << "RIGHT";
        }
    }
    // Registers the player and saves the name
    void askName() {
        //clearConsole();
        cout << "Player " << number << " whats your name?\n";
        cin >> name;
        cout << "Hello " << name << " welcome to Battleships!\n";
    }
    // score considers win and sunken ships and hits (and total shots)
    void /*update?*/scoring() {} // Diese Funktion sollte aufgerufen werden, wenn ein Spieler ein Schiff versenkt oder das Spiel gewinnt.

    class Ships {
        int size;   // ship size
        int x;      // position upper left
        int y;      // position upper left
        int dir;    // 0 horizontal  1 vertikal
        bool isSunk = false;    // 0 no 1 yes 
        // Füge ein, um den Zustand des Schiffs zu verfolgen.  Dies ist sehr
        // nützlich, um später im Spiel festzustellen, wann ein Spieler
        // alle Schiffe versenkt hat und das Spiel beendet werden kann.
        // Man könnte auch noch ein Array oder einen Vektor von bool-Werten hinzufügen, um zu
        // verfolgen, welche Teile des Schiffs getroffen wurden.
    };
};



//--------------------------------------GLOBAL--------------------------------------------

GameState state = GameState::MENU; 
stack<MenuID> menuStack;

//-----------------------------------Deklaration--------------------------------------

void displayMenu(const vector<pair<char, string>>& options);
MenuID goback(stack<MenuID>& stack);
void clearConsole();
void genFields(vector<Player>& players);
void initGame(vector<Player>& players);

void placeShips(vector<Player>& players);
void select(Player& player, pair<int, int> cursor, int size);

//-------------------------------MAPS--------------------------------------------

// data map for the menu text
map<MenuID, vector<pair<char, string>>> menuData = {
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
map<MenuID, map<char, function<void()>>> menuActions = {
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
            clearConsole();
            cout << "Current: " << gameSetup.fieldSize << endl << endl;
            cout << "How big should the field be?\n";
            int size;
            cin >> size;
            if (cin.fail()) { //Fehlerbehandlung
                cout << "Invalid Input. Please input a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            gameSetup.fieldSize = size;
        }},
        {'3', []() {
            clearConsole();
            if (gameSetup.shipSetting == Settings::TEN){
                cout << "Current: many\n\n";
            }else if (gameSetup.shipSetting == Settings::FIVE){
                cout << "Current: few\n\n";
            }
            cout << "How many ships?\n\t1. few\n\t2. many\n";
            int num = _getch();
            if (0 < num < 3) { num == 1 ? gameSetup.shipSetting = Settings::FIVE : gameSetup.shipSetting = Settings::TEN; };
            if (num == '1') {
                gameSetup.shipSetting = Settings::FIVE;
            }else if (num == '2') {
               gameSetup.shipSetting = Settings::TEN;
            }else {
               cout << "Invalid Input.\n";
               return;
            }
            gameSetup.shipCount = 0;
            gameSetup.genShips();
        }},
        {'4', []() {
            clearConsole();
            cout << "Current: " << gameSetup.shipCount << endl << endl;
            cout << "How many ships?\n(This setting overwrites the automatic number!)\n";
            int num;
            cin >> num;
            if (cin.fail()) { //Fehlerbehandlung
                cout << "Invalid Input. Please input a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            gameSetup.shipCount = num;
            gameSetup.genShips();
        }},
        {'5', []() {
            clearConsole();
            if (gameSetup.shipSetting == Settings::CLOSE) {
                cout << "Current: close\n\n";
            }
            else if (gameSetup.shipSetting == Settings::FAR) {
                cout << "Current: far\n\n";
            }
            cout << "What distance should the ships be?\n\t1. close\n\t2. far\n";
            int num = _getch();
            if (num == '1') {
                gameSetup.distance = Settings::CLOSE;
            }else if (num == '2') {
           gameSetup.distance = Settings::FAR;
            }else {
               cout << "Invalid Input.\n";
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
    // starting with main menu
    menuStack.push(MenuID::MAIN);
    // saving all players
    vector<Player> players;
    
    while (true) {
        if (state == GameState::MENU) {
            MenuID currentMenuID = menuStack.top();
            vector<pair<char, string>> currentMenu = menuData[currentMenuID];
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
                cout << "Wrong Input" << endl;
                continue;
            }
        }else if (state == GameState::GAME_LOOP){
            initGame(players);
            placeShips(players);
        }else if (state == GameState::GAME_OVER){

        }
    }
    return 0;
}

//---------------------------------------GAME--------------------------------------------------

void initGame(vector<Player>& players) {
    gameSetup.genAxis();
    gameSetup.genShips();
    for (int i = 0; i < gameSetup.playerCount; i++) {
        players.emplace_back(i + 1);
        players[i].askName();
    }
    genFields(players);

    //TODO: Schiffe platzieren.  Die Schiffe müssen jetzt platziert werden, entweder
    // automatisch oder durch den Spieler.  Dies ist ein komplexer Teil des Spiels,
    // der die Interaktion mit dem Spieler und die Überprüfung der Platzierung
    // beinhaltet.
}
//
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
                        }else if (input == 13/*ENTER*/) {
                            selected = 1;
                            cout << "Choose a direction [h] = horizontal [v] = vertical\n";
                            while (!placed){
                                int dir = _getch();
                                if (dir == 118/*v*/ || dir == 86/*V*/){
                                    cout << "Placed a " << num << " long ship verticaly\n";
                                    placed = 1;
                                }else if (dir == 104/*h*/ || dir == 72/*H*/) {
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

void select(Player& player, pair<int, int> cursor, int size) {
    vector<vector<char>> render;
    render.clear();
    render = player.field;
    clearConsole();
    cout << "Placing a " << size << " long ship\n";
    // displaying cursor
    for (int i = 0; i < gameSetup.fieldSize; i++) {
        for (int j = 0; j < gameSetup.fieldSize; j++) {
            if (i == cursor.first && j == cursor.second) {
                render[i][j] = '#';
            }
        }
    }
    // x-axis
    for (int k = 0; k < gameSetup.fieldSize; k++) {
        if (!k) cout << "   ";
        k < 26 && gameSetup.fieldSize > 26 ? cout << gameSetup.xAxisLabel[k] << "  " : cout << gameSetup.xAxisLabel[k] << " ";
    }
    cout << endl;
    // y-axis
    for (int i = 0; i < gameSetup.fieldSize; i++) {
        if (i + 1 < 10) cout << " ";
        cout << gameSetup.yAxisLabel[i] << " ";
        for (int j = 0; j < gameSetup.fieldSize; j++) gameSetup.fieldSize > 26 ? cout << render[i][j] << "  " : cout << render[i][j] << " ";
        cout << endl;
    }
}
/*
void render(optional<pair<int, int>> cursor, Player& player) {

}*/

// generates the field for every player 
void genFields(vector<Player>& players) {
    for (Player& player : players) {
        player.field.clear();
        player.field.resize(gameSetup.fieldSize);
        for (int i = 0; i < gameSetup.fieldSize; ++i) {
            player.field[i].resize(gameSetup.fieldSize, '.');   // inserting . as place holder
        }
    }
}

// game function
void game() {
}

// 
void gameLoop() {
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
void displayMenu(const vector<pair<char, string>>& options) {
    clearConsole();
    cout << "\n\n\n\n";
    for (auto& pair : options) {
        cout << "\t" << pair.first << ". " << pair.second << endl;
    }
    cout << endl << endl;
}

// go one menu back (pop top menu from stack)
MenuID goback(stack<MenuID>& stack) {
    if (stack.size() > 1) {
        stack.pop();
        return stack.top();
    }else {
        cout << "Fehler" << endl;
        return stack.top();
    }
}

// clears the console with a command
void clearConsole() {
    #ifdef _WIN32
        system("cls");
        //for (int i = 0; i < 30; i++) {
        //    cout << endl;
        //}
    #else
        system("clear");
    #endif
}

//------------------------------------------------------------------------------------