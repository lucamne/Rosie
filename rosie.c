#include "defs.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{

	init();

	int move = 0;

	int from = A2;
	int to = H7;
	int captured = wR;
	int promoted = bK;

	move = from | (to << 7) | (captured << 14) | (promoted << 18);

	printf("from:%d to :%d cap:%d prom:%d\n", 
			get_from_sq(move), get_to_sq(move), get_captured_piece(move),
			get_promotion(move));

	char* s = print_sq(from);	
	printf("Algebraic from: %s\n", s);
	free(s);
	s = print_sq(to);
	printf("Algebraic to: %s\n", s); 
	free(s);
	s = print_move(move);
	printf("Algebraic move: %s\n",s);
	free(s);

	return 0;
}
