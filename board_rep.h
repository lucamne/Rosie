/*
 * Handles board representation
 *
 * Source Files:
 * board_rep.c
 * bitboard.c
 * hashkey.c
 *
 * Author: Luca Negris
 * Modified: 12/14/2024 
 */
#ifndef BOARD_REP_H
#define BOARD_REP_H

#include "global_defs.h"
#include <stdint.h>


/* ==========================================================================
 * TYPES
 * ========================================================================== */

/***Typedefs***/

// number of squares in our 10 x 12
#define BRD_SQ_NUM 120
typedef enum { OFFBOARD, EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK} Piece;
typedef enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE } File;
typedef enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE } Rank;
typedef enum { WHITE, BLACK, BOTH } Color;
// Castling permissions will be stored in a 4 bit int with each bit representing a castling permission
typedef enum { WKCA = 0b0001, WQCA = 0b0010, BKCA = 0b0100, BQCA = 0b1000 } Castle;

typedef enum {
	NO_SQ = -1,
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8 
} SQ120;

// Stores a past board state
typedef struct {
	int move;
	unsigned char castlePerm;
	SQ120 enPassantSquare;
	int fiftyMoveCounter;
	U64 positionKey;
} PastState;

typedef struct {
	// Tracks state of each square
	Piece pieces[BRD_SQ_NUM];
	// pawn bitboards for white, black, and combined
	U64 pawns[3];
	// White and black King's squares
	SQ120 kingSquare[2];
	// current en passant square if any
	SQ120 enPassantSquare;
	Color sideToMove;
	int fiftyMoveCounter;
	// current search ply
	int ply;
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
	// total game ply
	int historyPly;
	PastState history[MAX_GAME_MOVES];
	// Piece list holds square of each piece on the board
	// 1st dimension is piece type and second dimension is instance of said piece type
	Piece pieceList[13][10];		
} BoardState;


/* ==========================================================================
 * BOARD
 * ========================================================================== */


/***FUNCTIONS***/

// inits all data structures
int init(void);
SQ120 file_and_rank_to_120(int f, int r);
int file_and_rank_to_64(int f, int r);
int reset_board(BoardState *state);
int parse_fen(char* fen, BoardState* state);
int print_board(const BoardState* state);

/***Constants***/
// lookup arrays to convert between indices of 120 square and 64 square boards
extern const int sq64ToSq120[64];
extern const int sq120ToSq64[120];


/* ==========================================================================
 * BITBOARDS
 * ========================================================================== */


/***FUNCTIONS***/

int init_bit_mask(void);
int print_bitboard(U64 bb);
// pops lowest bit
int pop_bit(U64 *bb);
int count_bits(U64 b);
int set_bit(U64 *bb, int sq);
int clear_bit(U64 *bb, int sq);

/***CONSTANTS***/
extern const int bitTable[64];


/* ==========================================================================
 * HASHKEY
 * ========================================================================== */


/***FUNCTIONS***/

int init_hashkeys(void);
U64 generate_position_key(const BoardState const *state);

/***GLOBALS***/
extern U64 pieceKeys[13][BRD_SQ_NUM];
extern U64 sideKey;
extern U64 castleKeys[16];


/* ==========================================================================
 * DATA
 * ========================================================================== */

/***CONSTANTS***/
extern const char* pieceChar;
extern const char* sideChar;
extern const char* rankChar;
extern const char* fileChar;

#endif
