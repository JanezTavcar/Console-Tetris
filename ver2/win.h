#ifndef WIN_H
#define WIN_H

void hideCursor();
void setCursor(int x, int y);
void clearConsole();
void sleep(int milliseconds);

bool keyDown();
int getChar();

#endif