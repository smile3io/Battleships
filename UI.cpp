#include "UI.h"

extern Setup gameSetup;
extern int consoleWidth, consoleHeight;
extern int fieldDisplayWidth, fieldDisplayHeight;
extern int sleepTime;

void titleBox() {
    // clear screen
    console_utils::eraseInDisplay(2);
    // visual box around the console
    console_utils::cursorPosition(1, 1);
    for (int i = 0; i < (consoleWidth - 17) / 2; i++) std::cout << "=";
    console_utils::selectGraphicRendition(1);
    std::cout << " [ BATTLESHIPS ] ";
    console_utils::selectGraphicRendition(0);
    for (int i = 0; i < (consoleWidth - 16) / 2; i++) std::cout << "=";
    std::cout << "\n";
    for (int i = 0; i < consoleHeight - 2; i++) std::cout << "|\n";
    for (int i = 0; i < consoleWidth; i++) std::cout << "=";
    console_utils::cursorPosition(2, consoleWidth);
    for (int i = 0; i < consoleHeight - 2; i++) { console_utils::cursorHorizontalAbsolute(consoleWidth); std::cout << "|\n"; }
}

void currentControlls(Controlls UI) {
    console_utils::cursorPosition(consoleHeight - 2, 3);
    std::cout << "CONTROLLS:\n";
    console_utils::cursorHorizontalAbsolute(3);
    switch (UI)
    {
    case Controlls::MENU: // MENU
        std::cout << "0-9 - choose option | ESC - go back";
        break;
    case Controlls::NAME: // NAME
        std::cout << "A-Z - write name | ESC - go back";
        break;
    case Controlls::FIELDSIZE: // FIELDSIZE
        std::cout << "0-50 - choose option | ESC - go back";    // change max fieldsize by testing
        break;
    case Controlls::SHIPCOUNT: // SHIPCOUNT
        std::cout << "NUMBER - choose option | ESC - go back";    // change max fieldsize by testing
        break;
    case Controlls::PLACING: // PLACING
        std::cout << "ARROWS - move arround | R - rotate | ENTER - place ship | ESC - go back";
        break;
    case Controlls::SHOOTING: // SHOOTING
        std::cout << "ARROWS - move arround | ENTER - shoot | ESC - go back";
        break;
    default:
        break;
    }
}

void displayMenu(const std::vector<std::pair<char, std::string>>& options) {
    titleBox();
    console_utils::cursorPosition(consoleHeight * 0.35, consoleWidth * 0.25);
    for (auto& pair : options) {
        std::cout << "[" << pair.first << "]  " << pair.second << std::endl;
        console_utils::cursorHorizontalAbsolute(consoleWidth * 0.25);
    }
    currentControlls(Controlls::MENU);
}

void printField(std::vector<std::vector<char>> field, int fieldWidth) {
    // x-axis
    std::cout << "  ";
    for (int k = 0; k <= gameSetup.fieldSize; k++) {
        k < 26 && gameSetup.fieldSize > 26 ? std::cout << gameSetup.xAxisLabel[k] << "  " : std::cout << gameSetup.xAxisLabel[k] << " ";
    }
    std::cout << std::endl;
    console_utils::cursorHorizontalAbsolute(fieldWidth);
    // y-axis and field
    for (int i = 0; i < gameSetup.fieldSize; i++) {
        if (gameSetup.yAxisLabel[i] < 10) std::cout << " ";
        std::cout << gameSetup.yAxisLabel[i] << " ";
        for (int j = 0; j < gameSetup.fieldSize; j++) {
            switch (field[i][j])
            {
            case '~':
                console_utils::backgroundColor(21);
                break;
            case 'O':
                console_utils::backgroundColor(255);
                break;
            case 'X':
                console_utils::backgroundColor(196);
                break;
            case '*':
                console_utils::backgroundColor(245);
                break;
            default:
                console_utils::resetBackgroundColor();
                break;
            }
            std::cout << field[i][j] << (gameSetup.fieldSize > 26 ? "  " : " ");
            console_utils::resetBackgroundColor();
        }
        console_utils::cursorBack(fieldDisplayWidth * 2);
        console_utils::cursorDown();
        console_utils::cursorHorizontalAbsolute(fieldWidth);
    }
}

void renderPlace(Player& player, std::pair<int, int> cursor, int length, Rotation rotation) {
    std::vector<std::vector<char>> renderField = player.field;

    int col = cursor.first;
    int row = cursor.second;

    if (col < 0 || col >= gameSetup.fieldSize || row < 0 || row >= gameSetup.fieldSize) {
        return;
    }

    for (int i = 0; i < gameSetup.fieldSize; i++) {
        for (int j = 0; j < gameSetup.fieldSize; j++) {
            if (player.ships[i][j] != ' ') {
                renderField[i][j] = player.ships[i][j];
            }
        }
    }

    if (rotation == Rotation::HOR) {
        for (int i = 0; i < length; i++) {
            renderField[col][row + i] = player.ships[col][row + i] == '*' ? 'X' : '#';
        }
    }
    else if (rotation == Rotation::VER) {
        for (int i = 0; i < length; i++) {
            renderField[col + i][row] = player.ships[col + i][row] == '*' ? 'X' : '#';
        }
    }

    titleBox();
    currentControlls(Controlls::PLACING);
    console_utils::cursorPosition(consoleHeight * 0.3, consoleWidth * 0.05);
    std::cout << player.name << " your turn\n\n";
    console_utils::cursorHorizontalAbsolute(consoleWidth * 0.05);
    std::cout << "Placing a " << length << " long ship\n";
    console_utils::cursorHorizontalAbsolute(consoleWidth * 0.05);
    std::cout << "(" << (rotation == Rotation::HOR ? "horizontal" : "vertical") << ")\n";

    int fieldHeight = (consoleHeight + 2 - fieldDisplayHeight) / 2 + 1;
    int fieldWidth = (consoleWidth + 2 - fieldDisplayWidth) / 2 + 1;

    console_utils::cursorPosition(fieldHeight, fieldWidth);
    printField(renderField, fieldWidth);
    fieldBox(fieldHeight, fieldWidth, &player);
    console_utils::cursorPosition(consoleHeight + 2, 1);
}

void renderShoot(Player& target, Player& self, std::pair<int, int> cursor) {

}

void renderMulti(std::vector<Player>& playerVect) {

}

void fieldBox(int x, int y, Player* p) {
    // title or who's board 
    console_utils::cursorPosition(x - 3, y - 1);
    console_utils::selectGraphicRendition(1);
    std::cout << (p ? p->name + " Board" : " Your Board");
    console_utils::selectGraphicRendition(0);
    std::cout.flush();
    // box 
    int width = fieldDisplayWidth - 1;
    console_utils::cursorPosition(x - 2, y - 3);
    std::cout << "+";
    for (int i = 0; i < width; ++i) std::cout << "-";
    std::cout << "+";
    for (int i = 0; i <= fieldDisplayHeight - 2; ++i)
    {
        console_utils::cursorPosition(x - 1 + i, y - 3);
        std::cout << "|";
        console_utils::cursorPosition(x - 1 + i, y + width - 2);
        std::cout << "|";
    }
    console_utils::cursorPosition(x + fieldDisplayHeight - 3, y - 3);
    std::cout << "+";
    for (int i = 0; i < width; ++i) std::cout << "-";
    std::cout << "+";
    std::cout.flush();
}


