#include "board_rep.h"
#include <stdlib.h>

U64 pieceKeys[13][BRD_SQ_NUM];
U64 sideKey;
U64 castleKeys[16];

U64 random64(void) {
	return ((U64) random()) | (((U64) random()) << 31) | (((U64) random()) << 62);
}

int init_hashkeys(void) {
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 120; j++) {
			pieceKeys[i][j] = random64();
		}
	}
	sideKey = random64();
	for (int i = 0; i < 16; i++) {
		castleKeys[i] = random64();
	}
}

U64 generate_position_key(const BoardState const *state) {
	U64 key = 0;
	int piece = EMPTY;

	// pieces
	for (int sq = 0; sq < BRD_SQ_NUM; sq++) {
		piece = state->pieces[sq];
		if(piece != NO_SQ && piece != EMPTY)
		{
			assert(piece >= wP && piece <= bK);
			key ^= pieceKeys[piece][sq];
		}
	}

	if (state->sideToMove == WHITE) {
		key ^= sideKey;
	}

	if (state->enPassantSquare != NO_SQ)
	{
		assert(state->enPassantSquare >= 0 && state->enPassantSquare < BRD_SQ_NUM);
		key ^= pieceKeys[EMPTY][state->enPassantSquare];
	}

	assert(state->castlePerm >= 0 && state->castlePerm <= 15);
	key ^= castleKeys[state->castlePerm];

	return key;
}
