#include "../parsing/pokemons/pokemon_parser.h"
#include "../parsing/pokemon_moves/pokemon_moves_parser.h"
#include <stdlib.h>

// Enum of all the different pokemon types of the first generation.
typedef enum Types {
    PT_NULL,
    PT_NORMAL,
    PT_FIGHTING,
    PT_FLYING,
    PT_POISON,
    PT_GROUND,
    PT_ROCK,
    PT_BUG,
    PT_GHOST,
    PT_STEEL, // This is a gen 2 type so it is unused, but it is necessary for the type ids to work.
    PT_FIRE,
    PT_WATER,
    PT_GRASS,
    PT_ELECTRIC,
    PT_PSYCHIC,
    PT_ICE,
    PT_DRAGON,
} Types;

// Enum of all the different experience growth patterns.
typedef enum ExperienceGrowth {
    EG_SLOW,
    EG_MEDIUM_SLOW,
    EG_MEDIUM_FAST,
    EG_FAST,
} ExperienceGrowth;

// Structure for a pokemon species. Stores id, name, types, experience growth, stats and moves.
typedef struct Species
{
    int id;
    char name[24];
    Types primaryType;
    Types secondaryType;
    ExperienceGrowth experienceGrowth;
    int healthStat;
    int attackStat;
    int defenseStat;
    int specialAttackStat;
    int specialDefenseStat;
    int speedStat;
    int experienceYeild;
    int evolution;
    int evolve_lv;
    int catchRate;
    LearnableMove learnableMoves[24];
} Species;

// Fills the given gen1Species array with data for all the pokemon species from generation 1.
void getGen1Species(Species* gen1Species);

// Returns the type corresponding to the given id.
char* get_type_by_id(int id);