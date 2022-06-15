#pragma once
#include <stdio.h>
#include <string.h>

#include "../parser_helper.h"


// Structure for a learnable pokemon move. Stores id and level.
typedef struct LearnableMove {
    int id;
    int level;
} LearnableMove;

// Parses the whole csv and stores its values into the list of moves.
void parse_pk_moves_csv(LearnableMove* learnable_moves, int pk_id);

// Fills the list of moves with csv data.
void get_pk_move_list(LearnableMove* learnable_moves, int pk_id);