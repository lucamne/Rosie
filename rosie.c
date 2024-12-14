#include "global_defs.h"
#include "board_rep.h"
#include <stdio.h>

int main()
{
	U64 playBitBoard = 0b0;
	playBitBoard |= (0b1 << sq120ToSq64[D2]);
	playBitBoard |= (0b1 << sq120ToSq64[D4]);
	playBitBoard |= (0b1 << sq120ToSq64[D3]);
	print_bitboard(playBitBoard);

	int sq64 = 0;
	while (playBitBoard) {
		sq64 = pop_bit(&playBitBoard);
		printf("popped:%d\n", sq64);
	}

	return 0;
}
