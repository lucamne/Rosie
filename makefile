all:
	gcc rosie.c bitboard.c hashkey.c board.c data.c attack.c io.c movegen.c validate.c make_move.c -o rosie.o -Wextra
