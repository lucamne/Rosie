#include "stdio.h"
#include "defs.h"
#include <stdlib.h>

char* print_sq(const int sq120) {
	// string is static so that output can easily be passed many times to printf
	static char sq_str[3];
	assert(sq_str);

	const int file = SQ_TO_FILE[sq120];
	const int rank = SQ_TO_RANK[sq120];

	sprintf(sq_str, "%c%c", ('a' + file), ('1' + rank));

	return sq_str;
}

char* print_move(const unsigned int move) {
	// string is static so that output can easily be passed many times to printf
	static char mv_str[6]; 
	const int ff = SQ_TO_FILE[get_from_sq(move)];
	const int rf = SQ_TO_RANK[get_from_sq(move)];
	const int ft = SQ_TO_FILE[get_to_sq(move)];
	const int rt = SQ_TO_RANK[get_to_sq(move)];

	const int promoted = get_promotion(move);

	if (promoted) {
		char pchar = 'q';
		if (PIECE_KNIGHT[promoted]) {
			pchar = 'n';
		} else if (PIECE_ROOK_QUEEN[promoted] && !PIECE_BISHOP_QUEEN[promoted]) {
			pchar = 'r';
		} else if (!PIECE_ROOK_QUEEN[promoted] && PIECE_BISHOP_QUEEN[promoted]) {
			pchar = 'b';
		}
		sprintf(mv_str, "%c%c%c%c%c", 'a' + ff, '1' + rf, 'a' + ft, '1' + rt, pchar);
	} else {
		sprintf(mv_str, "%c%c%c%c", 'a' + ff, '1' + rf, 'a' + ft, '1' + rt);
	}

	return mv_str;
}

int parse_move(char* ptrChar, BoardState* state) {

	if (ptrChar[1] > '8' || ptrChar[1] < '1') return false;
	if (ptrChar[3] > '8' || ptrChar[3] < '1') return false;
	if (ptrChar[0] > 'h' || ptrChar[0] < 'a') return false;
	if (ptrChar[2] > 'h' || ptrChar[2] < 'a') return false;

	const int from = file_and_rank_to_120(ptrChar[0] - 'a', ptrChar[1] - '1');
	const int to = file_and_rank_to_120(ptrChar[2] - 'a', ptrChar[3] - '1');
	
	assert(sq_on_board(from) && sq_on_board(to));

	MoveList* mList = malloc(sizeof(MoveList));
	generate_all_moves(state, mList);

	for (int moveNum = 0; moveNum < mList->count; moveNum++) {
		const int move = mList->moves[moveNum].move;

		if(get_from_sq(move) == from && get_to_sq(move) == to) {
			const int promPiece = get_promotion(move);

			// if the move has a malformed promotion continue to next move and likely return no moves
			if (promPiece != EMPTY) {
				if (PIECE_ROOK_QUEEN[promPiece] 
						&& !PIECE_BISHOP_QUEEN[promPiece] 
						&& ptrChar[4] == 'r') {
					return move;
				} else if (!PIECE_ROOK_QUEEN[promPiece] 
						&& PIECE_BISHOP_QUEEN[promPiece] 
						&& ptrChar[4] == 'b') {
					return move;
				} else if (PIECE_ROOK_QUEEN[promPiece] && 
						PIECE_BISHOP_QUEEN[promPiece] && 
						ptrChar[4] == 'q') {
					return move;
				} else if (PIECE_KNIGHT[promPiece] && ptrChar[4] == 'n') {
					return move;
				}
				continue;
			}
			return move;
		}
	}
	return NO_MOVE;
}

int print_move_list(const MoveList* list) {

	printf("MoveList:\n");
	for(int i = 0; i < list->count; i++) {
		const int move = list->moves[i].move;
		const int score = list->moves[i].score;

		printf("Move: %d > %s (score: %d)\n", i + 1, print_move(move), score);
	}
	printf("MoveList Total %d Moves:\n\n", list->count);

	return 0;
}
