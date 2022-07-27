#include "moves_parser.h"

MoveDBEntry move_list[165] = {};


void parse_moves_csv_int(FILE* f, char** db_ptr)
{
    // Get the int value.
    int val = 0;
    fscanf(f, "%d", &val);

    // Assign the value to the entry.
    (**db_ptr) = val;

    // Increment the pointers.
    (*db_ptr) += 4;
}


void parse_moves_csv_str(FILE* f, char** db_ptr)
{
    // Get the entry's value up to the end of the string.
    char val[24];
    fscanf(f, "%[^,]", val);

    // Set the entry's value in the pokedex.
    for (int i = 0; val[i] != '\0'; i++) {
        (*db_ptr)[i] = toupper(val[i]);
    }

    // Increment the pointer.
    (*db_ptr) += 24;
}


void parse_moves_csv_line(FILE* f, MoveDBEntry* move_list, int col_num)
{
    char* db_ptr = (char*)move_list;

    for (int i = 0; i < col_num; i++) 
    {
        // Parse the second column as a string.
        if (i == 1) {
            parse_moves_csv_str(f, &db_ptr);
        }
        // Parse every other column as an int.
        else {
            parse_moves_csv_int(f, &db_ptr);
        }
        goto_next_comma_or_line(f);
    }
}


void parse_moves_csv(MoveDBEntry* move_list)
{
    // Open the csv file.
    FILE* f = fopen("parsing/databases/moves.csv", "r");

    // Skip the first line since it contains the column headers.
    goto_next_line(f);

    for (int i = 0; !feof(f) && i < 165; i++) // 165 is the number of moves in the first generation. To add more generations, make this number bigger.
    {
        parse_moves_csv_line(f, &move_list[i], 15);
    }
}


void fill_move_list(MoveDBEntry* move_list)
{
    parse_moves_csv(move_list);
}


MoveDBEntry get_move_by_id(int id)
{
    for (int i = 0; i < 165; i++) 
    {
        if (move_list[i].id == id) 
        {
            return move_list[i];
        }
    }
    printf("UNKNOWN MOVE ID: %d\n", id);
    return *move_list;
}