#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

static long leafNodes;

void perft(const int depth, BoardState* state) {

	assert(check_board(state));

	if (depth == 0) {
		leafNodes++;
		return;
	}

	MoveList* list = malloc(sizeof(MoveList));
	generate_all_moves(state, list);

	for (int moveNum = 0; moveNum < list->count; moveNum++) {
		const U32 move = list->moves[moveNum].move;
		if (!make_move(state, list->moves[moveNum].move)) {
			continue;
		}
		perft(depth - 1, state);
		take_move(state);
	}

	free(list);
	return;
}

void perftTest(const int depth, BoardState* state) {

	assert(check_board(state));
	print_board(state);

	printf("\nStarting Test to Depth: %d\n", depth);

	leafNodes = 0;

	MoveList* list = malloc(sizeof(MoveList));
	generate_all_moves(state, list);

	for (int moveNum = 0; moveNum < list->count; moveNum++) {
		const int move = list->moves[moveNum].move;
		if (!make_move(state, move)) { continue; }

		long cumnodes = leafNodes;
		perft(depth - 1, state);

		take_move(state);
		long oldnodes = leafNodes - cumnodes;
		printf("move %d : %s : %ld\n", moveNum + 1, print_move(move), oldnodes);

	}

	printf("\nTest Complete : %ld nodes visited\n", leafNodes);
	free(list);
}
