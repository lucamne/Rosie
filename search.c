#include "defs.h"
#include <stdio.h>


bool is_repetition(const struct BoardState* state) {
	// fiftyMoveCounter is reset on capture and pawn move which is garaunteed to be a new position
	// Therefore, we can start at the last time fiftyMoveCounter was updated
	for (int i = state->historyPly - state->fiftyMoveCounter; i < state->historyPly - 1; i++) {
		if (state->positionKey == state->history[i].positionKey) {
			return true;
		}
	}
	return false;
}

void search_position(struct BoardState* state) {
	
}
