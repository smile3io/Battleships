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

enum class MenuID {
    MAIN, PLAY, PRESETS, OPTIONS, SHIPS, MODE, AI
};

enum Settings{
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
public:
    int number;         // player number
    string name;        // player name
    int score;          // session score calc -> win, ships sunken points
    vector<vector<char>> field; // own player field 
    int shots;          // shot count 

    //
    void shoot(){}

    //
    void placeShips(){
        int input = _getch();
        if (input == UP) {
        }
        else if (input == DOWN){

        }
        else if (input == LEFT){

        }
        else if (input == RIGHT){

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
    void scoring(){}
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
        distance = CLOSE;
        ship = FIVE;
        fieldSize = 26;
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

    void createShips() {
        int lengthShips[] = { 5,4,3,3,2 };
        if(!shipCount){
            int shipFields = fieldSize * fieldSize * (ship / 100.);
            while (shipFields >= 2) {
                for (int num : lengthShips) {
                    if (num > shipFields) {
                        break;
                    }
                    shipFields -= num;
                    ships.push_back(num);
                }
            }
        }else {
            int temp = shipCount;
            while (temp) {
                for (int num : lengthShips) {
                    if (temp) {
                        break;
                    }
                    shipCount--;
                    ships.push_back(num);
                }
            }
        }
        
    }

    void rules() {
        switch (mode){
        case STANDARD:
            cout << "Klassisches Schiffe versenken.\n"; break;
        case SALVO:
            cout << "Funf Schüsse gleichzeitig.\n"; break;
        case RACE:
            cout << "Rennen wer als erstes zum Ende kommt.\n"; break;
        }
        switch (distance){
        case FAR:
            cout << "Schiffe mussen ein Feld auseinander platziert werden.\n"; break;
        case CLOSE:
            cout << "Schiffe konnen direkt nebeneinander platziert werden\n"; break;
        }
        switch (ship){
        case FIVE:
            cout << "Die Anzahl der Schiffe ist gering.\n"; break;
        case TEN:
            cout << "Die Anzahl der Schiffe ist hoch.\n"; break;
        }
        switch (aiDifficulty){
        case EASY:
            cout << "Die KI ist auf Einfach eingestellt.\n"; break;
        case NORMAL:
            cout << "Die KI ist auf Normal eingestellt.\n"; break;
        case HARD:
            cout << "Die KI ist auf Schwer eingestellt.\n"; break;
        }
    }
};

//--------------------------------------GLOBAL--------------------------------------------

//char alph[27] = { ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
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
        {'3', []() {custom.rules(); }},
        {'4', []() {menuStack.push(MenuID::OPTIONS);}},
        {'0', []() {/*return*/}}}},
    {MenuID::PLAY, {
        {'1', []() {custom.playerCount = 1; }},
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
        {'2', []() {clearConsole(); cout << "Welche Seitenlange soll das Feld haben?\n";  int size; cin >> size; custom.fieldSize = size; }},
        {'3', []() {clearConsole(); cout << "Wie viele Schiffe sollen es sein?\n\t1. wenige\n\t2. viele\n"; int num = _getch(); if (0 < num < 3) { num == 1 ? custom.ship = FIVE : custom.ship = TEN; }; custom.shipCount = 0; }},
        {'4', []() {clearConsole(); cout << "Wie viele Schiffe willst du haben?\n(Diese Einstellung uberscheibt die automatische Anzahl!)\n";  int num; cin >> num; custom.shipCount = num; }},
        {'5', []() {clearConsole(); cout << "Welche distanz sollen die Schiffe haben?\n\t1. nah\n\t2. fern\n"; int num = _getch(); if (0 < num < 3) { num == 1 ? custom.distance = CLOSE : custom.distance = FAR; }}},
        {'6', []() {menuStack.push(MenuID::AI); }},
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

void fieldGen1(const Setup& objekt, Player& player);

int main() {
    menuStack.push(MenuID::MAIN);
    Player p1;

    fieldGen1(preset, p1);

    while (true) {
        
        /*
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
        */
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


void fieldGen1(const Setup& settings, Player& player) {
    int fieldSizeWithBorder = settings.fieldSize+1;
    player.field.clear();
    player.field.resize(fieldSizeWithBorder);
    char character = 65;
    for (int i = 0; i < fieldSizeWithBorder; i++) {
        if (!i){ player.field[0].push_back(' '); }
        player.field[0].push_back(character);
        character++;
    }
    cout << endl;
    char number = 49;
    for (int i = 1; i < fieldSizeWithBorder; i++) {
        if ((i-1/10) < 1){
            player.field[i].push_back('0');
            player.field[i].push_back(number);
        }
        player.field[i].push_back(number);
        number++;
    }
    for (int i = 1; i < fieldSizeWithBorder; i++) {
        for (int j = 1; j < fieldSizeWithBorder; j++) {
            player.field[i].push_back('.');
        }
    }
    


    for (int i = 0; i < fieldSizeWithBorder; i++) {
        for (int j = 0; j < fieldSizeWithBorder; j++) {
            cout << player.field[i][j] << " ";
        }
        cout << endl;
    }
}   






// alt muss neugemacht werden

// zweigt das ausgewählte spielfeld an au<s der sicht des ausführenden spielers
void displayField(const vector<vector<vector<string>>>& grid, int player, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << grid[player][i][j] << " ";
        }
    }
}

//------------------------------------------------------------------------------------