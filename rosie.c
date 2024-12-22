#include "defs.h"
#include <stdio.h>
#include <stdlib.h>


int main()
{

	init();
	
	BoardState* state = malloc(sizeof(BoardState));
	MoveList* list = malloc(sizeof(MoveList));

	parse_fen(STARTING_FEN, state);
	generate_all_moves(state, list);

	print_board(state);
	getchar();

	for (int moveNum = 0; moveNum < list->count; moveNum++) {
		const int move = list->moves[moveNum].move;

		// if move is legal we skip past it
		if (!make_move(state, move)) { continue; } 
		printf("\nMADE: %s\n", print_move(move));
		print_board(state);

		take_move(state);
		printf("\nTAKEN: %s\n", print_move(move));
		print_board(state);
		
		getchar();
	}

	free(list);
	free(state);
	return 0;
}
