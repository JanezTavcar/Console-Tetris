#ifdef _WIN32

#include <windows.h>
#include <conio.h>

#include "win.h"

void hideCursor()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.bVisible = FALSE;
    info.dwSize = 1;
    SetConsoleCursorInfo(h, &info);
}

void setCursor(int x, int y)
{
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearConsole()
{
    system("cls");
}

void sleep(int milliseconds)
{
    Sleep(milliseconds);
}

bool keyDown()
{
    return _kbhit();
}

int getChar()
{
    return _getch();
}

#endif