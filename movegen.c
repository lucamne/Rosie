#include "defs.h"
#include "stdio.h"

int add_quiet_move(const BoardState* state, int move, MoveList* list) {
	// list->moves is an array of structs of type Move
	// struct Move contains int move and int score 
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
	return 0;
}

int add_capture_move(const BoardState* state, int move, MoveList* list) {
	// list->moves is an array of structs of type Move
	// struct Move contains int move and int score 
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
	return 0;
}

int add_en_passant_move(const BoardState* state, int move, MoveList* list) {
	// list->moves is an array of structs of type Move
	// struct Move contains int move and int score 
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
	return 0;
}

int generate_all_moves(const BoardState* state, MoveList* list) {
	list->count = 0;
	return 0;
}
