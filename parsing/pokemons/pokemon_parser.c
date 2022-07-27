#include "pokemon_parser.h"


void parse_pk_csv_int(FILE* f, int** int_ptr, float** float_ptr, char** char_ptr)
{
    // Get the int value.
    int val = 0;
    fscanf(f, "%d", &val);

    // Assign the value to the entry.
    (**int_ptr) = val;

    // Increment the pointers.
    (*int_ptr)++;
    (*float_ptr) = (*int_ptr);
    (*char_ptr) = (*int_ptr);
}


void parse_pk_csv_float(FILE* f, int** int_ptr, float** float_ptr, char** char_ptr)
{
    // Get the float value.
    float val;
    fscanf(f, "%f", &val);

    // Assign the value to the entry.
    (**float_ptr) = val;

    // Increment the pointers.
    (*float_ptr)++;
    (*int_ptr) = (*float_ptr);
    (*char_ptr) = (*float_ptr);
}
 

void parse_pk_csv_str(FILE* f, int** int_ptr, float** float_ptr, char** char_ptr, int str_len)
{
    // Check if the entry is null.
    if (fgetc(f) == 'N') {
        (**char_ptr) = '\0';
    }

    // If it is not null, get its value and set it in the pokedex.
    else {
        // Skip the three " at the start of the string (the first one was already skipped by the fgetc in the if above).
        fgetc(f); fgetc(f);

        // Get the entry's value up to the end of the string.
        char val[str_len];
        fscanf(f, "%[^\"]", val);

        // Set the entry's value in the pokedex.
        for (int i = 0; val[i] != '\0'; i++) {
            (*char_ptr)[i] = val[i];
        }
    }

    // Increment the pointers.
    (*char_ptr) += str_len;
    (*int_ptr) = (*char_ptr);
    (*float_ptr) = (*char_ptr);
}


int int_in_array(int val, int* array, int arr_size)
{
    for (int i = 0; i < arr_size; i++) {
        if (array[i] == val) {
            return 1;
        }
    }
    return 0;
}


void parse_pk_csv_line(FILE* f, PokemonDBEntry* pokedex_entry, int col_num, char* format, int* str_lengths)
{
    int*   int_ptr   = (int*)  pokedex_entry;
    float* float_ptr = (float*)pokedex_entry;
    char*  char_ptr  = (char*) pokedex_entry;

    int num_of_strs = 0;
    for (int i = 0; i < col_num; i++) 
    {
        switch (format[i])
        {
            // Parse an int.
            case 'd':
                parse_pk_csv_int(f, &int_ptr, &float_ptr, &char_ptr);
                break;
            // Parse a float.
            case 'f':
                parse_pk_csv_float(f, &int_ptr, &float_ptr, &char_ptr);
                break;
            // Parse a string.
            case 's':
                parse_pk_csv_str(f, &int_ptr, &float_ptr, &char_ptr, str_lengths[num_of_strs]);
                num_of_strs++;
                break;
            // Wrong formatting character in format.
            default:
                break;
        }
        goto_next_comma_or_line(f);
    }
}


void parse_pk_csv(PokemonDBEntry* pokedex)
{
    // Open the csv file.
    FILE* f = fopen("parsing/databases/pokemons.csv", "r");

    // Format of all the columns.
    char* format = "ddsssdsffssssssssssffdssdddddddddddddddsddssdds";

    // Length of all the strings.
    int str_lengths[] = { 24, 24, 24, 16, 16, 16, 24, 192, 24, 192, 192, 128, 192, 192, 24, 92, 64, 64, 24, 128 };

    // Skip the first line since it contains the column headers.
    goto_next_line(f);

    // int offset = 0; // Used when pokemon are skipped because they are the wrong generation.
    for (int i = 0; !feof(f); i++)
    {
        parse_pk_csv_line(f, &pokedex[i], 47, format, str_lengths); // Used to be &pokedex[i-offset] when I tried to make the parser only parse 1st gen pokemon.
        
        /* // Attempt at making the parser only parse 1st gen pokemon. Doesn't work and I have no clue why.
        if (strcmp(pokedex[i - offset].GamesOfOrigin, "Red")) {
            offset++;
        }*/
    }
}


void fill_pokedex(PokemonDBEntry* pokedex)
{
    parse_pk_csv(pokedex);
}


PokemonDBEntry get_pkmn_by_id(PokemonDBEntry* pokedex, int id)
{
    for (int i = 0; i < 151; i++) {
        if (pokedex[i].PokemonId == id) {
            return pokedex[i];
        }
    }
    printf("UNKNOWN POKEMON ID: %d\n", id);
    return pokedex[0];
}