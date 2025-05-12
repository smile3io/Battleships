#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <iostream>
#include <string>
#include <utility> // for std::pair
#include <windows.h>

namespace console_utils {
	// enables the manipulation of the console !must have!
	void enableVirtualTerminalProcessing();
	// Get cursor position (row, col, 1-based)
	bool getCursorPositionWindows(int& row, int& col);
	// Get size of console
	bool getConsoleSize(int& width, int& height);
	// Set size of console
	bool setConsoleSize(int width, int height);
	// delete char in console
	void backspace();
	// deletes line and sets cursor to the start of line 
	void deleteCurrentLine();
	// sets cursor to the start of line 
	void goToStart();
	// cursor manipulation objective
	void moveCursor(std::pair<int, int> cord);
	void cursorUp(int n = 1);
	void cursorDown(int n = 1);
	void cursorForward(int n = 1);
	void cursorBack(int n = 1);
	// Moves cursor to beginning of the line n lines down
	void cursorNextLine(int n = 1);
	// Moves cursor to beginning of the line n lines up
	void cursorPreviousLine(int n = 1);
	// Moves the cursor to column n
	void cursorHorizontalAbsolute(int n = 1);
	// Moves the cursor to row n, column m. The values are 1-based, and default to 1 (top left corner) if omitted
	void cursorPosition(int row = 1, int column = 1);
	// Clears part of the screen
	// If n is 0 (or missing), clear from cursor to end of screen
	// If n is 1, clear from cursor to beginning of the screen
	// If n is 2, clear entire screen (and moves cursor to upper left on DOS ANSI.SYS)
	// If n is 3, clear entire screen and delete all lines saved in the scrollback buffer
	void eraseInDisplay(int n);
	// Erases part of the line
	// If n is 0 (or missing), clear from cursor to the end of the line
	// If n is 1, clear from cursor to beginning of the line
	// If n is 2, clear entire line. Cursor position does not change
	void eraseInLine(int n);
	// Scroll whole page up by n (default 1) lines. New lines are added at the bottom
	void scrollUp(int lines = 1);
	// Scroll whole page down by n (default 1) lines. New lines are added at the top
	void scrollDown(int lines = 1);
	// Shows the cursor
	void showCursor();
	// Hides the cursor
	void hideCursor();
	/*
	0	Reset or normal	All attributes become turned off
	1	Bold or increased intensity	As with faint, the color change is a PC(SCO / CGA) invention.[24][better source needed]
	2	Faint, decreased intensity, or dim	May be implemented as a light font weight like bold.[25]
	3	Italic	Not widely supported.Sometimes treated as inverse or blink.[24]
	4	Underline	Style extensions exist for Kitty, VTE, mintty, iTerm2and Konsole.[26][27][28]
	5	Slow blink	Sets blinking to less than 150 times per minute
	6	Rapid blink	MS - DOS ANSI.SYS, 150 + per minute; not widely supported
	7	Reverse video or invert	Swap foreground and background colors; inconsistent emulation[29][dubious – discuss]
	8	Conceal or hide	Not widely supported.
	9	Crossed - out, or strike	Characters legible but marked as if for deletion.Not supported in Terminal.app.
	10	Primary(default) font
	11–19	Alternative font	Select alternative font n − 10
	20	Fraktur(Gothic)	Rarely supported
	21	Doubly underlined; or : not bold	Double - underline per ECMA - 48, [14] : 8.3.117  but instead disables bold intensity on several terminals, including in the Linux kernel's console before version 4.17.[30]
	22	Normal intensity	Neither bold nor faint; color changes where intensity is implemented as such.
	23	Neither italic, nor blackletter
	24	Not underlined	Neither singly nor doubly underlined
	25	Not blinking	Turn blinking off
	26	Proportional spacing	ITU T.61 and T.416, not known to be used on terminals
	27	Not reversed
	28	Reveal	Not concealed
	29	Not crossed out
	30–37	Set foreground color
	38	Set foreground color	Next arguments are 5; n or 2; r; g; b
	39	Default foreground color	Implementation defined(according to standard)
	40–47	Set background color
	48	Set background color	Next arguments are 5; n or 2; r; g; b
	49	Default background color	Implementation defined(according to standard)
	50	Disable proportional spacing	T.61 and T.416
	51	Framed																		Implemented as "emoji variation selector" in mintty.[31]
	52	Encircled
	53	Overlined	Not supported in Terminal.app
	54	Neither framed nor encircled
	55	Not overlined
	58	Set underline color	Not in standard; implemented in Kitty, VTE, mintty, and iTerm2.[26][27] Next arguments are 5; n or 2; r; g; b.
	59	Default underline color	Not in standard; implemented in Kitty, VTE, mintty, and iTerm2.[26][27]
	60	Ideogram underline or right side line										Rarely supported
	61	Ideogram double underline, or double line on the right side
	62	Ideogram overline or left side line
	63	Ideogram double overline, or double line on the left side
	64	Ideogram stress marking
	65	No ideogram attributes	Reset the effects of all of 60–64
	73	Superscript																	Implemented only in mintty[31]
	74	Subscript																	-"-
	75	Neither superscript nor subscript
	90–97	Set bright foreground color												Not in standard; originally implemented by aixterm[15]
	100–107	Set bright background color
	*/
	// Sets colors and style of the characters following this code
	void selectGraphicRendition(int n);
	// background colors 
	// https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit
	void backgroundColor(int n);
	void backgroundColor(int r, int g, int b);
	void resetBackgroundColor();
	// foreground colors
	// https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit
	void foregroundColor(int n);
	void foregroundColor(int r, int g, int b);
	void resetForegroundColor();


} // namespace console_utils

#endif // CONSOLE_UTILS_H