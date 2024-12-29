#include "defs.h"
#include <stdlib.h>

U64 PIECE_KEYS[PIECE_NUM][BRD_SQ_NUM];
U64 SIDE_KEY;
U64 CASTLE_KEYS[16];

U64 random64(void) {
	return ((U64) random()) | (((U64) random()) << 31) | (((U64) random()) << 62);
}

int init_hashkeys(void) {
	for (int p = EMPTY; p <= bK; p++){ 
		for (int s = 0; s < BRD_SQ_NUM; s++) {
			PIECE_KEYS[p][s] = random64();
		}
	}
	SIDE_KEY = random64();
	for (int i = 0; i < 16; i++) {
		CASTLE_KEYS[i] = random64();
	}
}

U64 generate_position_key(const struct BoardState const *state) {
	U64 key = 0;
	// pieces
	for (int sq = 0; sq < BRD_SQ_NUM; sq++) {
		int piece = state->pieces[sq];
		if(piece != EMPTY && piece != OFFBOARD)
		{
			assert(piece >= wP && piece <= bK);
			key ^= PIECE_KEYS[piece][sq];
		}
	}

	if (state->sideToMove == WHITE) {
		key ^= SIDE_KEY;
	}

	if (state->enPassantSquare != NO_SQ)
	{
		assert(state->enPassantSquare >= A1 && state->enPassantSquare <= H8);
		key ^= PIECE_KEYS[EMPTY][state->enPassantSquare];
	}

	assert(state->castlePerm <= 15u);
	key ^= CASTLE_KEYS[state->castlePerm];

	return key;
}
