#include <iostream>
#include <string>
#include <vector>
#include <conio.h> // for _getch
#include <map>
#include <stack>
#include <functional>
#include <limits> // for numeric_limits cin

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

enum class Settings{
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

class Ships {
    //int player; // player number
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

class Setup {
private:
    const int lengthShips[5] = { 5,4,3,3,2 };
public:
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
        shipCount(5),
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
        }
        // filling in the y-axis with numbers
        for (int i = 1; i <= fieldSize; i++) {
            yAxisLabel.push_back(i);
        }
    }
    // generates num and lenght of ships
    void genShips() {
        shipsLenght.clear();
        if (!shipCount) {
            int shipFields = fieldSize * fieldSize * (static_cast<int>(shipSetting) / 100.);
            while (shipFields >= 2) {
                for (int num : lengthShips) {
                    if (num > shipFields) break;
                    shipFields -= num;
                    shipsLenght.push_back(num);
                }
            }
        }
        else {
            int temp = shipCount;
            while (temp) {
                for (int num : lengthShips) {
                    if (temp) break;
                    shipCount--;
                    shipsLenght.push_back(num);
                }
            }
        }
    }
    // displays the current rules
    const void displayRules() {
        switch (mode) {
        case Settings::STANDARD: cout << "Klassisches Schiffe versenken.\n"; break;
        case Settings::SALVO: cout << "Funf Schusse gleichzeitig.\n"; break;
        case Settings::RACE: cout << "Rennen wer als erstes zum Ende kommt.\n"; break;
        }
        switch (distance) {
        case Settings::FAR: cout << "Schiffe mussen ein Feld auseinander platziert werden.\n"; break;
        case Settings::CLOSE: cout << "Schiffe konnen direkt nebeneinander platziert werden\n"; break;
        }
        if (shipCount) {
            cout << "Die Anzahl der Schiffe betragt " << shipCount << "." << endl;
            int temp = 0;
            cout << "Jeder hat:\n";
            for (int num : lengthShips) {
                if (temp == num) continue;
                temp = num;
                int amount = count(shipsLenght.begin(), shipsLenght.end(), num);
                cout << amount << "x" << num << "er Schiffe" << endl;
            }
        }
        else {
            switch (shipSetting) {
            case Settings::FIVE: cout << "Die Anzahl der Schiffe ist gering.\n"; break;
            case Settings::TEN: cout << "Die Anzahl der Schiffe ist hoch.\n"; break;
            }
        }

        cout << "Die Feld grosse betragt " << fieldSize << "x" << fieldSize << endl;
        switch (aiDifficulty) {
        case Settings::EASY: cout << "Die KI ist auf Einfach eingestellt.\n"; break;
        case Settings::NORMAL: cout << "Die KI ist auf Normal eingestellt.\n"; break;
        case Settings::HARD: cout << "Die KI ist auf Schwer eingestellt.\n"; break;
        }
    }
};

class Player {
public:
    int number;         // player number
    string name;        // player name
    int score = 0;      // session score calc -> win, ships sunken points
    vector<vector<char>> field; // own player field // Das Spielfeld des Spielers.  Hier ist ein 'char' sinnvoll, um verschiedene Zustände des Feldes darzustellen (z.B. '.', 'O', 'X', '#').
    int shotsFired = 0;     // shot count // Dies hilft, die Effizienz des Spielers zu bewerten (Verhältnis von Treffern zu Schüssen).

    // 
    Player(int number) : number(number) {}
    // prints the field of the current player
    void displayField(const Setup& settings) {
        // x-axis
        for (int k = 0; k < settings.fieldSize; k++) {
            if (!k) cout << "   ";
            k < 26 && settings.fieldSize > 26 ? cout << settings.xAxisLabel[k] << "  " : cout << settings.xAxisLabel[k] << " ";
        }
        cout << endl;
        // y-axis
        for (int i = 0; i < settings.fieldSize; i++) {
            if (i + 1 < 10) cout << " ";
            cout << settings.yAxisLabel[i] << " ";
            for (int j = 0; j < settings.fieldSize; j++) {
                settings.fieldSize > 26 ? cout << field[i][j] << "  " : cout << field[i][j] << " ";
            }
            cout << endl;
        }
    }
    //
    void shoot(){}
    //
    void placeShips(){
        int input = _getch();
        if (input == static_cast<int> (Settings::UP)) {
        }
        else if (input == static_cast<int> (Settings::DOWN)){

        }
        else if (input == static_cast<int> (Settings::LEFT)){

        }
        else if (input == static_cast<int> (Settings::RIGHT)){

        }
    }
    // Registers the player and saves the name
    void askName(){
        //clearConsole();
        cout << "Player " << number << " whats your name?\n";
        cin >> name;
        cout << "Hello " << name << " welcome to Battleships!\n";
    }
    // score considers win and sunken ships and hits (and total shots)
    void /*update?*/scoring() {} // Diese Funktion sollte aufgerufen werden, wenn ein Spieler ein Schiff versenkt oder das Spiel gewinnt.
};



//--------------------------------------GLOBAL--------------------------------------------

Setup gameSetup;
stack<MenuID> menuStack;

//-----------------------------------Deklaration--------------------------------------

void displayMenu(const vector<pair<char, string>>& options);
MenuID goback(stack<MenuID>& stack);
void clearConsole();
void genFields(vector<Player>& players);

//-------------------------------MAPS--------------------------------------------

// data map for the menu text
map<MenuID, vector<pair<char, string>>> menuData = {
    {MenuID::MAIN, {
        {'1', "Spielen"},
        {'2', "Rangliste"},
        {'3', "Regeln"},
        {'4', "Optionen"},
        {'0', "Beenden"}}},
    {MenuID::PLAY, {
        {'1', "KI"},
        {'2', "2 Spieler"},
        {'3', "3 Spieler"},
        {'4', "4 Spieler"},
        {'0', "Zuruck"}}},
    {MenuID::PRESETS, {
        {'1', "Klein"},
        {'2', "Mittel"},
        {'3', "Gross"},
        {'4', "Benutzerdefiniert"},
        {'0', "Zuruck"}}},
    {MenuID::OPTIONS, {
        {'1', "Modus"},
        {'2', "Feldgrosse"},
        {'3', "Schiffs Menge (automatisch)"},
        {'4', "Schiffs Anzahl (manuell)"},
        {'5', "Distanz"},
        {'6', "KI Schwierigkeit"},
        {'0', "Zuruck"}}},
    {MenuID::MODE, {
        {'1', "Normal"},
        {'2', "Salvo"},
        {'3', "Race"},
        {'0', "Zuruck"}}},
    {MenuID::AI, {
        {'1', "Einfach"},
        {'2', "Normal"},
        {'3', "Schwer"},
        {'0', "Zuruck"}}}
};

// map for the coresponding actions for the choices in menus 
map<MenuID, map<char, function<void()>>> menuActions = {
    {MenuID::MAIN, {
        {'1', []() {menuStack.push(MenuID::PRESETS);}},
        {'2', []() {/* TODO: Rangliste SQL?*/}},
        {'3', []() {gameSetup.displayRules(); }},
        {'4', []() {menuStack.push(MenuID::OPTIONS);}},
        {'0', []() {/*return*/}}}},
    {MenuID::PLAY, {
        {'1', []() {gameSetup.playerCount = 1; /* TODO: Spiel starten */}},
        {'2', []() {gameSetup.playerCount = 2; /* TODO: Spiel starten */}},
        {'3', []() {gameSetup.playerCount = 3; /* TODO: Spiel starten */}},
        {'4', []() {gameSetup.playerCount = 4; /* TODO: Spiel starten */}},
        {'0', []() {goback(menuStack); }}}},
    {MenuID::PRESETS, {
        {'1', []() {gameSetup.setPreset(Settings::SMALL); menuStack.push(MenuID::PLAY); }},
        {'2', []() {gameSetup.setPreset(Settings::MEDIUM); menuStack.push(MenuID::PLAY); }},
        {'3', []() {gameSetup.setPreset(Settings::LARGE); menuStack.push(MenuID::PLAY); }},
        {'4', []() {menuStack.push(MenuID::PLAY);}},
        {'0', []() {goback(menuStack);}}}},
    {MenuID::OPTIONS, {
        {'1', []() {menuStack.push(MenuID::MODE); }},
        {'2', []() {
            clearConsole(); 
            cout << "Welche Seitenlange soll das Feld haben?\n";  
            int size; 
            cin >> size; 
            if (cin.fail()) { //Fehlerbehandlung
                cout << "Ungültige Eingabe. Bitte eine Zahl eingeben.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            gameSetup.fieldSize = size;
        }},
        {'3', []() {
            clearConsole(); 
            cout << "Wie viele Schiffe sollen es sein?\n\t1. wenige\n\t2. viele\n"; 
            int num = _getch(); 
            if (0 < num < 3) { num == 1 ? gameSetup.shipSetting = Settings::FIVE : gameSetup.shipSetting = Settings::TEN; };
            if (num == '1') {
                gameSetup.shipSetting = Settings::FIVE;
            }else if (num == '2') {
                gameSetup.shipSetting = Settings::TEN;
            }else {
                cout << "Ungültige Eingabe.\n";
                return;
            }
            gameSetup.shipCount = 0;
        }},
        {'4', []() {
            clearConsole(); 
            cout << "Wie viele Schiffe willst du haben?\n(Diese Einstellung uberscheibt die automatische Anzahl!)\n";  
            int num; 
            cin >> num;
            if (cin.fail()) { //Fehlerbehandlung
                cout << "Ungültige Eingabe. Bitte eine Zahl eingeben.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            gameSetup.shipCount = num;
        }},
        {'5', []() {
            clearConsole(); 
            cout << "Welche distanz sollen die Schiffe haben?\n\t1. nah\n\t2. fern\n"; 
            int num = _getch(); 
            if (num == '1') {
                gameSetup.distance = Settings::CLOSE;
            }else if (num == '2') {
                gameSetup.distance = Settings::FAR;
            }else {
                cout << "Ungültige Eingabe.\n";
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
        MenuID currentMenuID = menuStack.top();
        vector<pair<char, string>> currentMenu = menuData[currentMenuID];

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
            cout << "Falsche Eingabe" << endl;
            //cin.clear();
            //cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (currentMenuID == MenuID:: PLAY){

        }
    }
    return 0;
}

//---------------------------------------GAME--------------------------------------------------

void initGame(vector<Player>& players) {
    gameSetup.genAxis();
    gameSetup.genShips();
    for (int i = 0; i < gameSetup.playerCount; i++){
        players.emplace_back(i + 1);
        players[i].askName();
    }
    //TODO: Schiffe platzieren.  Die Schiffe müssen jetzt platziert werden, entweder
    // automatisch oder durch den Spieler.  Dies ist ein komplexer Teil des Spiels,
    // der die Interaktion mit dem Spieler und die Überprüfung der Platzierung
    // beinhaltet.
}

// generates the field for every player 
/*
void genFields(const vector<Player>& players) {
    for (Player &player: players){

    }
}
*/

void genFields1(const Setup& settings, Player& player) {
    int fieldSize = settings.fieldSize;
    player.field.clear();
    player.field.resize(fieldSize);
    // inserting . as place holder
    for (int i = 0; i < fieldSize; i++) {
        for (int j = 0; j < fieldSize; j++) {
            player.field[i].push_back('.');
        }
    }
}

// game function
void game() {
    //genAxis(preset);
    //fieldGen(preset, p1);
    //displayField(preset, p1);
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
MenuID goback(stack<MenuID>& stack){
    if (stack.size() > 1){
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
        //system("cls");
        for (int i = 0; i < 30; i++) {
            cout << endl;
        }
    #else
        system("clear");
    #endif
}

//------------------------------------------------------------------------------------