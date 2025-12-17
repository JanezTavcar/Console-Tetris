#include <iostream>
#include <cstdlib>
#include "pieces.h"

int piece_I[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

int piece_O[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
};

int piece_T[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0}
};

int piece_S[4][4] = {
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
};

int piece_Z[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
};

int piece_J[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 0}
};

int piece_L[4][4] = {
    {0, 0, 0, 0},
    {1, 1, 1, 0},
    {1, 0, 0, 0},
    {0, 0, 0, 0}
};

int (*pieces[7])[4] = { piece_I, piece_O, piece_T, piece_S, piece_Z, piece_J, piece_L };

void pickRandomPiece(int (&out)[4][4]) {
    int index = rand() % 7;
    int newPiece[4][4];
    memcpy(newPiece, pieces[index], sizeof(newPiece));

    int color = std::rand() % 7 + 1;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (newPiece[i][j] != 0)
            {
                newPiece[i][j] = color;
            }
    memcpy(out, newPiece, sizeof(newPiece));
}

