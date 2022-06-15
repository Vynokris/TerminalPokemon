#include "pokemon_moves_parser.h"


// Parses the whole csv and stores its values into the list of moves.
void parse_pk_moves_pk_moves_csv(LearnableMove* learnable_moves, int pk_id)
{
    // Open the csv file.
    FILE* f = fopen("../parsing/databases/pokemon_moves.csv", "r");

    // Skip the first line since it contains the column headers.
    goto_next_line(f);

    int found_moves = 0;
    while (!feof(f))
    {
        int read_pk_id = -1;
        fscanf(f, "%d", &read_pk_id);

        // Only take moves for the right pokemon.
        if (read_pk_id == pk_id) 
        {
            fgetc(f);
            int temp_ver_id = -1;
            fscanf(f, "%d", &temp_ver_id);

            // Only take moves from the first generation.
            if (temp_ver_id == 1) 
            {
                // Get the move id.
                fgetc(f);
                int move_id = -1;
                fscanf(f, "%d", &move_id);

                // Get the move method (how to learn it).
                fgetc(f);
                int move_method_id = -1;
                fscanf(f, "%d", &move_method_id);

                // If the move can be learned by leveling up or by hatching.
                if (move_method_id == 1 || move_method_id == 2) {
                    // Get the move level.
                    fgetc(f);
                    int level = -1;
                    fscanf(f, "%d", &level);

                    // Set the move id and level.
                    learnable_moves[found_moves].id = move_id;
                    learnable_moves[found_moves].level = level;

                    // Add 1 to the number of moves found.
                    found_moves++;
                }
            }
            else 
            {
                learnable_moves[found_moves].id = -1;
                learnable_moves[found_moves].level = -1;
                break;
            }
        }
        goto_next_line(f);
    }
}

// Fills the list of moves with csv data.
void get_pk_move_list(LearnableMove* learnable_moves, int pk_id)
{
    parse_pk_moves_pk_moves_csv(learnable_moves, pk_id);
}