#include <stdio.h>

#include "../parsing/pokemon_moves/pokemon_moves_parser.h"


int main()
{
    LearnableMove move_list[24];
    get_pk_move_list(move_list, 1);
    printf("Finished parsing the pokemon moves database.\n");
    return 0;
}