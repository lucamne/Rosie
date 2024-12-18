#include "defs.h"
#include "stdio.h"

static inline unsigned int build_move(unsigned int from, unsigned int to, unsigned int capture, 
				      unsigned int promote, unsigned int flags) {
	return from | (to << 7) | (capture << 14) | (promote << 18) | flags;
}

static inline bool sq_offboard(const int sq120) {return SQ_TO_FILE[sq120] == FILE_NONE;}

int add_quiet_move(const BoardState* state, unsigned int move, MoveList* list) {
	// list->moves is an array of structs of type Move
	// struct Move contains int move and int score 
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
	return 0;
}

int add_capture_move(const BoardState* state, unsigned int move, MoveList* list) {
	// list->moves is an array of structs of type Move
	// struct Move contains int move and int score 
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
	return 0;
}

int add_en_passant_move(const BoardState* state, unsigned int move, MoveList* list) {
	// list->moves is an array of structs of type Move
	// struct Move contains int move and int score 
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
	return 0;
}

// in between function to handle pawn promotions
int add_wP_move(const BoardState* state, const unsigned int from,
			const unsigned int to, const unsigned int capture,
			MoveList* list) {

	if(SQ_TO_RANK[from] == RANK_7) {
		add_capture_move(state, build_move(from, to, capture, wQ, 0), list);
		add_capture_move(state, build_move(from, to, capture, wR, 0), list);
		add_capture_move(state, build_move(from, to, capture, wB, 0), list);
		add_capture_move(state, build_move(from, to, capture, wN, 0), list);
	} else {
		add_capture_move(state, build_move(from, to, capture, EMPTY, 0), list);
	}
	return 0;
}

int generate_all_moves(const BoardState* state, MoveList* list) {
	assert(check_board(state));
	
	const int side = state->sideToMove;
	list->count = 0;

	if (side == WHITE) {
		for (int pieceCount = 0; pieceCount < state->pieceCounts[wP]; pieceCount++) {
			int sq120 = state->pieceList[wP][pieceCount];
			assert(!sq_offboard(sq120));	
			assert(sq120 >= 0);
			// + 10 adds 1 rank
			if (state->pieces[sq120 + 10] == EMPTY) {
				add_wP_move(state, sq120, sq120 + 10, EMPTY, list);
				// + 20 adds to ranks	
				if (SQ_TO_RANK[sq120] == RANK_2 && state->pieces[sq120 + 20] == EMPTY) {
					add_quiet_move(
							state, 
							build_move(
								sq120, 
								sq120 + 20, 
								EMPTY, 
								EMPTY, F_PAWN_START),
							list);
				}
			}
			// check for possible captures
			if (!sq_offboard(sq120 + 9) && PIECE_COLOR[state->pieces[sq120 + 9]] == BLACK) {
					add_wP_move(state, sq120, sq120 + 9, state->pieces[sq120 + 9], list);
			}
			if (!sq_offboard(sq120 + 11) && PIECE_COLOR[state->pieces[sq120 + 11]] == BLACK) {
					add_wP_move(state, sq120, sq120 + 11, state->pieces[sq120 + 11], list);
			}
			// handle en passant captures
			if (sq120 + 9 == state->enPassantSquare) {
				add_capture_move(
						state, 
						build_move(
							sq120, 
							sq120 + 9, 
							EMPTY, EMPTY, 
							F_EP_CAPTURE),
						list);
			}
			if (sq120 + 11 == state->enPassantSquare) {
				add_capture_move(
						state, 
						build_move(
							sq120, 
							sq120 + 11, 
							EMPTY, EMPTY, 
							F_EP_CAPTURE),
						list);
			}
		}
	}
	return 0;
}


