#include "defs.h"
#include <stdio.h>

int init(void) {
	init_bit_mask();
	init_hashkeys();
	return 0;
}

int file_and_rank_to_120(int f, int r) {
	return ((21 + f) + (r * 10));
}

int file_and_rank_to_64(int f, int r) {
	return ((r * 8) + f);
}

int parse_fen(char* fen, BoardState* state) {
	assert(fen);
	assert(state);

	int rank = RANK_8;
	int file = FILE_A;

	reset_board(state);

	while ((rank >= RANK_1) && *fen) {
		int count = 1;
		int piece = EMPTY;

		switch (*fen) {
			case 'p': piece = bP; break;
			case 'r': piece = bR; break;
			case 'n': piece = bN; break;
			case 'b': piece = bB; break;
			case 'k': piece = bK; break;
			case 'q': piece = bQ; break;
			case 'P': piece = wP; break;
			case 'R': piece = wR; break;
			case 'N': piece = wN; break;
			case 'B': piece = wB; break;
			case 'K': piece = wK; break;
			case 'Q': piece = wQ; break;

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				piece = EMPTY;
				count =  *fen - '0';
				break;

			// go to next rank or exit for loop
			case '/':
			case ' ':
				rank--;
				file = FILE_A;
				fen++;
				continue;

			default:
				printf("FEN error\nCurr char: %c\nCurr rank:%d\n", *fen, rank);
				return -1;
		}
		
		for (int i = 0; i < count; i++) {
			const int sq120 = file_and_rank_to_120(file, rank);
			if (piece != EMPTY) {
				state->pieces[sq120] = piece;
			}
			file++;
		}
		fen++;
	}

	assert(*fen == 'w' || *fen == 'b');

	state->sideToMove = (*fen == 'w') ? WHITE : BLACK;
	fen += 2;

	for (int i = 0; i < 4; i++) {
		if (*fen == ' ') break;
		switch (*fen) {
			case 'K': state->castlePerm |= WKCA;
				break;
			case 'Q':
				state->castlePerm |= WQCA;
				break;
			case 'k':
				state->castlePerm |= BKCA;
				break;
			case 'q':
				state->castlePerm |= BQCA;
				break;
			default:
				assert(*fen == '-');
				break;
		}
		fen++;
	}
	fen++;

	assert(state->castlePerm <= 15);
	assert(*fen != ' ');

	if (*fen != '-') {
		file = fen[0] - 'a';
		rank = fen[1] - '1';

		assert(file >= FILE_A && file <= FILE_H);
		assert(rank >= RANK_1 && rank <= RANK_8);

		state->enPassantSquare = file_and_rank_to_120(file, rank);
	}

	state->positionKey = generate_position_key(state);

	update_material_list(state);

	return 0;
}

int reset_board(BoardState *state) {
	// reset 120 board
	for (int sq = 0; sq < BRD_SQ_NUM; sq++) {
		if (SQ_120_TO_64[sq] == -1)
			state->pieces[sq] = OFFBOARD;
		else
			state->pieces[sq] = EMPTY;
	}
	// reset piece group counts and pawn bitboards
	for (int c = WHITE; c <= BOTH; c++) {
		state->bigPieceCounts[c] = 0;
		state->majorPieceCounts[c] = 0;
		state->minorPieceCounts[c] = 0;
		state->materialScores[c] = 0;
		// pawn bitboard reset
		state->pawns[c] = 0ULL;
	}
	// reset piece counts by type
	for (int p = EMPTY; p <= bK; p++) {
		state->pieceCounts[p] = 0;
	}

	state->kingSquare[WHITE] = NO_SQ;
	state->kingSquare[BLACK] = NO_SQ;
	state->sideToMove = BOTH;
	state->enPassantSquare = NO_SQ;
	state->fiftyMoveCounter = 0;
	state->ply = 0;
	state->historyPly = 0;
	state->castlePerm = 0;
	state->positionKey = 0ULL;

	return 0;
}

int print_board(const BoardState* state) {

	printf("\nGame Board:\n\n");

	for (int rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d  ", rank + 1);
		for (int file = FILE_A; file <= FILE_H; file++) {
			const int sq120 = file_and_rank_to_120(file, rank);
			const int piece = state->pieces[sq120];
			printf("%3c", PIECE_CHAR[piece]);
		}
		printf("\n");
	}
	
	printf("\n   ");
	for(int file = FILE_A; file <= FILE_H; file++) {
		printf("%3C", 'a' + file);
	}

	printf("\n\n");
	printf("side:%c\n", SIDE_CHAR[state->sideToMove]);
	printf("enPas:%d\n", state->enPassantSquare);
	printf("castle:%c%c%c%c\n",
			state->castlePerm & WKCA ? 'K' : '-',
			state->castlePerm & WQCA ? 'Q' : '-',
			state->castlePerm & BKCA ? 'k' : '-',
			state->castlePerm & BQCA ? 'q' : '-' );

	printf("Position Key:%lX\n", state->positionKey);

	return 0;
}

int update_material_list(BoardState* state) {

	for (int sq120 = 0; sq120 < BRD_SQ_NUM; sq120++) {
		int piece = state->pieces[sq120];
		int color = PIECE_COLOR[piece];
		if (piece != OFFBOARD && piece != EMPTY && color != BOTH) {

			// update piece counts and squares
			if (PIECE_BIG[piece] == true) state->bigPieceCounts[color]++;
			if (PIECE_MAJOR[piece] == true) state->majorPieceCounts[color]++;
			if (PIECE_MINOR[piece] == true) state->minorPieceCounts[color]++;

			state->materialScores[color] += PIECE_VAL[piece];

			state->pieceList[piece][state->pieceCounts[piece]] = sq120;
			state->pieceCounts[piece]++;

			if (piece == wK) state->kingSquare[WHITE] = sq120;
			else if (piece == bK) state->kingSquare[BLACK] = sq120;


			// update pawn bitboards
			if (piece == wP) {
				set_bit(&(state->pawns[WHITE]), SQ_120_TO_64[sq120]);
				set_bit(&(state->pawns[BOTH]), SQ_120_TO_64[sq120]);
			}
			else if (piece == bP) {
				set_bit(&(state->pawns[BLACK]), SQ_120_TO_64[sq120]);
				set_bit(&(state->pawns[BOTH]), SQ_120_TO_64[sq120]);
			}
		}
	}

	return 0;
}

bool check_board(const BoardState* state) {

	int _pieceCounts[PIECE_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int _bigPieceCounts[COLOR_NUM - 1] = {0, 0};
	int _majorPieceCounts[COLOR_NUM - 1] = {0, 0};
	int _minorPieceCounts[COLOR_NUM - 1] = {0, 0};
	int _materialValues[COLOR_NUM - 1] = {0, 0};
	
	U64 _pawns[COLOR_NUM] = {0ULL, 0ULL, 0ULL};
	_pawns[WHITE] = state->pawns[WHITE];
	_pawns[BLACK] = state->pawns[BLACK];
	_pawns[BOTH] = state->pawns[BOTH];

	// check that piece list and piece counts match 120 square board
	for (int piece = wP; piece <= bK; piece++) {
		for (int pieceCount = 0; pieceCount < state->pieceCounts[piece]; pieceCount++) {
			int sq120 = state->pieceList[piece][pieceCount];
			assert(state->pieces[sq120] == piece);
		}
	}

	// populate piece and material counts
	for (int sq64 = 0; sq64 < 64; sq64++) {
		int sq120 = SQ_64_TO_120[sq64];
		int piece = state->pieces[sq120];
		_pieceCounts[piece]++;
		int color = PIECE_COLOR[piece];
		
		if (color != BOTH) {
			if (PIECE_BIG[piece]) _bigPieceCounts[color]++;
			if (PIECE_MAJOR[piece]) _majorPieceCounts[color]++;
			if (PIECE_MINOR[piece]) _minorPieceCounts[color]++;

			_materialValues[color] += PIECE_VAL[piece];
		}
		
	}
	
	for (int piece = wP; piece <= bK; piece++) {
		// printf("Piece:%d\t%d\n", piece, _pieceCounts[piece]);
		assert(_pieceCounts[piece] == state->pieceCounts[piece]);
	}

	// check bitboard counts
	int pawnCount = count_bits(_pawns[WHITE]);
	assert(pawnCount == state->pieceCounts[wP]);

	pawnCount = count_bits(_pawns[BLACK]);
	assert(pawnCount == state->pieceCounts[bP]);

	pawnCount = count_bits(_pawns[BOTH]);
	assert(pawnCount == state->pieceCounts[wP] + state->pieceCounts[bP]);

	// check bitboard squares
	while (_pawns[WHITE]) {
		int sq64 = pop_bit(&_pawns[WHITE]);
		assert(state->pieces[SQ_64_TO_120[sq64]] == wP);
	}
	while (_pawns[BLACK]) {
		int sq64 = pop_bit(&_pawns[BLACK]);
		assert(state->pieces[SQ_64_TO_120[sq64]] == bP);
	}
	while (_pawns[BOTH]) {
		int sq64 = pop_bit(&_pawns[BOTH]);
		assert(state->pieces[SQ_64_TO_120[sq64]] == bP || state->pieces[SQ_64_TO_120[sq64]] == wP);
	}

	assert(_materialValues[WHITE] == state->materialScores[WHITE] && _materialValues[BLACK] == state->materialScores[BLACK]);
	assert(_minorPieceCounts[WHITE] == state->minorPieceCounts[WHITE] && _minorPieceCounts[BLACK] == state->minorPieceCounts[BLACK]);
	assert(_majorPieceCounts[WHITE] == state->majorPieceCounts[WHITE] && _majorPieceCounts[BLACK] == state->majorPieceCounts[BLACK]);
	assert(_bigPieceCounts[WHITE] == state->bigPieceCounts[WHITE] && _bigPieceCounts[BLACK] == state->bigPieceCounts[BLACK]);

	assert(state->sideToMove == WHITE || state->sideToMove);
	assert(generate_position_key(state) == state->positionKey);

	assert(state->enPassantSquare == NO_SQ || (SQ_TO_RANK[state->enPassantSquare] == RANK_6 && state->sideToMove == WHITE)
			|| (SQ_TO_RANK[state->enPassantSquare] == RANK_3 && state->sideToMove == BLACK));

	assert(state->pieces[state->kingSquare[WHITE]] == wK);
	assert(state->pieces[state->kingSquare[BLACK]] == bK);

	return true;
}


const int SQ_64_TO_120[64] = {
	21, 22, 23, 24, 25, 26, 27, 28,
	31, 32, 33, 34, 35, 36, 37, 38,
	41, 42, 43, 44, 45, 46, 47, 48,
	51, 52, 53, 54, 55, 56, 57, 58,
	61, 62, 63, 64, 65, 66, 67, 68,
	71, 72, 73, 74, 75, 76, 77, 78,
	81, 82, 83, 84, 85, 86, 87, 88,
	91, 92, 93, 94, 95, 96, 97 ,98 };


const int SQ_120_TO_64[120] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
	-1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
	-1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
	-1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
	-1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
	-1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
	-1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
	-1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };


