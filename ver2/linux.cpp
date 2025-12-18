#ifdef __linux__

#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>

#include "linux.h"

static struct termios orig_termios;
static bool rawEnabled = false;

static void disableRawMode()
{
    if (!rawEnabled) return;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    rawEnabled = false;
    // restore cursor visibility
    std::cout << "\033[?25h" << std::flush;
}

static void enableRawMode()
{
    if (rawEnabled) return;
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) return;
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    rawEnabled = true;
    std::atexit(disableRawMode);
}

void hideCursor()
{
    std::cout << "\033[?25l" << std::flush;
    enableRawMode();
}

void setCursor(int x, int y)
{
    // ANSI sequences are 1-based; main uses 0-based coords so add +1
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H" << std::flush;
}

void clearConsole()
{
    std::cout << "\033[2J\033[1;1H" << std::flush;
}

void sleep(int milliseconds)
{
    usleep(milliseconds * 1000);
}

bool keyDown()
{
    enableRawMode();
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    struct timeval tv = {0, 0};
    int rv = select(STDIN_FILENO + 1, &set, nullptr, nullptr, &tv);
    return rv > 0;
}

int getChar()
{
    enableRawMode();
    char c = 0;
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n <= 0) return -1;
    return (unsigned char)c;
}

#endif