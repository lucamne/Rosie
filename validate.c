#include "defs.h"

// use assert on the following functions to check validity of arguments

bool sq_on_board(const int sq) { return SQ_TO_FILE[sq] == FILE_NONE ? 0 : 1; }

bool side_valid(const int side) { return (side == WHITE || side == BLACK) ? 1 : 0; }

bool file_rank_valid(const int fr) { return (fr >= 0 && fr <= 7) ? 1 : 0; }

bool piece_valid_empty(const int p) { return (p >= EMPTY && p <= bK) ? 1 : 0; }
