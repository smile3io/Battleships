#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <map>
#include <stack>
#include <functional>

using namespace std;
using action = function<void()>;

/*
- zwei windows für jeden eins
- leaderboard kann auf zb sql oder exel oder sheets datei zugreifen und diese umschreiben bzw lesen und dieses dann ausgeben
- Salvo mode 5 shots at once
- uboot -> kann mit anderem schiff überlappen oder kann nach treffer
- unterwasser gehen oder position ändern Leaderboard KI Schwierigkeiten
- Feldgrößen - Schiffs anzahl
- Mehrspieler zB 2-4
- Schiffe automatisch plazieren
- Regeln ändern (Platz zwischen Schiffen)
- Unterschiedliche Schiffs größen  2345(6789)->wenn map größer als 10
- für jedes schiff eine fähigkeit
- (Abilities wie "Sonar"->deckt fläche auf, Unterwasser Minen -> sagen dass
- gehitet wurde als ablenkung, torpedo -> schießt in einer linie) Anzahl an
- schiffen berechnung -> (size*size)*0.17 -> anzahl an schiff feldern -> ans -
- 2(3,3,4,5)   -> periodisch
*/
//---------------------------------------ENUM--------------------------------------------------

enum class MenuID {
    MAIN, PLAY, PRESETS, OPTIONS, SHIPS, MODE, AI
};

enum Settings{
    SMALL, MEDIUM, LARGE,   // size
    EASY, NORMAL, HARD,     // diff
    STANDARD, SALVO, RACE,  // mode
    FAR, CLOSE,             // dist
    FIVE = 17, TEN = 30     // ship
};

//---------------------------------------Struct------------------------------------------------

//--------------------------------------KLASSEN----------------------------------------------

class Field { // . nichts  X kein Treffer  O Treffer  #  --- ||| versenkt
    int size;
};

class Ships {
    int player; // player number
    int size;   // ship size
    int x;      // position upper left
    int y;      // position upper left
    int dir;    // 0 horizontal  1 vertikal

    // positioning of ships
    void positionShip(int size, int x, int y, int dir) {

    }
};

class Player {
    int number;       // player number
    string name;      // player name
    int sessionScore; // session score calc -> win, ships sunken points

    // score considers win and sunken ships and hits (and total shots)
    void score(int player) {
    }
};

class Setup {
public:
    int playerCount;    //  easy    normal      hard                custom
    int mode;           //  normal / salvo / race?                  
    int distance;       //  normal ohne abstand / mit abstand
    int ship;           //  5 schiffe auf 10 (17 aka 0.17) / 10 auf 10 (30 aka 0.3)
    int fieldSize;      //  5       10          15                  1** 2*** 1**** 1***** 17Felder / 4** 3*** 2**** 1***** 30Felder
    int shipCount;      //  2       5           12                  0,17 / 0,30
    vector<int> ships;  //  2*2     2,3*2,4,5   2*4,3*4,4*2,5*2
    int aiDifficulty;   //  easy    normal      hard

    Setup() {
        playerCount = 1;
        mode = STANDARD;
        distance = FAR;
        ship = FIVE;
        fieldSize = 10;
        shipCount = 5;
        aiDifficulty = NORMAL;
    }

    void setPreset(int diff) {
        switch (diff) {
            case SMALL:
                mode = STANDARD;
                fieldSize = 5;
                shipCount = 2;
                aiDifficulty = SMALL;
                break;
            case MEDIUM:
                mode = STANDARD;
                fieldSize = 10;
                shipCount = 5;
                aiDifficulty = MEDIUM;
                break;
            case LARGE:
                mode = STANDARD;
                fieldSize = 15;
                shipCount = 12;
                aiDifficulty = LARGE;
                break;
            default:
                break;
        }
    }

    void createShips(int total) {
        int lengthShips[] = { 2,3,3,4,5 };
        int shipFields = fieldSize * fieldSize * (total / 100.);
        while (shipFields >= 2) {
            for (int num : lengthShips) {
                if (num > shipFields) {
                    break;
                }
                shipFields -= num;
                ships.push_back(num);
            }
        }
    }

};

//--------------------------------------GLOBAL--------------------------------------------

const int num[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const string alph[27] = { " ", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
stack<MenuID> menuStack;

//-----------------------------------Deklaration--------------------------------------

void displayMenu(vector<pair<char, string>> opt);
MenuID goback(stack<MenuID>& stack);
void clearConsole();

Setup preset;
Setup custom;

//-------------------------------MAPS--------------------------------------------

map<MenuID, vector<pair<char, string>>> mData = {
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
        {'0', "Zurueck"}}},
    {MenuID::PRESETS, {
        {'1', "Klein"},
        {'2', "Mittel"},
        {'3', "Gross"},
        {'4', "Benutzeridentifiziert"},
        {'0', "Zurueck"}}},
    {MenuID::OPTIONS, {
        {'1', "Modus"},
        {'2', "Feldgroesse"},
        {'3', "Schiffs Anzahl"},
        {'4', "KI Schwierigkeit"},
        {'0', "Zurueck"}}},
    {MenuID::MODE, {
        {'1', "Normal"},
        {'2', "Salvo"},
        {'3', "Race"},
        {'0', "Zuruek"}}},
    {MenuID::AI, {
        {'1', "Einfach"},
        {'2', "Normal"},
        {'3', "Schwer"},
        {'0', "Zuruek"}}}
};

map<MenuID, map<char, action>> mActions = {
    {MenuID::MAIN, {
        {'1', []() {menuStack.push(MenuID::PRESETS);}},
        {'2', []() {}},
        {'3', []() {}},
        {'4', []() {menuStack.push(MenuID::OPTIONS);}},
        {'0', []() {}}}},
    {MenuID::PLAY, {
        {'1', []() {custom.playerCount = 1; clearConsole(); cout << custom.fieldSize; for (int i : custom.ships) { cout << endl << i << endl; }}},
        {'2', []() {custom.playerCount = 2; }},
        {'3', []() {custom.playerCount = 3; }},
        {'4', []() {custom.playerCount = 4; }},
        {'0', []() {goback(menuStack); }}}},
    {MenuID::PRESETS, {
        {'1', []() {preset.setPreset(SMALL); menuStack.push(MenuID::PLAY); }},
        {'2', []() {preset.setPreset(MEDIUM); menuStack.push(MenuID::PLAY); }},
        {'3', []() {preset.setPreset(LARGE); menuStack.push(MenuID::PLAY); }},
        {'4', []() {menuStack.push(MenuID::PLAY);}},
        {'0', []() {goback(menuStack);}}}},
    {MenuID::OPTIONS, {
        {'1', []() {menuStack.push(MenuID::MODE); }},
        {'2', []() {clearConsole(); cout << "Welche Seitenlaenge soll das Feld haben?" << endl;  int size; cin >> size; custom.fieldSize = size; }},
        {'3', []() {clearConsole(); cout << "Wie viele Schiffe willst du haben?\n(Diese Einstellung überscheibt die manuelle anzahl!)" << endl;  int num; cin >> num; custom.shipCount = num; }},
        {'4', []() {menuStack.push(MenuID::AI); }},
        {'0', []() {goback(menuStack); }}}},
    {MenuID::MODE, {
        {'1', []() {custom.mode = NORMAL; goback(menuStack); }},
        {'2', []() {custom.mode = SALVO; goback(menuStack); }},
        {'3', []() {custom.mode = RACE; goback(menuStack); }},
        {'0', []() {goback(menuStack); }}}},
    {MenuID::AI, {
        {'1', []() {custom.aiDifficulty = EASY; goback(menuStack); }},
        {'2', []() {custom.aiDifficulty = NORMAL; goback(menuStack); }},
        {'3', []() {custom.aiDifficulty = HARD; goback(menuStack); }},
        {'0', []() {goback(menuStack); }}}}
};

//----------------------------------------MAIN--------------------------------------------------

int main() {
    menuStack.push(MenuID::MAIN);


    while (true) {
        MenuID currentMenuID = menuStack.top();
        vector<pair<char, string>> currentMenu = mData[currentMenuID];

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
            mActions[currentMenuID][choice]();  // perform specific action
        }else {
            cout << "Falsche Eingabe" << endl;
            continue;
        }
    }
}

//-------------------------------------FUNKTIONEN-----------------------------------------------

// displays the selected option menu
void displayMenu(vector<pair<char, string>> opt) {
    clearConsole();
    for (auto& pair : opt) {
        cout << "\t" << pair.first << ". " << pair.second << endl;
    }
    cout << endl << endl;
}

// go one menu back (pop top menu)
MenuID goback(stack<MenuID>& stack){
    if (stack.size() > 1){
        stack.pop();
        return stack.top();
    }else {
        cout << "Fehler" << endl;
        return stack.top();
    }
}

// clears the console in a primitive way gets swapped with a command
void clearConsole() {
    for (int i = 0; i < 30; i++) {
        cout << endl;
    }
}








// alt muss neugemacht werden

// generiert das feld sobald spiel startet mit größe und numerierung der x und y achse
void fieldGen(vector<vector<vector<string>>>& grid, int size, int playerCount) {
    size++;
    for (int i = 0; i < size; i++) {
        grid[0][0][i] = alph[size / 26] + alph[size % 26];
        cout << grid[0][0][i];
    }
    for (int k = 0; k < playerCount - 1; k++) {
        for (int i = 1; i < size; i++) {
            for (int j = 1; j < size; j++) {
                cout << "k: " << k << ", i: " << i << ", j: " << j << ", value: " << grid[k][i][j] << endl; // Debugging-Ausgabe
                grid[k][i][j] = ".";
            }
        }
    }
}

// zweigt das ausgewählte spielfeld an au<s der sicht des ausführenden spielers
void displayField(const vector<vector<vector<string>>>& grid, int player, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << grid[player][i][j] << " ";
        }
    }
}

//------------------------------------------------------------------------------------