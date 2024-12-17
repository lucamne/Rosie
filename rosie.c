#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

#define FEN_TEST "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2 "

void show_sq_at_by_side(const int side, const BoardState* state) {
	int rank = 0;
	int file = 0;
	int sq = 0;

	printf("\n\nSquares attacked by:%c\n", SIDE_CHAR[side]);
	for (rank = RANK_8; rank >= RANK_1; rank--) {
		for (file = FILE_A; file <= FILE_H; file++) {
			sq = file_and_rank_to_120(file, rank);
			if (sq_attacked(sq, side, state)) {
				printf("X");
			} else {
				printf("-");
			}

		}
		printf("\n");
	}
	printf("\n\n");
}

int main()
{

	init();

	BoardState* state = malloc(sizeof(BoardState));
	assert(state);

	parse_fen(FEN_TEST, state);

	print_board(state);

	printf("White attacking\n");
	show_sq_at_by_side(WHITE, state);
	printf("Black attacking\n");
	show_sq_at_by_side(BLACK, state);




	// state->pieceCounts[wP]--;

	// assert(check_board(state));

	return 0;
}
