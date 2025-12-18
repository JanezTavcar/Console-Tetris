#include <iostream>
#include <cstring>

#include "pieces.h"

#ifdef _WIN32
  #include "win.h"
#elif __linux__
  #include "linux.h"
#else
  #error "Unsupported platform"
#endif

constexpr int WIDTH = 10;
constexpr int HEIGHT = 24;

constexpr int SCORE_INCREASE_PER_ROW = 100;
constexpr int SCORE_INCREASE_PER_PIECE = 25;

constexpr int gravityThreshold = 20;

struct GameState
{
    int board[HEIGHT][WIDTH];
    int currentPiece[4][4];
    int xPos;
    int yPos;
    int score;
    int gravityCounter = 0;
} gameState;

void rotatePiece(int (&out)[4][4], int (&piece)[4][4], bool clockwise);
void placePiece(int (&board)[HEIGHT][WIDTH], int (&piece)[4][4], int posX, int posY);
bool canPlace(int (&board)[HEIGHT][WIDTH], int (&piece)[4][4], int posX, int posY);
void clearFullRows(int (&board)[HEIGHT][WIDTH]);
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

void clearFullRows(int (&board)[HEIGHT][WIDTH])
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

            gameState.score += SCORE_INCREASE_PER_ROW;

            row++;
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
            const char* colors[] = {
                "\x1b[0m",       // 0 - reset
                "\x1b[31m",      // 1 - red
                "\x1b[32m",      // 2 - green
                "\x1b[33m",      // 3 - yellow
                "\x1b[34m",      // 4 - blue
                "\x1b[35m",      // 5 - magenta
                "\x1b[36m",      // 6 - cyan
                "\x1b[37m"       // 7 - white
            };

            if (result[row][col] != 0) 
            {
                std::cout << colors[result[row][col]] << "[]" << colors[0];
            }
            else
            {
                std::cout << "  ";
            }
        }

        std::cout << "| |" << std::endl;
    }

    std::cout << "+-+";
    for (int i = 0; i < WIDTH * 2; ++i) std::cout << "-";
    std::cout << "+-+" << std::endl;

    std::cout << "Score: " << gameState.score << " ";
    std::cout << "(Press 'q' to quit)" << std::endl;
    std::cout << "Controls: A - left, D - right, W - rotate, S - down" << std::endl;
}

int main() 
{
    srand(time(nullptr));
    
    clearConsole();
    hideCursor();

    memset(gameState.board, 0, sizeof(gameState.board));

    gameState.xPos = WIDTH / 2 - 2;
    gameState.yPos = 0;



    pickRandomPiece(gameState.currentPiece);

    while(true)
    {
        setCursor(0, 0);

        while (keyDown()) 
        {
            char ch = getChar();
            if (ch == 'q') break;
            else if (ch == 'a') 
            {
                if (canPlace(gameState.board, gameState.currentPiece, gameState.xPos - 1, gameState.yPos)) 
                {
                    gameState.xPos--;
                }
            }
            else if (ch == 'd') 
            {
                if (canPlace(gameState.board, gameState.currentPiece, gameState.xPos + 1, gameState.yPos)) 
                {
                    gameState.xPos++;
                }
            }
            else if (ch == 'w') 
            {
                int rotated[4][4];
                rotatePiece(rotated, gameState.currentPiece, true);
                if (canPlace(gameState.board, rotated, gameState.xPos, gameState.yPos)) 
                {
                    memcpy(gameState.currentPiece, rotated, sizeof(rotated));
                }
            }
            else if (ch == 's') 
            {
                if (canPlace(gameState.board, gameState.currentPiece, gameState.xPos, gameState.yPos + 1)) 
                {
                    gameState.yPos++;
                }
            }
            else if (ch == ' ') 
            {
                for (int dropY = gameState.yPos + 1; dropY < HEIGHT; ++dropY) 
                {
                    if (!canPlace(gameState.board, gameState.currentPiece, gameState.xPos, dropY)) 
                    {
                        gameState.yPos = dropY - 1;
                        break;
                    }
                }
            }
        }

        gameState.gravityCounter++;
        if (gameState.gravityCounter >= gravityThreshold) 
        {
            gameState.yPos++;
            gameState.gravityCounter = 0;
        }

        if (!canPlace(gameState.board, gameState.currentPiece, gameState.xPos, gameState.yPos))
        {
            placePiece(gameState.board, gameState.currentPiece, gameState.xPos, gameState.yPos - 1);
            pickRandomPiece(gameState.currentPiece);
            if (!canPlace(gameState.board, gameState.currentPiece, WIDTH / 2 - 2, 0)) 
            {
                setCursor(0, HEIGHT + 4);
                std::cout << "Game Over! Final Score: " << gameState.score << std::endl;
                break;
            } else 
            {
                gameState.score += SCORE_INCREASE_PER_PIECE;
            }
            gameState.xPos = WIDTH / 2 - 2;
            gameState.yPos = 0; 
        }

        clearFullRows(gameState.board);
        printBoard(gameState.board, gameState.currentPiece, gameState.xPos, gameState.yPos);
        sleep(16); // ~60 FPS
    }

    clearConsole();
    return 0;
}