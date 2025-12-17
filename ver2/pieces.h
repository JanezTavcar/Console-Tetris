#ifndef PIECES_H
#define PIECES_H

void DEBUGprintPiece(int piece[4][4]);

extern int piece_I[4][4];
extern int piece_O[4][4];
extern int piece_T[4][4];
extern int piece_S[4][4];
extern int piece_Z[4][4];
extern int piece_J[4][4];
extern int piece_L[4][4];

void pickRandomPiece(int (&out)[4][4]);

#endif // PIECES_H