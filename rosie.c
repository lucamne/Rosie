#include "global_defs.h"
#include "board_rep.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{

	init();

	BoardState* state = malloc(sizeof(BoardState));
	assert(state);

	parse_fen(STARTING_FEN, state);
	print_board(state);
	parse_fen(FEN1, state);
	print_board(state);
	parse_fen(FEN2, state);
	print_board(state);
	parse_fen(FEN3, state);
	print_board(state);

	return 0;
}
