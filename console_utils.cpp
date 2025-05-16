#include "console_utils.h"

void enableVirtualTerminalProcessing() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hConsole, &mode);
	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hConsole, mode);
}

bool getCursorPositionWindows(int& row, int& col) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		return false;
	}
	row = csbi.dwCursorPosition.Y + 1;	// 1-based for consistency with ANSI
	col = csbi.dwCursorPosition.X + 1;
	return true;
}
bool getConsoleSize(int& width, int& height) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		return false;
	}
	width = csbi.srWindow.Right - csbi.srWindow.Left + 1; // Visible columns
	height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1; // Visible rows
	return true;
}
bool setConsoleSize(int width, int height) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) return false;
	// Ensure minimum size (Windows imposes limits)
	width = std::max(width, 20); // Arbitrary minimum columns
	height = std::max(height, 8); // Arbitrary minimum rows
	// Set screen buffer size (must be at least window size)
	COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
	if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) {
		return false;
	}
	// Set window size
	SMALL_RECT windowRect = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &windowRect)) {
		return false;
	}
	// Verify size
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		return false;
	}
	int currentWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int currentHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return (currentWidth == width && currentHeight == height);
}

void backspace() {
	std::cout << "\b \b";
	std::cout.flush();
}
void deleteCurrentLine() {
	std::cout << "\x1b[2K\x1b[0G";
	std::cout.flush();
}
void goToStart() {
	std::cout << "\x1b[0G";
	std::cout.flush();
}

void moveCursor(std::pair<int, int> cord) {
	if (cord.first > 0) {
		std::cout << "\x1b[" << abs(cord.first) << "C";
		std::cout.flush();
	}
	else if (cord.first < 0) {
		std::cout << "\x1b[" << abs(cord.first) << "D";
		std::cout.flush();
	}
	if (cord.second > 0) {
		std::cout << "\x1b[" << abs(cord.second) << "A";
		std::cout.flush();
	}
	else if (cord.second < 0) {
		std::cout << "\x1b[" << abs(cord.second) << "B";
		std::cout.flush();
	}
}
void cursorUp(int n) {
	if (n > 0) {
		std::cout << "\x1b[" << n << "A";
		std::cout.flush();
	}
}
void cursorDown(int n) {
	if (n > 0) {
		std::cout << "\x1b[" << n << "B";
		std::cout.flush();
	}
}
void cursorForward(int n) {
	if (n > 0) {
		std::cout << "\x1b[" << n << "C";
		std::cout.flush();
	}
}
void cursorBack(int n) {
	if (n > 0) {
		std::cout << "\x1b[" << n << "D";
		std::cout.flush();
	}
}

void cursorNextLine(int n) {
	if (n > 0) {
		std::cout << "\x1b[" << n << "E";
		std::cout.flush();
	}
}
void cursorPreviousLine(int n) {
	if (n > 0) {
		std::cout << "\x1b[" << n << "F";
		std::cout.flush();
	}
}
void cursorHorizontalAbsolute(int n) {
	if (n > 0) {
		std::cout << "\x1b[" << n << "G";
		std::cout.flush();
	}
}
void cursorPosition(int row, int column) {
	if (row >= 1 && column >= 1) {
		std::cout << "\x1b[" << row << ";" << column << "H";
		std::cout.flush();
	}
}

void eraseInDisplay(int n) {
	if (0 <= n <= 3) {
		std::cout << "\x1b[" << n << "J";
		std::cout.flush();
	}
}
void eraseInLine(int n) {
	if (0 <= n <= 2) {
		std::cout << "\x1b[" << n << "K";
		std::cout.flush();
	}
}

void scrollUp(int lines) {
	if (lines >= 0) {
		std::cout << "\x1b[" << lines << "S";
		std::cout.flush();
	}
}
void scrollDown(int lines) {
	if (lines >= 0) {
		std::cout << "\x1b[" << lines << "T";
		std::cout.flush();
	}
}

void showCursor() {
	std::cout << "\x1b[?25h";
	std::cout.flush();
}
void hideCursor() {
	std::cout << "\x1b[?25l";
	std::cout.flush();
}

void selectGraphicRendition(int n) {
	if (0 <= n <= 107) {
		std::cout << "\x1b[" << n << "m";
		std::cout.flush();
	}
}

void backgroundColor(int n) {
	if (0 <= n <= 255) {
		std::cout << "\x1b[48;5;" << n << "m";
		std::cout.flush();
	}
}
void backgroundColor(int r, int g, int b) {
	if (0 <= r <= 255 && 0 <= g <= 255 && 0 <= b <= 255) {
		std::cout << "\x1b[48;2;" << r << ";" << g << ";" << b << "m";
		std::cout.flush();
	}
}
void resetBackgroundColor() {
	std::cout << "\x1b[49m";
	std::cout.flush();
}

void foregroundColor(int n) {
	if (0 <= n <= 255) {
		std::cout << "\x1b[38;5;" << n << "m";
		std::cout.flush();
	}
}
void foregroundColor(int r, int g, int b) {
	if (0 <= r <= 255 && 0 <= g <= 255 && 0 <= b <= 255) {
		std::cout << "\x1b[38;5;" << r << ";" << g << ";" << b << "m";
		std::cout.flush();
	}
}
void resetForegroundColor() {
	std::cout << "\x1b[39m";
	std::cout.flush();
}