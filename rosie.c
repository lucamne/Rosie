#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

#define PERFTFEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPP/R3K2R w"

int main()
{

	init();
	
	BoardState* state = malloc(sizeof(BoardState));
	MoveList* list = malloc(sizeof(MoveList));

	parse_fen(STARTING_FEN, state);
	perftTest(3, state);

	free(list);
	free(state);
	return 0;
}
