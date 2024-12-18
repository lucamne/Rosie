all:
	gcc rosie.c bitboard.c hashkey.c board.c data.c attack.c io.c movegen.c validate.c -o rosie.o -Wextra
