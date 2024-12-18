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
