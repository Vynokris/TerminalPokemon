#pragma once
#include <stdio.h>
#include <string.h>

#include "../parser_helper.h"


typedef struct PokemonDBEntry
{
    int PokemonId;
    int PokedexNumber;
    char PokemonName[24];
    char Classification[24];
    char AlternateFormName[24];
    int OriginalPokemonID;
    char LegendaryType[16];
    float PokemonHeight;
    float PokemonWeight;
    char PrimaryType[16];
    char SecondaryType[16];
    char PrimaryAbility[24];
    char PrimaryAbilityDescription[192];
    char SecondaryAbility[24];
    char SecondaryAbilityDescription[192];
    char HiddenAbility[192];
    char HiddenAbilityDescription[128];
    char SpecialEventAbility[192];
    char SpecialEventAbilityDescription[192];
    float MaleRatio;
    float FemaleRatio;
    int BaseHappiness;
    char GamesOfOrigin[24];
    char RegionOfOrigin[92];
    int HealthStat;
    int AttackStat;
    int DefenseStat;
    int SpecialAttackStat;
    int SpecialDefenseStat;
    int SpeedStat;
    int BaseStatTotal;
    int HealthEV;
    int AttackEV;
    int DefenseEV;
    int SpecialAttackEV;
    int SpecialDefenseEV;
    int SpeedEV;
    int EVYieldTotal;
    int CatchRate;
    char ExperienceGrowth[64];
    int ExperienceGrowthTotal;
    int ExperienceYield;
    char PrimaryEggGroup[64];
    char SecondaryEggGroup[24];
    int EggCycleCount;
    int PreEvolutionPokemonId;
    char EvolutionDetails[128];
} PokemonDBEntry;


// Parse an int and return it.
void parse_pk_csv_int(FILE* f, int** int_ptr, float** float_ptr, char** char_ptr);

// Parse a float and return it.
void parse_pk_csv_float(FILE* f, int** int_ptr, float** float_ptr, char** char_ptr);

// Parse a string and return it.
void parse_pk_csv_str(FILE* f, int** int_ptr, float** float_ptr, char** char_ptr, int str_len);

// Parses one csv line according to the format.
void parse_pk_csv_line(FILE* f, PokemonDBEntry* pokedex_entry, int col_num, char* format, int* str_lengths);

// Parses the whole csv and stores its values into the Pokedex.
void parse_pk_csv(PokemonDBEntry* pokedex);

// Fills the pokedex with csv data.
void fill_pokedex(PokemonDBEntry* pokedex);

// Returns the PokemonDBEntry corresponding to the id.
PokemonDBEntry get_pkmn_by_id(PokemonDBEntry* pokedex, int id);