#include <iostream>
#include <cstring>
#include <conio.h>
#include <windows.h>

#include "pieces.h"

const int WIDTH = 10;
const int HEIGHT = 24;

int score = 0;

void rotatePiece(int (&out)[4][4], int (&piece)[4][4], bool clockwise);
void placePiece(int (&board)[HEIGHT][WIDTH], int (&piece)[4][4], int posX, int posY);
bool canPlace(int (&board)[HEIGHT][WIDTH], int (&piece)[4][4], int posX, int posY);
void clearFullRows(int (&board)[HEIGHT][WIDTH], int (&piece)[4][4], int posX, int posY);
void pickRandomPiece(int (&piece)[4][4]);
void hideCursor();
void setCursor(int x, int y);
void printBoard(int (&board)[HEIGHT][WIDTH], int (&piece)[4][4], int posX, int posY);
void DEBUGprintBoard(int (&board)[HEIGHT][WIDTH]);

void rotatePiece(int (&out)[4][4], int (&piece)[4][4], bool clockwise = true)
{
    memset(out, 0, sizeof(out));

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            if (clockwise)
                out[col][3 - row] = piece[row][col];
            else
                out[3 - col][row] = piece[row][col];
        }
    }
}

void placePiece(int (&board)[HEIGHT][WIDTH], int (&piece)[4][4], int posX, int posY)
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            if (piece[row][col] != 0)
            {
                board[posY + row][posX + col] = piece[row][col];
            }
        }
    }
}

bool canPlace(int (&board)[HEIGHT][WIDTH], int (&piece)[4][4], int posX, int posY)
{
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            if (piece[row][col] != 0)
            {
                int boardX = posX + col;
                int boardY = posY + row;

                if (boardX < 0 || boardX >= WIDTH || boardY < 0 || boardY >= HEIGHT)
                    return false;

                if (board[boardY][boardX] != 0)
                    return false;
            }
        }
    }
    return true;
}

void clearFullRows(int (&board)[HEIGHT][WIDTH], int (&piece)[4][4], int posX, int posY)
{
    for (int row = HEIGHT - 1; row > 0; --row)
    {
        bool full = true;
        for (int col = 0; col < WIDTH; ++col)
        {
            if (board[row][col] == 0)
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            for (int r = row; r > 0; --r)
            {
                memcpy(board[r], board[r - 1], sizeof(board[r]));
            }
            memset(board[0], 0, sizeof(board[0]));

            score+=100;
        }
    }
}

void printBoard(int (&board)[HEIGHT][WIDTH], int (&piece)[4][4], int posX, int posY)
{
    int result[HEIGHT][WIDTH];
    memcpy(result, board, sizeof(result));
    placePiece(result, piece, posX, posY);
    
    std::cout << "+-+";
    for (int i = 0; i < WIDTH * 2; ++i) std::cout << "-";
    std::cout << "+-+" << std::endl;

    for (int row = 0; row < HEIGHT; ++row) 
    {
        std::cout << "| |";
        
        for (int col = 0; col < WIDTH; ++col) 
        {
            if (result[row][col] == 0) 
            {
                std::cout << "  ";
            } else 
            {
                std::cout << "[]";
            }
        }

        std::cout << "| |" << std::endl;
    }

    std::cout << "+-+";
    for (int i = 0; i < WIDTH * 2; ++i) std::cout << "-";
    std::cout << "+-+" << std::endl;

    std::cout << "Score: " << score << " ";
    std::cout << "(Press 'q' to quit)" << std::endl;
    std::cout << "Controls: A - left, D - right, W - rotate, S - down" << std::endl;
}

void hideCursor()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(h, &info);
}

void setCursor(int x, int y)
{
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void DEBUGprintBoard(int (&board)[HEIGHT][WIDTH]) 
{
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() 
{
    srand(time(nullptr));
    
    system("cls");
    hideCursor();

    int board[HEIGHT][WIDTH];
    memset(board, 0, sizeof(board));

    int currentPiece[4][4];
    int xPos = WIDTH / 2 - 2;
    int yPos = 0;

    int gravityCounter = 0;
    const int gravityThreshold = 20;

    pickRandomPiece(currentPiece);

    while(true)
    {
        setCursor(0, 0);

        if (_kbhit()) 
        {
            char ch = _getch();
            if (ch == 'q') break;
            else if (ch == 'a') 
            {
                if (canPlace(board, currentPiece, xPos - 1, yPos)) 
                {
                    xPos--;
                }
            }
            else if (ch == 'd') 
            {
                if (canPlace(board, currentPiece, xPos + 1, yPos)) 
                {
                    xPos++;
                }
            }
            else if (ch == 'w') 
            {
                int rotated[4][4];
                rotatePiece(rotated, currentPiece, true);
                if (canPlace(board, rotated, xPos, yPos)) 
                {
                    memcpy(currentPiece, rotated, sizeof(rotated));
                }
            }
            else if (ch == 's') 
            {
                if (canPlace(board, currentPiece, xPos, yPos + 1)) 
                {
                    yPos++;
                }
            }
        }
        
        if (canPlace(board, currentPiece, xPos, yPos + 1)) 
        {
            gravityCounter++;
            if (gravityCounter >= gravityThreshold) 
            {
                yPos++;
                gravityCounter = 0;
            }
        } else 
        {
            placePiece(board, currentPiece, xPos, yPos);
            pickRandomPiece(currentPiece);
            if (!canPlace(board, currentPiece, WIDTH / 2 - 2, 0)) 
            {
                // Game Over
                setCursor(0, HEIGHT + 4);
                std::cout << "Game Over! Final Score: " << score << std::endl;
                break;
            } else 
            {
                score += 25;
            }
            xPos = WIDTH / 2 - 2;
            yPos = 0;
        }
        
        clearFullRows(board, currentPiece, xPos, yPos);
        printBoard(board, currentPiece, xPos, yPos);
        Sleep(16); // ~60 FPS
    }

    system("cls");
    return 0;
}