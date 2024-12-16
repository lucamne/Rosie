#include "global_defs.h"
#include "board_rep.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{

	init();

	BoardState* state = malloc(sizeof(BoardState));
	assert(state);

	parse_fen(FEN4, state);

	print_board(state);

	printf("\nwP:\n");
	print_bitboard(state->pawns[WHITE]);
	printf("\nbP:\n");
	print_bitboard(state->pawns[BLACK]);
	printf("\nall P::\n");
	print_bitboard(state->pawns[BOTH]);

	// state->pieceCounts[wP]--;

	assert(check_board(state));

	return 0;
}
