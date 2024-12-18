#include "defs.h"

// direction pieces can move from to attack square
const int KN_DIR[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
const int RK_DIR[4] = {-1, -10, 1, 10}; 
const int BI_DIR[4] = {-9, -11, 11, 9}; 
const int KI_DIR[8] = {-1, -10, 1, 10, -9, -11, 11, 9};

bool sq_attacked(const int sq120, const int attacking_side, const BoardState* state) {

	assert(sq_on_board(sq120));
	assert(side_valid(attacking_side));
	assert(check_board(state));

	// check pawn attacks
	if (attacking_side == WHITE) {
		if (state->pieces[sq120 - 11] == wP || state->pieces[sq120 - 9] == wP) {
			return true;
		}

	} else {
		if (state->pieces[sq120 + 11] == bP || state->pieces[sq120 + 9] == bP) {
			return true;
		}
	}
	// check knight attacks
	for (int i = 0; i < 8; i++) {
		const int piece = state->pieces[sq120 + KN_DIR[i]];
		if (PIECE_KNIGHT[piece] && PIECE_COLOR[piece] == attacking_side) {
			return true;
		}
	}
	// check rook and queen attacks
	for (int i = 0; i < 4; i++) {
		const int offset = RK_DIR[i];
		int t_sq = sq120 + offset;
		int piece = state->pieces[t_sq];

		while (piece != OFFBOARD) {
			if (piece != EMPTY) {
				if (PIECE_ROOK_QUEEN[piece] && PIECE_COLOR[piece] == attacking_side) {
					return true;
				}
				// we can break because there is some other piece in the way
				break;
			}
			t_sq += offset;
			piece = state->pieces[t_sq];
		}
	}
	// check bishops and queens
	for (int i = 0; i < 4; i++) {
		const int offset = BI_DIR[i];
		int t_sq = sq120 + offset;
		int piece = state->pieces[t_sq];

		while (piece != OFFBOARD) {
			if (piece != EMPTY) {
				if (PIECE_BISHOP_QUEEN[piece] && PIECE_COLOR[piece] == attacking_side) {
					return true;
				}
				// we can break because there is some other piece in the way
				break;
			}
			t_sq += offset;
			piece = state->pieces[t_sq];
		}
	}
	// check king attacks
	for (int i = 0; i < 4; i++) {
		const int piece = state->pieces[sq120 + KI_DIR[i]];
		if (PIECE_KING[piece] && PIECE_COLOR[piece] == attacking_side) {
			return true;
		}
	}

	return false;
}
