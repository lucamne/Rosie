#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>
#include <stdbool.h>

#define NAME "Rosie 1.0"
#define BRD_SQ_NUM 120
// half moves, arbitrarily chosen large number
#define MAX_GAME_MOVES 2048
typedef  uint64_t U64;

typedef enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK} Piece;
typedef enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE } File;
typedef enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE } Rank;
typedef enum { WHITE, BLACK, BOTH } Color;

// Castling permissions will be stored in a 4 bit int with each bit representing a castling permission
enum { WKCA = 0b0001, WQCA = 0b0010, BKCA = 0b0100, BQCA = 0b1000 };

typedef enum {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
} Square;

typedef struct {
	int move;
	unsigned char castlePerm;
	Square enPassantSquare;
	int fiftyMoveCounter;
	U64 positionKey;

} s_Undo;

typedef struct {
	// Tracks piece positions
	// Board is 120 squares to deal with off board checks when finding moves see png in folder
	int pieces[BRD_SQ_NUM];
	// Each bit represents a square and there is one index for each color
	U64 pawns[3];

	Square KingSquare[2];
	Square enPassantSquare;

	Color sideToMove;
	int fiftyMoveCounter;
	// Ply is number of half moves 
	int Ply;
	unsigned char castlePerm;
	// unique key for each position
	U64 positionKey;
	// How many pieces of each type are on the board
	int pieceCounts[13];
	// Store by color the number of non pawns on the board
	int bigPieceCounts[3];
	// Store by color the number of rooks and queens on the board
	int majorPieceCounts[3];
	// Store by color the number of Knights and Bishops on the board
	int minorPieceCounts[3];

	int historyPly;
	s_Undo history[MAX_GAME_MOVES];
	
} s_Board;


#endif
