#include "board_rep.h"
#include <stdio.h>

int init(void) {
	init_bit_mask();
	init_hashkeys();
	return 0;
}

SQ120 file_and_rank_to_120(int f, int r) {
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
		Piece piece = EMPTY;

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

	assert(state->castlePerm >= 0 && state->castlePerm <= 15);
	assert(*fen != ' ');

	if (*fen != '-') {
		file = fen[0] - 'a';
		rank = fen[1] - '1';

		assert(file >= FILE_A && file <= FILE_H);
		assert(rank >= RANK_1 && rank <= RANK_8);

		state->enPassantSquare = file_and_rank_to_120(file, rank);
	}

	state->positionKey = generate_position_key(state);

	return 0;
}

int reset_board(BoardState *state) {
	// reset 120 board
	for (int sq = 0; sq < BRD_SQ_NUM; sq++) {
		if (sq120ToSq64[sq] == -1)
			state->pieces[sq] = OFFBOARD;
		else
			state->pieces[sq] = EMPTY;
	}
	// reset piece group counts and pawn bitboards
	for (Color c = WHITE; c <= BOTH; c++) {
		state->bigPieceCounts[c] = 0;
		state->majorPieceCounts[c] = 0;
		state->minorPieceCounts[c] = 0;
		// pawn bitboard reset
		state->pawns[c] = 0ULL;
	}
	// reset piece counts by type
	for (Piece p = EMPTY; p <= bK; p++) {
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
			const Piece piece = state->pieces[sq120];
			printf("%3c", pieceChar[piece]);
		}
		printf("\n");
	}
	
	printf("\n   ");
	for(int file = FILE_A; file <= FILE_H; file++) {
		printf("%3C", 'a' + file);
	}

	printf("\n\n");
	printf("side:%c\n", sideChar[state->sideToMove]);
	printf("enPas:%d\n", state->enPassantSquare);
	printf("castle:%c%c%c%c\n",
			state->castlePerm & WKCA ? 'K' : '-',
			state->castlePerm & WQCA ? 'Q' : '-',
			state->castlePerm & BKCA ? 'k' : '-',
			state->castlePerm & WQCA ? 'q' : '-' );

	printf("Position Key:%luX\n", state->positionKey);

	return 0;
}

const int sq64ToSq120[64] = {
	21, 22, 23, 24, 25, 26, 27, 28,
	31, 32, 33, 34, 35, 36, 37, 38,
	41, 42, 43, 44, 45, 46, 47, 48,
	51, 52, 53, 54, 55, 56, 57, 58,
	61, 62, 63, 64, 65, 66, 67, 68,
	71, 72, 73, 74, 75, 76, 77, 78,
	81, 82, 83, 84, 85, 86, 87, 88,
	91, 92, 93, 94, 95, 96, 97 ,98 };


const int sq120ToSq64[120] = {
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


