#include "defs.h"

// '?' corresponds to offboard piece, if this is printed there is an issue
const char* PIECE_CHAR = ".PNBRQKpnbrqk?";
const char* SIDE_CHAR = "wb-";
const char* RANK_CHAR = "12345678";
const char* FILE_CHAR = "abcdefgh";

const int PIECE_VAL[PIECE_NUM] = { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000 };
const int PIECE_COLOR[PIECE_NUM] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };

const int SQ_TO_FILE[BRD_SQ_NUM] = { 
	FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE,
	FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE,
	FILE_NONE, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE,
	FILE_NONE, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE,
	FILE_NONE, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE,
	FILE_NONE, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE,
	FILE_NONE, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE,
	FILE_NONE, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE,
	FILE_NONE, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE,
	FILE_NONE, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE,
	FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE,
	FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE, FILE_NONE };


const int SQ_TO_RANK[BRD_SQ_NUM] = {
	RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE,
	RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE,
	RANK_NONE, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_NONE,
	RANK_NONE, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, RANK_NONE,
	RANK_NONE, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_NONE,
	RANK_NONE, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_NONE,
	RANK_NONE, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_NONE,
	RANK_NONE, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_NONE,
	RANK_NONE, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, RANK_NONE,
	RANK_NONE, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_NONE,
	RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE,
	RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE, RANK_NONE };

const bool PIECE_BIG[PIECE_NUM] = { false, false, true, true, true, true, true, false, true, true, true, true, true };
const bool PIECE_MAJOR[PIECE_NUM] = { false, false, false, false, true, true, true, false, false, false, true, true, true };
const bool PIECE_MINOR[PIECE_NUM] = { false, false, true, true, false, false, false, false, true, true, false, false, false };

const bool PIECE_KNIGHT[PIECE_NUM] = {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
const bool PIECE_KING[PIECE_NUM] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
const bool PIECE_ROOK_QUEEN[PIECE_NUM] = {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0};
const bool PIECE_BISHOP_QUEEN[PIECE_NUM] = {0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0};

const bool PIECE_SLIDES[PIECE_NUM] = {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0};
