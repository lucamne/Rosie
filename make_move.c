#include "defs.h"

// state->castlePerm will be &= with CASTLE_PERM_UPDATE[from]
// This will remove appropriate castling permissions when a rook or king moves for the first time
static const int CASTLE_PERM_UPDATE[BRD_SQ_NUM] = {
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static inline void hash_piece(struct BoardState* state, int piece, int sq120) {
	state->positionKey ^= PIECE_KEYS[piece][sq120];
}

static inline void hash_castle(struct BoardState* state) {
	state->positionKey ^= CASTLE_KEYS[state->castlePerm];
}

static inline void hash_side(struct BoardState* state) {
	state->positionKey ^= SIDE_KEY;
}

static inline void hash_ep(struct BoardState* state) {
	state->positionKey ^= PIECE_KEYS[EMPTY][state->enPassantSquare];
}	

static void clear_piece(const int sq120, struct BoardState* state) {

	assert(sq_on_board(sq120));
	const int piece = state->pieces[sq120];
	assert(piece_valid_empty(piece));

	hash_piece(state, piece, sq120);

	const int color = PIECE_COLOR[piece];

	state->pieces[sq120] = EMPTY;
	state->materialScores[color] -= PIECE_VAL[piece];

	if (PIECE_BIG[piece]) {
		state->bigPieceCounts[color]--;
		if (PIECE_MINOR[piece]) {
			state->minorPieceCounts[color]--;
		} else {
			state->majorPieceCounts[color]--;
		}
	// otherwise piece is a pawn
	} else {
		clear_bit(&state->pawns[color], SQ_120_TO_64[sq120]);
		clear_bit(&state->pawns[BOTH], SQ_120_TO_64[sq120]);
	}	

#ifndef NDEBUG
	bool piece_found = false;
#endif
	// swap last entry in pieceList[piece][] to square entry fort this piece and decrement pieceCounts
	// allows us to remove entry and shorten array without doing any costly memory copies or the like
	for (int i = 0; i < state->pieceCounts[piece]; i++) {
		if (state->pieceList[piece][i] == sq120) {
#ifndef NDEBUG
			piece_found = true;
#endif
			state->pieceList[piece][i] = state->pieceList[piece][--state->pieceCounts[piece]];
			break;
		}
	}
	assert(piece_found);
}

static void add_piece(const int sq120, struct BoardState* state, const int piece) {
	assert(piece_valid_empty(piece));
	assert(sq_on_board(sq120));

	hash_piece(state, piece, sq120);

	state->pieces[sq120] = piece;

	const int color = PIECE_COLOR[piece];
	if (PIECE_BIG[piece]) {
		state->bigPieceCounts[color]++;
		if (PIECE_MINOR[piece]) {
			state->minorPieceCounts[color]++;
		} else {
			state->majorPieceCounts[color]++;
		}
	} else {
		set_bit(&state->pawns[color], SQ_120_TO_64[sq120]);
		set_bit(&state->pawns[BOTH], SQ_120_TO_64[sq120]);
	}
	state->materialScores[color] += PIECE_VAL[piece];
	state->pieceList[piece][state->pieceCounts[piece]++] = sq120;
}

static void move_piece (const int from, const int to, struct BoardState* state) {
	assert(sq_on_board(from));
	assert(sq_on_board(to));
	
	const int piece = state->pieces[from];
	const int color = PIECE_COLOR[piece];

	hash_piece(state, piece, from);
	state->pieces[from] = EMPTY;
	
	hash_piece(state, piece, to);
	state->pieces[to] = piece;

#ifndef NDEBUG
	bool piece_found = false;
#endif

	if (!PIECE_BIG[piece]) {
		clear_bit(&state->pawns[color], SQ_120_TO_64[from]);
		clear_bit(&state->pawns[BOTH], SQ_120_TO_64[from]);
		set_bit(&state->pawns[color], SQ_120_TO_64[to]);
		set_bit(&state->pawns[BOTH], SQ_120_TO_64[to]);
	}

	for (int i = 0; i < state->pieceCounts[piece]; i++) {
		if (state->pieceList[piece][i] == from) {
#ifndef NDEBUG
			piece_found = true;
#endif
			state->pieceList[piece][i] = to;
			break;
		}
	}
	assert(piece_found);
}

// return true if move is legal (does not leave king in check) and false otherwise
bool make_move(struct BoardState* state, const U32 move) {
	assert(check_board(state));

	const int from = get_from_sq(move);
	const int to = get_to_sq(move);
	const int sideToMove = state->sideToMove;

	assert(from >= 0 && sq_on_board(from));
	assert(to >= 0 && sq_on_board(to));
	assert(side_valid(sideToMove));
	assert(piece_valid_empty(state->pieces[from]) && state->pieces[from] != EMPTY);

	/* Update history */
	// important to store position key before making any board changes
	state->history[state->historyPly].positionKey = state->positionKey;	
	state->history[state->historyPly].move = move;
	state->history[state->historyPly].fiftyMoveCounter = state->fiftyMoveCounter;
	state->history[state->historyPly].enPassantSquare = state->enPassantSquare;
	state->history[state->historyPly].castlePerm = state->castlePerm;

	/* Update state members */
	if (state->enPassantSquare != NO_SQ) { hash_ep(state);}
	state->enPassantSquare = NO_SQ;
	// castle perms might change so we need to remove it from hash and the re-add after updating
	hash_castle(state);
	state->castlePerm &= CASTLE_PERM_UPDATE[from];
	state->castlePerm &= CASTLE_PERM_UPDATE[to];
	hash_castle(state);

	state->fiftyMoveCounter++;
	state->historyPly++;
	state->ply++;

	/* Handle Captures */
	const int captured = get_captured_piece(move);
	if (captured != EMPTY) {
		assert(piece_valid_empty(captured));
		// we can safely clear in case of enpassant because square was already empty
		clear_piece(to, state);
		state->fiftyMoveCounter = 0;
	}
	// en passant capture requires clearing a piece offset from the to square
	if (move & F_EP_CAPTURE) {
		if (sideToMove == WHITE) {
			clear_piece(to - 10, state);
		} else {
			clear_piece(to + 10, state);
		}
	}

	/* Handle piece moves */
	move_piece(from, to, state);
	// pawn start case	
	if (PIECE_PAWN[state->pieces[to]]) {
		state->fiftyMoveCounter = 0;
		if (move & F_PAWN_START) {
			if (sideToMove == WHITE) {
				state->enPassantSquare = from + 10;
				assert(SQ_TO_RANK[state->enPassantSquare] == RANK_3);
			} else {
				state->enPassantSquare = from - 10;
				assert(SQ_TO_RANK[state->enPassantSquare] == RANK_6);
			}
			hash_ep(state);
		}
	}
	// Rooks need to be moved as they are not represented by the to and from squares
	else if (move & F_CASTLE) {
		switch(to) {
			case C1:
				move_piece(A1, D1, state);
				break;
			case C8:
				move_piece(A8, D8, state);
				break;
			case G1:
				move_piece(H1, F1, state);
				break;
			case G8:
				move_piece(H8, F8, state);
				break;
			default:
				assert(false);
		}
	}

	// promotion requires removing a pawn and adding a new piece
	const int promotedPiece = get_promotion(move);
	if (promotedPiece != EMPTY) {
		assert(piece_valid_empty(promotedPiece) && !PIECE_PAWN[promotedPiece]);
		clear_piece(to, state);
		add_piece(to, state, promotedPiece);
	}

	if (PIECE_KING[state->pieces[to]]) {
		state->kingSquare[state->sideToMove] = to;
	}

	state->sideToMove ^= 1;
	hash_side(state);

	assert(check_board(state));

	// king cannot be in check at the end of board updates
	if (sq_attacked(state->kingSquare[sideToMove], state->sideToMove, state)) {
		take_move(state);
		return false;
	}
	return true;
}

void take_move(struct BoardState* state) {

	assert(check_board);

	state->historyPly--;
	state->ply--;

	const U32 move = state->history[state->historyPly].move;
	const int from = get_from_sq(move);
	const int to = get_to_sq(move);

	assert(sq_on_board(from));
	assert(sq_on_board(to));

	if (state->enPassantSquare != NO_SQ) { hash_ep(state); }
	hash_castle(state);

	state->castlePerm = state->history[state->historyPly].castlePerm;
	state->fiftyMoveCounter = state->history[state->historyPly].fiftyMoveCounter;
	state->enPassantSquare = state->history[state->historyPly].enPassantSquare;

	if (state->enPassantSquare != NO_SQ) { hash_ep(state); }
	hash_castle(state);

	state->sideToMove ^= 1;
	hash_side(state);

	if (F_EP_CAPTURE & move) {
		if (state->sideToMove == WHITE) {
			add_piece(to - 10, state, bP);
		} else {
			add_piece(to + 10, state, wP);
		}
	}
	// reverse castle
	else if (F_CASTLE & move) {
		switch (to) {
			case C1:
				move_piece(D1, A1, state);
				break;
			case C8:
				move_piece(D8, A8, state);
				break;
			case G1:
				move_piece(F1, H1, state);
				break;
			case G8:
				move_piece(F8, H8, state);
				break;
			default:
				assert(false);
		}
	}

	// must move piece before adding back captured piece or the capturing piece will be replaced
	move_piece(to, from, state);

	if (PIECE_KING[state->pieces[from]]) {
		state->kingSquare[state->sideToMove] = from;
	}

	const int captured = get_captured_piece(move);
	if (captured != EMPTY) {
		assert(piece_valid_empty(captured));
		add_piece(to, state, captured);
	}

	// need to replace promoted piece with a pawn
	const int promotedPiece = get_promotion(move);
	if (promotedPiece != EMPTY) {
		assert(piece_valid_empty(promotedPiece) && !PIECE_PAWN[promotedPiece]);
		clear_piece(from, state);
		add_piece(from, state, PIECE_COLOR[promotedPiece] == WHITE ? wP : bP);
	}
	
	assert(check_board(state));
}
