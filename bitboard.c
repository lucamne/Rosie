#include "defs.h"
#include <stdio.h>

const int bitTable[64] = {
	63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
	51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52, 
	26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
	58, 20, 37, 17, 36, 8 
};

U64 setMask[64];
U64 clearMask[64];

int print_bitboard(U64 bb) {
	U64 mask = 1ULL;
	int sq64 = 0;

	printf("\n");
	for(int rank = RANK_8; rank >= RANK_1; rank--) {
		for(int file = FILE_A; file <= FILE_H; file++) {
			sq64 = file_and_rank_to_64(file, rank);
			if((mask << sq64) & bb) {
				printf("X");
			} else {
				printf("-");
			}
		}
		printf("\n");
	}
	printf("\n\n");

	return 0;
}

int pop_bit(U64 *bb) {
	U64 b = *bb ^ (*bb - 1);
	unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
	*bb &= (*bb - 1);
	return bitTable[(fold * 0x783a9b23) >> 26];
}

int count_bits(U64 b) {
	int r;
	for(r = 0; b ; r++, b &= b - 1);
	return r;
}

int init_bit_mask(void) {
	for(int i = 0; i < 64; i++) {
		setMask[i] = 1ULL << i;
		clearMask[i] = ~setMask[i];
	}
	return 0;
}

int set_bit(U64 *bb, int sq) {
	*bb |= setMask[sq];
	return 0;
}

int clear_bit(U64 *bb, int sq) {
	*bb &= clearMask[sq];
	return 0;
}
