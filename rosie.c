#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

#define PAWNMOVES "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"

int main()
{

	init();
	
	BoardState* state = malloc(sizeof(BoardState));

	parse_fen(PAWNMOVES, state);
	assert(check_board);

	print_board(state);

	MoveList* list = malloc(sizeof(MoveList));

	generate_all_moves(state, list);

	print_move_list(list);

	return 0;
}
