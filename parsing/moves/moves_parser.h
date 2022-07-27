#pragma once
#include <stdio.h>
#include <string.h>

#include "../parser_helper.h"

typedef struct MoveDBEntry 
{
    int id;
    char identifier[24];
    int generation_id;
    int type_id;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target_id;
    int damage_class_id;
    int effect_id;
    int effect_chance;
    int contest_type_id;
    int contest_effect_id;
    int super_contest_effect_id;
} MoveDBEntry;

// All the moves of generation 1.
extern MoveDBEntry move_list[165];

// Parse an int and return it.
void parse_moves_csv_int(FILE* f, char** db_ptr);

// Parse a string and return it.
void parse_moves_csv_str(FILE* f, char** db_ptr);

// Parses one csv line and sets one move entry with the data.
void parse_moves_csv_line(FILE* f, MoveDBEntry* move_list, int col_num);

// Parses the whole csv and stores its values into the list of moves.
void parse_moves_csv(MoveDBEntry* move_list);

// Fills the list of moves with csv data.
void fill_move_list(MoveDBEntry* move_list);

// Returns the MoveDBEntry corresponding to the id.
MoveDBEntry get_move_by_id(int id);