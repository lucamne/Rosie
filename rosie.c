#include "global_defs.h"
#include "board_rep.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{

	init();

	int piece1 = rand();
	int piece2 = rand();
	int piece3 = rand();
	int piece4 = rand();

	printf("Piece1:%X\n", piece1);
	printf("Piece1:%X\n", piece2);
	printf("Piece1:%X\n", piece3);
	printf("Piece1:%X\n", piece4);
	
	int key = piece1 ^ piece2 ^ piece3 ^ piece4;
	printf("Key:%X\n", key);

	return 0;
}
