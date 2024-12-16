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


/* ==========================================================================
 * TYPES
 * ========================================================================== */

/***Typedefs***/

// number of squares in our 10 x 12
#define BRD_SQ_NUM 120
#define PIECE_NUM 14
#define COLOR_NUM 3

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
	int enPassantSquare;
	int fiftyMoveCounter;
	U64 positionKey;
} PastState;

typedef struct {
	// Tracks state of each square
	int pieces[BRD_SQ_NUM];
	// pawn bitboards for white, black, and combined
	U64 pawns[COLOR_NUM];
	int kingSquare[COLOR_NUM - 1];
	int enPassantSquare;
	int sideToMove;
	int fiftyMoveCounter;
	// current search ply
	int ply;
	// total game ply
	int historyPly;

	// bitwise and of castling rights
	unsigned char castlePerm;
	U64 positionKey;
	
	// How many pieces of each type are on the board
	int pieceCounts[PIECE_NUM];
	// Counts of piece types by color
	int bigPieceCounts[COLOR_NUM];
	int majorPieceCounts[COLOR_NUM];
	int minorPieceCounts[COLOR_NUM];
	int materialScores[COLOR_NUM];

	// Piece list holds square of each piece on the board
	// 1st dimension is piece type and second dimension is instance of said piece type
	int pieceList[PIECE_NUM][10];		

	PastState history[MAX_GAME_MOVES];
} BoardState;


/* ==========================================================================
 * BOARD
 * ========================================================================== */


/***FUNCTIONS***/

// inits all data structures
int init(void);
int file_and_rank_to_120(int f, int r);
int file_and_rank_to_64(int f, int r);
int reset_board(BoardState *state);
int parse_fen(char* fen, BoardState* state);
int print_board(const BoardState* state);
int update_material_list(BoardState* state);
bool check_board(const BoardState* state);

/***Constants***/
// lookup arrays to convert between indices of 120 square and 64 square boards
extern const int SQ_64_TO_120[64];
extern const int SQ_120_TO_64[BRD_SQ_NUM];

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
// Offboard piece not used in creating hashkey, empty is used for en passant square
extern U64 pieceKeys[PIECE_NUM - 1][BRD_SQ_NUM];
extern U64 sideKey;
extern U64 castleKeys[16];


/* ==========================================================================
 * DATA
 * ========================================================================== */

/***CONSTANTS***/
extern const char* PIECE_CHAR;
extern const char* SIDE_CHAR;
extern const char* RANK_CHAR;
extern const char* FILE_CHAR;

extern const bool PIECE_BIG[PIECE_NUM];
extern const bool PIECE_MAJOR[PIECE_NUM];
extern const bool PIECE_MINOR[PIECE_NUM];
extern const int PIECE_VAL[PIECE_NUM];
extern const int PIECE_COLOR[PIECE_NUM];

extern const int SQ_TO_FILE[BRD_SQ_NUM];
extern const int SQ_TO_RANK[BRD_SQ_NUM];

#endif
