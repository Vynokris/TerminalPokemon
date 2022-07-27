#include <stdio.h>

#include "../parsing/pokemons/pokemon_parser.h"


int main()
{
    PokemonDBEntry pokedex[1077];
    fill_pokedex(&pokedex);
    printf("Finished parsing the pokemon database.\n");
    return 0;
}