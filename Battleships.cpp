#include "Game.h"
#include "console_utils.h"

int main() {
    enableVirtualTerminalProcessing();
    Game game;
    game.run();
    return 0;
}