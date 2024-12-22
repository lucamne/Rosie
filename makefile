all:
	gcc rosie.c bitboard.c hashkey.c board.c data.c attack.c io.c movegen.c validate.c make_move.c perft.c -o rosie.o -Wextra

debug:
	gcc rosie.c bitboard.c hashkey.c board.c data.c attack.c io.c movegen.c validate.c make_move.c perft.c -o rosie.o -Wextra -g
