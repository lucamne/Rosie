#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

#define PERFTFEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"



int main()
{
	init();
	
	struct BoardState* state = malloc(sizeof(struct BoardState));
	struct MoveList* mList = malloc(sizeof(struct MoveList));

	parse_fen(STARTING_FEN, state);

	// hacky user io for testing
	char input[6];
	while(1) {
		print_board(state);
		printf("Please enter a move > ");
		fgets(input, 6, stdin);

		// (q)uit, (t)ake_back, otherwise parse move
		if (input[0] == 'q') {
			break;
		} else if (input[0] == 't') {
			take_move(state);
		} else {
			const int move = parse_move(input, state);
			if (move != NO_MOVE) {
				make_move(state, move);
				/*if(is_repetition(state)) {
					printf("POSITION REPEATED\n");
				}*/
			} else {
				printf("Move not parsed: %s\n", input);
			}
		}

		fflush(stdin);
	}

	free(state);
	free(mList);
	return 0;
}
