#include <stdio.h>

#include "../parsing/moves/moves_parser.h"


int main()
{
    MoveDBEntry move_list[165]; // 165 is the number of moves in the 1st generation.
    fill_move_list(&move_list);
    printf("Finished parsing the moves database.\n");
    return 0;
}