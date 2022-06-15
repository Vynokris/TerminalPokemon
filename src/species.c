#include "species.h"


Types type_str_to_enum(char* str_type)
{
    if (!strcmp(str_type, "Normal")) {
        return PT_NORMAL;
    }
    else if (!strcmp(str_type, "Fire")) {
        return PT_FIRE;
    }
    else if (!strcmp(str_type, "Water")) {
        return PT_WATER;
    }
    else if (!strcmp(str_type, "Electric")) {
        return PT_ELECTRIC;
    }
    else if (!strcmp(str_type, "Electric")) {
        return PT_ELECTRIC;
    }
    else if (!strcmp(str_type, "Grass")) {
        return PT_GRASS;
    }
    else if (!strcmp(str_type, "Ice")) {
        return PT_ICE;
    }
    else if (!strcmp(str_type, "Fighting")) {
        return PT_FIGHTING;
    }
    else if (!strcmp(str_type, "Poison")) {
        return PT_POISON;
    }
    else if (!strcmp(str_type, "Ground")) {
        return PT_GROUND;
    }
    else if (!strcmp(str_type, "Flying")) {
        return PT_FLYING;
    }
    else if (!strcmp(str_type, "Psychic")) {
        return PT_PSYCHIC;
    }
    else if (!strcmp(str_type, "Bug")) {
        return PT_BUG;
    }
    else if (!strcmp(str_type, "Rock")) {
        return PT_ROCK;
    }
    else if (!strcmp(str_type, "Ghost")) {
        return PT_GHOST;
    }
    else if (!strcmp(str_type, "Dragon")) {
        return PT_DRAGON;
    }
    else if (!strcmp(str_type, "\0")) {
        return PT_NULL;
    }
    else if (!strcmp(str_type, "Steel") || !strcmp(str_type, "Fairy")) {
        return PT_NULL;
    }
    else {
        printf("UNKNOWN TYPE NAME (src/species.c: type_str_to_enum): %s\n", str_type);
        return PT_NULL;
    }
}


ExperienceGrowth exp_str_to_enum(char* str_xp)
{
    if (!strcmp(str_xp, "Slow")) {
        return EG_SLOW;
    }
    else if (!strcmp(str_xp, "Medium Slow")) {
        return EG_MEDIUM_SLOW;
    }
    else if (!strcmp(str_xp, "Medium Fast")) {
        return EG_MEDIUM_FAST;
    }
    else if (!strcmp(str_xp, "Fast")) {
        return EG_FAST;
    }
    else {
        printf("UNKNOWN EXPERIENCE GROWTH (src/species.c: exp_str_to_enum): %s\n", str_xp);
        return EG_SLOW;
    }
}


void getGen1Species(Species* gen1Species) 
{
    PokemonDBEntry pokedex[1605];
    fill_pokedex(pokedex);

    int gen1_i = 0;
    for (int i = 0; i < 1605; i++) {
        // Find all the first generation pokemon.
        if (!strcmp(pokedex[i].GamesOfOrigin, "Red")) 
        {
            // Set the id of each gen 1 pokemon.
            gen1Species[gen1_i].id = pokedex[i].PokedexNumber;

            // Set the name of each gen 1 pokemon.
            for (int j = 0; j < 24 && pokedex[i].PokemonName[j] != '\0'; j++) {
                gen1Species[gen1_i].name[j] = toupper(pokedex[i].PokemonName[j]);
            }

            // Set the primary type of each gen 1 pokemon.
            gen1Species[gen1_i].primaryType = type_str_to_enum(pokedex[i].PrimaryType);

            // Set the secondary type of each gen 1 pokemon.
            gen1Species[gen1_i].secondaryType = type_str_to_enum(pokedex[i].SecondaryType);

            // Set the experience growth of each gen 1 pokemon.
            gen1Species[gen1_i].experienceGrowth = exp_str_to_enum(pokedex[i].ExperienceGrowth);

            // Set the health stat of each gen 1 pokemon.
            gen1Species[gen1_i].healthStat = pokedex[i].HealthStat;

            // Set the attack stat of each gen 1 pokemon.
            gen1Species[gen1_i].attackStat = pokedex[i].AttackStat;

            // Set the defense stat of each gen 1 pokemon.
            gen1Species[gen1_i].defenseStat = pokedex[i].DefenseStat;

            // Set the special attack stat of each gen 1 pokemon.
            gen1Species[gen1_i].specialAttackStat = pokedex[i].SpecialAttackStat;

            // Set the special defense stat of each gen 1 pokemon.
            gen1Species[gen1_i].specialDefenseStat = pokedex[i].SpecialDefenseStat;

            // Set the speed stat of each gen 1 pokemon.
            gen1Species[gen1_i].speedStat = pokedex[i].SpeedStat;

            // Set the experience yeild of each gen 1 pokemon.
            gen1Species[gen1_i].experienceYeild = pokedex[i].ExperienceYield;

            // Get the pokemon's evolution and evolving level if it has one.
            gen1Species[gen1_i].evolution = 0; 
            gen1Species[gen1_i].evolve_lv = 50; // Default evolving level for pokemon who don't evolve through leveling up.
            for (int j = 0; j < 1605; j++) {
                if (pokedex[j].PreEvolutionPokemonId == pokedex[i].PokemonId) {
                    gen1Species[gen1_i].evolution = pokedex[j].PokedexNumber;
                    
                    if (strcmp(pokedex[j].EvolutionDetails, "Level ") > 0) {
                        int temp_evolve_lv = atoi(&pokedex[j].EvolutionDetails[6]);
                        if (temp_evolve_lv != 0) {
                            gen1Species[gen1_i].evolve_lv = temp_evolve_lv;
                        }
                    }
                    break;
                }
            }

            // Get the pokemon's catch rate.
            gen1Species[gen1_i].catchRate = pokedex[i].CatchRate;

            // Set the learnable moves of each gen 1 pokemon.
            get_pk_move_list(gen1Species[gen1_i].learnableMoves, gen1Species[gen1_i].id);

            // Increment the gen1 iterator.
            gen1_i++;
        }
    }
}


char* get_type_by_id(int id)
{
    switch (id)
    {
        case 1:
            return "NORMAL";
        case 2:
            return "FIGHTING";
        case 3:
            return "FLYING";
        case 4:
            return "POISON";
        case 5:
            return "GROUND";
        case 6:
            return "ROCK";
        case 7:
            return "BUG";
        case 8:
            return "GHOST";
        case 10:
            return "FIRE";
        case 11:
            return "WATER";
        case 12:
            return "GRASS";
        case 13:
            return "ELECTRIC";
        case 14:
            return "PSYCHIC";
        case 15:
            return "ICE";
        case 16:
            return "DRAGON";
        default:
            printf("UNKNOWN TYPE ID (src/species.c: get_type_by_id): %d", id);
            return "NULL";
    }
}