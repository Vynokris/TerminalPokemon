#include "game_functions.h"

// --------------- GLOBAL VARIABLES --------------- //
// Type table ordered like the Types enum (Null (Unused), Normal, Fighting, Flying, Poison, Ground, Rock, Bug, Ghost, Steel (Unused), Fire, Water, Grass, Electric, Psychic, Ice, Dragon).
float typeTable[17][17] = 
{
    { 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1   },
    { 1  , 1  , 1  , 1  , 1  , 1  , 0.5, 1  , 0  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1   },
    { 1  , 2  , 1  , 0.5, 0.5, 1  , 2  , 0.5, 0  , 1  , 1  , 1  , 1  , 1  , 0.5, 2  , 1   },
    { 1  , 1  , 2  , 1  , 1  , 1  , 0.5, 2  , 1  , 1  , 1  , 1  , 2  , 0.5, 1  , 1  , 1   },
    { 1  , 1  , 1  , 1  , 0.5, 0.5, 0.5, 1  , 0.5, 1  , 1  , 1  , 2  , 1  , 1  , 1  , 1   },
    { 1  , 1  , 1  , 0  , 2  , 1  , 2  , 0.5, 1  , 1  , 2  , 1  , 0.5, 2  , 1  , 1  , 1   },
    { 1  , 1  , 0.5, 2  , 1  , 0.5, 1  , 2  , 1  , 1  , 2  , 1  , 1  , 1  , 1  , 2  , 1   },
    { 1  , 1  , 0.5, 0.5, 0.5, 1  , 1  , 1  , 0.5, 1  , 0.5, 1  , 2  , 1  , 2  , 1  , 1   },
    { 1  , 0  , 1  , 1  , 1  , 1  , 1  , 1  , 2  , 1  , 1  , 1  , 1  , 1  , 2  , 1  , 1   },
    { 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1   },
    { 1  , 1  , 1  , 1  , 1  , 1  , 0.5, 2  , 1  , 1  , 0.5, 0.5, 2  , 1  , 1  , 2  , 0.5 },
    { 1  , 1  , 1  , 1  , 1  , 2  , 2  , 1  , 1  , 1  , 2  , 0.5, 0.5, 1  , 1  , 1  , 0.5 },
    { 1  , 1  , 1  , 0.5, 0.5, 2  , 2  , 0.5, 1  , 1  , 0.5, 2  , 0.5, 1  , 1  , 1  , 0.5 },
    { 1  , 1  , 1  , 2  , 1  , 0  , 1  , 1  , 1  , 1  , 1  , 2  , 0.5, 0.5, 1  , 1  , 0.5 },
    { 1  , 1  , 2  , 1  , 2  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 0.5, 1  , 1   },
    { 1  , 1  , 1  , 2  , 1  , 2  , 1  , 1  , 1  , 1  , 0.5, 0.5, 2  , 1  , 1  , 0.5, 2   },
    { 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 1  , 2   },
};


// --------------- UTILITY FUNCTIONS --------------- //

void get_gen1Species()
{
    // Read the pokemon species from gen 1 from the gen1Species.bin file.
    FILE* f = fopen("../parsing/bin/gen1Species.bin", "rb");
    fread(&gen1Species, sizeof(gen1Species[0]), 151, f);
    fclose(f);
}

void get_move_list()
{
    // Read the pokemon moves from gen 1 from the move_list.bin file.
    FILE* f = fopen("../parsing/bin/move_list.bin", "rb");
    fread(&move_list, sizeof(move_list[0]), 165, f);
    fclose(f);
}

Species get_species_by_id(int id)
{
    for (int i = 0; i < 151; i++) {
        if (gen1Species[i].id == id) {
            return gen1Species[i];
        }
    }
    printf("UNKNOWN SPECIES ID (src/game_functions.c: get_species_by_id): %d\n", id);
    return gen1Species[0];
}

Species get_species_by_name(char* name)
{
    for (int i = 0; i < 151; i++) {
        if (!strcmp(gen1Species[i].name, name)) {
            return gen1Species[i];
        }
    }
    printf("UNKNOWN SPECIES NAME (src/game_functions.c: get_species_by_name): %s\n", name);
    return gen1Species[0];
}

int get_move_damage(Pokemon* attack_pk, Pokemon* defense_pk, int chosen_move)
{
    // Check if the move landed.
    if (rand()%100 > get_move_by_id(attack_pk->moves[chosen_move].id).accuracy) {
        return 0;
    }

    // Check if the move is critical.
    bool critical = false;
    float critical_chance = attack_pk->speed / 2;
    int critical_random = rand() % 256;
    if (critical_random < critical_chance) {
        critical = true;
    }

    // Get the attacking pokemon's level.
    int level = attack_pk->lv;
    if (critical) {
        level *= 2;
    }

    // Get the move's power.
    int power = get_move_by_id(attack_pk->moves[chosen_move].id).power;

    // Get the attacking pokemon's attack stat (physical or special).
    int attack_stat = 0;
    switch (get_move_by_id(attack_pk->moves[chosen_move].id).damage_class_id)
    {
        case 1:
            attack_stat = 0;
        case 2:
            attack_stat = attack_pk->attack;
            break;
        case 3:
            attack_stat = attack_pk->spe_attack;
            break;
    }

    // Get the defending pokemon's defense stat (physical or special).
    int defense_stat = 0;
    switch (get_move_by_id(attack_pk->moves[chosen_move].id).damage_class_id)
    {
        case 1:
            defense_stat = 1;
        case 2:
            defense_stat = defense_pk->defense;
            break;
        case 3:
            defense_stat = defense_pk->spe_defense;
            break;
    }

    // Get the random part of the move damage.
    float random = (float)(rand() % 39 + 217) / 255;

    // Check if the move was stab (same type as attacking pokemon).
    float stab = 1.0f;
    if (get_species_by_id(attack_pk->id).primaryType == get_move_by_id(attack_pk->moves[chosen_move].id).type_id
     || get_species_by_id(attack_pk->id).secondaryType == get_move_by_id(attack_pk->moves[chosen_move].id).type_id)
    {
        stab = 1.5f;
    }

    // Check if the move's type effectiveness on the defending pokemon.
    float type_effectiveness = typeTable[get_move_by_id(attack_pk->moves[chosen_move].id).type_id][get_species_by_id(defense_pk->id).primaryType]
                             * typeTable[get_move_by_id(attack_pk->moves[chosen_move].id).type_id][get_species_by_id(defense_pk->id).secondaryType];
    

    // Return the move's damage value.
    int damage = 0;
    if (get_move_by_id(attack_pk->moves[chosen_move].id).damage_class_id != 1) {
        damage = ((((2 * level) / 5 + 2) * power * (attack_stat / defense_stat)) / 50 + 2) * random * stab * type_effectiveness; // https://bulbapedia.bulbagarden.net/wiki/Damage#Damage_calculation
    }
    return damage;
}

int get_earned_exp(Pokemon* winner_pk, Pokemon* fainted_pk, bool trainer_battle)
{
    // Get the trainer battle parameter. Set it to 1 for wild battles and to 1.5 for trainer battles.
    float trainer_battle_param = 1.0f;
    if (trainer_battle) {
        trainer_battle_param = 1.5f;
    }

    // Get the base experience yield of the fainted Pokémon's species.
    int base_exp_yeild = get_species_by_id(fainted_pk->id).experienceYeild;

    // Get the level of the fainted pokemon (L).
    int fainted_level = fainted_pk->lv;

    // Calculate the exp gain.
    int exp = (trainer_battle_param * base_exp_yeild * fainted_level) / 7; // https://bulbapedia.bulbagarden.net/wiki/Experience#Experience_gain_in_battle
    return exp;
}

bool update_pk_lv(Pokemon* pk)
{
    // Get the exp necessary for the next level.
    int next_lv_exp = 0;
    int level = pk->lv + 1;

    switch (get_species_by_id(pk->id).experienceGrowth)
    {
        case EG_FAST:
            next_lv_exp = ceil((4 * level * level * level ) / 5); // (4*lv^3)/5
            break;
        case EG_MEDIUM_FAST:
            next_lv_exp = level * level * level; // lv^3
            break;
        case EG_MEDIUM_SLOW:
            next_lv_exp = ceil((6/5) * level * level * level - 15 * level * level + 100 * level - 140); // (6/5)*lv^3 - 15*lv^2 + 100 * lv - 140
            break;
        case EG_SLOW:
            next_lv_exp = ceil((5 * level * level * level) / 4); // (5*lv^3)/4
            break;
    }

    // Check if the pokemon has more exp than required to level up. Make him level up if he has.
    if (pk->exp >= next_lv_exp) {
        pk->lv++;
        printf("%s grew to level %d!\n", get_species_by_id(pk->id).name, pk->lv);

        // Wait for player input to level up.
        printf("%sPress [enter] to continue.", C_GRAY);
        getchar();
        printf("%s\n", C_DEFAULT);


        // Search for any move the pokemon could learn by leveling up.
        for (int i = 0; i < 24; i++) {
            if (get_species_by_id(pk->id).learnableMoves[i].level <= pk->lv
                && get_species_by_id(pk->id).learnableMoves[i].level > pk->lv-1) 
            {
                // Wait for player input to learn the move.
                printf("%sPress [enter] to continue.", C_GRAY);
                getchar();
                printf("%s\n", C_DEFAULT);

                learn_move(pk, get_species_by_id(pk->id).learnableMoves[i].id);
            }
        }

        // Check if the pokemon should evolve and make him evolve if he should.
        if (get_species_by_id(pk->id).evolve_lv <= pk->lv) 
        {
            // Wait for player input to evolve.
            printf("%sPress [enter] to continue.", C_GRAY);
            getchar();
            printf("%s\n", C_DEFAULT);

            // Notify the player about the evolution.
            printf("What? %s is evolving!\n", get_species_by_id(pk->id).name);

            // Let the player choose wether the pokemon should evolve or not.
            printf("%sLet %s evolve (y/n)? ", C_GRAY, get_species_by_id(pk->id).name);
            char player_choice = getchar();
            getchar();
            printf("%s\n", C_DEFAULT);

            if (player_choice == 'n' || player_choice == 'N') {
                return true;
            }

            printf("%s evolved into ", get_species_by_id(pk->id).name);

            // Create an instance of the pokemon's evolution.
            Pokemon temp_evolution_pk;
            Species temp_evolution_species = get_species_by_id(get_species_by_id(pk->id).evolution);
            init_pk_instance(&temp_evolution_pk, &temp_evolution_species, pk->lv);

            // Carry the pokemon's IVs.
            temp_evolution_pk.hp_iv = pk->hp_iv;
            temp_evolution_pk.attack_iv = pk->attack_iv;
            temp_evolution_pk.defense_iv = pk->defense_iv;
            temp_evolution_pk.speed_iv = pk->speed_iv;
            temp_evolution_pk.special_iv = pk->special_iv;
            temp_evolution_pk.shiny = pk->shiny;

            // Make the pokemon evolve.
            *pk = temp_evolution_pk;

            // Notify the user about the new pokemon.
            printf("%s!\n", get_species_by_id(pk->id).name);
        }
        return true;
    }
    return false;
}

void learn_move(Pokemon* pk, int move_id)
{
    // Set to true if the move has been learned or learning has been abandoned.
    bool learned = false;

    // Check if the pokemon has a free attack slot.
    for (int i = 0; i < 4; i++) {
        if (pk->moves[i].id == 0) {
            pk->moves[i].id = move_id;
            pk->moves[i].current_pp = get_move_by_id(move_id).pp;
            printf("%s learned %s!\n", get_species_by_id(pk->id).name, get_move_by_id(move_id).identifier);
            learned = true;
            break;
        }
    }

    while (!learned)
    {
        printf("%s is trying to learn %s!\n", get_species_by_id(pk->id).name, get_move_by_id(move_id).identifier);
        printf("But, %s can't learn more than 4 moves!\n", get_species_by_id(pk->id).name);
        printf("%sDelete an older move to make room for %s (y/n)? ", C_GRAY, get_move_by_id(move_id).identifier);

        char player_choice = '0';
        scanf("%c", &player_choice);
        getchar();

        if (player_choice == 'y' || player_choice == 'Y') 
        {
            printf("%s\n\n", C_DEFAULT);

            for (int i = 0; i < 4; i++) {
                printf("%d: %s (Type: %s, Power: %d, PP: %d, %d)\n", 
                       i+1, 
                       get_move_by_id(pk->moves[i].id).identifier,
                       get_type_by_id(get_move_by_id(pk->moves[i].id).type_id),
                       get_move_by_id(pk->moves[i].id).power,
                       get_move_by_id(pk->moves[i].id).pp,
                       get_move_by_id(pk->moves[i].id).damage_class_id);
            }
            printf("5: %s (Type: %s, Power: %d, PP: %d, %d)\n", 
                   get_move_by_id(move_id).identifier,
                   get_type_by_id(get_move_by_id(move_id).type_id),
                   get_move_by_id(move_id).power,
                   get_move_by_id(move_id).pp,
                   get_move_by_id(move_id).damage_class_id);
                   
            printf("%sWhich move should be forgotten? ", C_GRAY);
            int deleted_move = 0;
            scanf("%d", &deleted_move);
            getchar();

            if (player_choice >= 1 && player_choice <= 4) {
                printf("Forget %s (y/n)? ", get_move_by_id(pk->moves[deleted_move-1].id).identifier);
                scanf("%c", &player_choice);
                getchar();
                printf("%s\n\n", C_DEFAULT);

                if (player_choice == 'y' || player_choice == 'Y') 
                {
                    printf("1, 2 and...\nPoof!\n");
                    printf("%s forgot %s!\n", get_species_by_id(pk->id).name, get_move_by_id(pk->moves[deleted_move-1].id).identifier);

                    pk->moves[deleted_move-1].id = move_id;
                    pk->moves[deleted_move-1].current_pp = get_move_by_id(move_id).pp;

                    printf("And...\n");
                    printf("%s learned %s!\n", get_species_by_id(pk->id).name, get_move_by_id(move_id).identifier);
                    learned = true;
                }
            }
            else if (player_choice == 5) {
                printf("Abandon learning %s (y/n)? ", get_move_by_id(move_id).identifier);
                scanf("%c", &player_choice);
                getchar();
                printf("%s\n\n", C_DEFAULT);

                if (player_choice == 'y' || player_choice == 'Y') {
                    learned = true;
                    printf("%s did not learn %s!", get_species_by_id(pk->id).name, get_move_by_id(move_id).identifier);
                }
            }
        }

        else if (player_choice == 'n' || player_choice == 'N') 
        {
            printf("Abandon learning %s (y/n)? ", get_move_by_id(move_id).identifier);
            scanf("%c", &player_choice);
            getchar();
            printf("%s\n\n", C_DEFAULT);

            if (player_choice == 'y' || player_choice == 'Y') {
                learned = true;
                printf("%s did not learn %s!", get_species_by_id(pk->id).name, get_move_by_id(move_id).identifier);
            }
        }
    }
}

bool capture_pk(Pokemon* wild_pk, int ball_type, Item* ball_item)
{
    // See https://bulbapedia.bulbagarden.net/wiki/Catch_rate#Capture_method_.28Generation_I.29 for more info on pokemon catching.

    printf("You used a %s!\n", ball_item->name);
    ball_item->count--;

    // Wait for player input to continue.
    printf("%sPress [enter] to continue.", C_GRAY);
    getchar();
    printf("%s", C_DEFAULT);

    // Generate a random number (n) depending on the type of ball used.
    int n;
    switch (ball_type)
    {
        // For a Poké Ball: 0 to 255.
        case 1:
            n = rand() % 256;
            break;
        
        // For a Great Ball: 0 to 200.
        case 2:
            n = rand() % 201;
            break;
        
        // For an Ultra Ball: 0 to 150.
        case 3:
            n = rand() % 151;
            break;
    }

    // If n minus the status threshold (12) is greater than the Pokémon's catch rate, the Pokémon breaks free.
    if (n - 12 > get_species_by_id(wild_pk->id).catchRate) {
        printf("Oh no! %s broke free!\n\n", get_species_by_id(wild_pk->id).name);
        return false;
    }

    // If not, generate a random value, M, between 0 and 255.
    else {
        int m = rand() % 256;

        // Calculate f: (max_hp * 255 * 4) / (current_hp * ball) with ball = 8 if a great ball is used and ball = 12 if another ball is used. 1 <= f <= 255.
        int ball = 12;
        if (ball_type == 2) {
            ball = 8;
        }
        int f = (wild_pk->max_hp * 255 * 4) / (wild_pk->hp * ball);

        // If f is greater than or equal to M, the Pokémon is caught.
        if (f >= m) {
            printf("All right! %s was caught!\n\n", get_species_by_id(wild_pk->id).name);

            // Try to fit it into the player's team.
            for (int i = 0; i < 6; i++) {
                if (player_team[i].id == 0) {
                    player_team[i] = *wild_pk;
                    printf("%s is now part of your pokemon team!\n", get_species_by_id(wild_pk->id).name);
                    return true;
                }
            }

            // If the player's team is full, let him choose which pokemon to release.
            printf("Your team is full!\n");
            int pkmn_choice = -1;
            while (pkmn_choice < 1 || pkmn_choice > 7) {
                // Show the name and level of each of the player's pokemon.
                for (int i = 0; i < 6; i++)
                {
                    if (player_team[i].id > 0) {
                        printf("%d: %s (LV: %d)\n", i+1, get_species_by_id(player_team[i].id).name, player_team[i].lv);
                    }
                    else {
                        printf("%d: -\n", i+1);
                    }
                }
                printf("7: CANCEL\n");

                // Let the player choose which pokemon to release.
                printf("%sWhich pokemon should be released to make room for %s? ", C_GRAY, get_species_by_id(wild_pk->id).name);
                scanf("%d", &pkmn_choice);
                getchar();
                printf("%s\n\n", C_DEFAULT);

                // Let the player only choose slots where there are pokemons in his team.
                if (pkmn_choice >= 1 && pkmn_choice <= 6) {
                    printf("%s was released and %s joined your pokemon team!\n", get_species_by_id(player_team[pkmn_choice-1].id).name, get_species_by_id(wild_pk->id).name);
                    player_team[pkmn_choice-1] = *wild_pk;
                    return true;
                }
                else if (pkmn_choice == 7) {
                    return false;
                }
            }
        }
        
        // Otherwise, the Pokémon breaks free.
        else {
            printf("Oh no! %s broke free!\n\n", get_species_by_id(wild_pk->id).name);
            return false;
        }
    }
    return false;
}

void use_item(int item_type, Item* item, Pokemon* pk)
{
    int recovered_hp;
    switch (item_type)
    {
        // Potion.
        case 4:
            pk->hp += 20;
            recovered_hp = 20;
            if (pk->hp > pk->max_hp) {
                recovered_hp = pk->max_hp - (pk->hp - 20);
                pk->hp = pk->max_hp;
            }
            printf("Used a POTION on %s!\n%s recovered by %d!\n", get_species_by_id(pk->id).name, get_species_by_id(pk->id).name, recovered_hp);
            break;
        
        // Super potion.
        case 5:
            pk->hp += 50;
            recovered_hp = 50;
            if (pk->hp > pk->max_hp) {
                recovered_hp = pk->max_hp - (pk->hp - 50);
                pk->hp = pk->max_hp;
            }
            printf("Used a SUPER POTION on %s!\n%s recovered by %d!\n", get_species_by_id(pk->id).name, get_species_by_id(pk->id).name, recovered_hp);
            break;
        
        // Hyper potion.
        case 6:
            pk->hp += 200;
            recovered_hp = 200;
            if (pk->hp > pk->max_hp) {
                recovered_hp = pk->max_hp - (pk->hp - 200);
                pk->hp = pk->max_hp;
            }
            printf("Used a HYPER POTION on %s!\n%s recovered by %d!\n", get_species_by_id(pk->id).name, get_species_by_id(pk->id).name, recovered_hp);
            break;
        
        // Revive.
        case 7:
            pk->hp = pk->max_hp/2;
            printf("Used a REVIVE on %s!\nHalf of %s's health returned!\n", get_species_by_id(pk->id).name, get_species_by_id(pk->id).name);
            break;
        
        // Max revive.
        case 8:
            pk->hp = pk->max_hp;
            printf("Used a MAX REVIVE on %s!\n%s's health returned!\n", get_species_by_id(pk->id).name, get_species_by_id(pk->id).name);
            break;
    }
    item->count--;

    // Wait for player action to continue.
    printf("%sPress [enter] to continue.", C_GRAY);
    getchar();
    printf("%s\n\n", C_DEFAULT);
}


// --------------- SETUP FUNCTIONS --------------- //

void init_pk_instance(Pokemon* pk_instance, Species* species, int level)
{
    // ----- ID ----- //
    // Set the pokemon's id.
    pk_instance->id = species->id;


    // ----- LEVEL & EXP ----- //
    // Set the pokemon's level.
    pk_instance->lv = level;

    // Set the pokemon's exp.
    switch (species->experienceGrowth)
    {
        case EG_FAST:
            pk_instance->exp = ceil((4 * level * level * level ) / 5); // (4*lv^3)/5
            break;
        case EG_MEDIUM_FAST:
            pk_instance->exp = level * level * level; // lv^3
            break;
        case EG_MEDIUM_SLOW:
            pk_instance->exp = ceil((6/5) * level * level * level - 15 * level * level + 100 * level - 140); // (6/5)*lv^3 - 15*lv^2 + 100 * lv - 140
            break;
        case EG_SLOW:
            pk_instance->exp = ceil((5 * level * level * level) / 4); // (5*lv^3)/4
            break;
    }


    // ----- IVs ----- //
    // See https://bulbapedia.bulbagarden.net/wiki/Individual_values#Generation_I_and_II for more info on IV calculation.

    // Set the pokemon's attack IV.
    pk_instance->attack_iv = rand()%16;

    // Set the pokemon's defense IV.
    pk_instance->defense_iv = rand()%16;

    // Set the pokemon's speed IV.
    pk_instance->speed_iv = rand()%16;

    // Set the pokemon's special IV.
    pk_instance->special_iv = rand()%16;

    // Set the pokemon's hp IV.
    pk_instance->hp_iv = 0;
    int* iv_ptr = &pk_instance->special_iv;
    for (int i = 0; i < 4; i++) {
        pk_instance->hp_iv += (*iv_ptr)%2 * (int)(pow(2, i));
        iv_ptr--;
    }


    // ----- STATS ----- //
    // See https://bulbapedia.bulbagarden.net/wiki/Stat#Formula for more info on Stat calculation.

    // Set the pokemon's hp.
    pk_instance->hp = floor(((species->healthStat + pk_instance->hp_iv) * 2 * level) / 100 + level + 10);

    // Set the pokemon's max hp.
    pk_instance->max_hp = pk_instance->hp;

    // Set the pokemon's attack stat.
    pk_instance->attack = floor(((species->attackStat + pk_instance->attack_iv) * 2 * level) / 100 + 5);

    // Set the pokemon's defense stat.
    pk_instance->defense = floor(((species->defenseStat + pk_instance->defense_iv) * 2 * level) / 100 + 5);

    // Set the pokemon's special attack stat.
    pk_instance->spe_attack = floor(((species->specialAttackStat + pk_instance->special_iv) * 2 * level) / 100 + 5);

    // Set the pokemon's special defense stat.
    pk_instance->spe_defense = floor(((species->specialDefenseStat + pk_instance->special_iv) * 2 * level) / 100 + 5);

    // Set the pokemon's speed stat.
    pk_instance->speed = floor(((species->speedStat + pk_instance->speed_iv) * 2 * level) / 100 + 5);


    // ----- SHININESS ----- //
    // See https://bulbapedia.bulbagarden.net/wiki/Shiny_Pok%C3%A9mon#Determining_Shininess for more information on how shiny pokemon appear.

    // Make the pokemon shiny if his speed, defense and special IVs are 10 and if its attack IV is 2, 3, 6, 7, 10, 11, 14 or 15.
    if (pk_instance->speed_iv == 10 
     && pk_instance->defense_iv == 10 
     && pk_instance->special_iv == 10 
     &&   (pk_instance->attack_iv == 2 
        || pk_instance->attack_iv == 3 
        || pk_instance->attack_iv == 6 
        || pk_instance->attack_iv == 7 
        || pk_instance->attack_iv == 10 
        || pk_instance->attack_iv == 11 
        || pk_instance->attack_iv == 14 
        || pk_instance->attack_iv == 15)
    ) {
        pk_instance->shiny = true;
    }
    else {
        pk_instance->shiny = false;
    }


    // ----- MOVES ----- //
    // Get the pokemon's moves.
    int pk_moves_i[4] = { -1, -1, -1, -1 };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; species->learnableMoves[j].id != -1; j++) 
        {
            if (level > species->learnableMoves[j].level && // Make sure the pokemon is high level enough to learn this attack.
               (pk_moves_i[i] == -1 || // If the move hasn't been initialized yet.
                species->learnableMoves[pk_moves_i[i]].level <= species->learnableMoves[j].level)) // Make sure the previous set move_i isn't higher level than this one.
            {
                // Make sure this move isn't set as one of the pokemon's other moves.
                bool attack_already_set = false;
                for (int k = 0; k < 4; k++) {
                    if (i != k && j == pk_moves_i[k]) {
                        attack_already_set = true;
                    }
                }
                // Set the move if it isn't already.
                if (!attack_already_set) {
                    pk_moves_i[i] = j;
                }
            }
        }
    }

    // Assign each of the 4 moves to the pokemon.
    for (int i = 0; i < 4; i++) {
        if (pk_moves_i[i] != -1) {
            pk_instance->moves[i].id = species->learnableMoves[pk_moves_i[i]].id;
            pk_instance->moves[i].current_pp = get_move_by_id(pk_instance->moves[i].id).pp;
        }
        else {
            pk_instance->moves[i].id = 0;
            pk_instance->moves[i].current_pp = 0;
        }
    }
}

void get_player_team()
{
    // Read the player's team from team.bin.
    FILE* f = fopen("../player_data/team.bin", "rb");
    fread(&player_team, sizeof(player_team[0]), 6, f);
    fclose(f);
    
    // Old way to do it with a csv file.
    /*
    FILE* f = fopen("../player_data/team.csv", "r");

    goto_next_line(f);

    // Read each of the pokemons in the player's team.
    int i = 0;
    while (!feof(f)) 
    {
        // Get and set each of the current pokemon's saved stats.
        int saved_stat;
        int* stat_ptr = &player_team[i];
        for (int j = 0; j < 16; j++)
        {
            fscanf(f, "%d", &saved_stat);
            fgetc(f);
            *stat_ptr = saved_stat;
            stat_ptr++;
        }

        // Get and set each of the current pokemon's moves.
        for (int j = 0; j < 4; j++) {
            int current_move_id;
            fscanf(f, "%d", &current_move_id);
            fgetc(f);
            player_team[i].moves[j].id = current_move_id;
            if (current_move_id != 0) {
                player_team[i].moves[j].current_pp = get_move_by_id(current_move_id).pp;
            }
            else {
                player_team[i].moves[j].current_pp = 0;
            }
        }

        // Increment the iterator.
        i++;
    }

    // Set the first empty pokemon slot to id 0.
    while (i < 6) {
        player_team[i].id = 0;
        i++;
    }

    fclose(f);
    */
}

void save_player_team()
{
    // Save the player's team in a binary file.
    FILE* f = fopen("../player_data/team.bin", "wb");
    fwrite(&player_team, sizeof(player_team[0]), 6, f);
    fclose(f);

    // Old way to do it with a csv file.
    /*
    // Get the first line of the team.csv file (the one that contains all the headers).
    f = fopen("../player_data/team.csv", "r");
    char* first_team_csv_line = calloc(256, sizeof(char));
    fgets(first_team_csv_line, 256, f);

    // Wipe the team.csv and add the header line.
    f = fopen("../player_data/team.csv", "w");
    fprintf(f, "%s", first_team_csv_line);
    free(first_team_csv_line);

    // Save each of the player's pokemons.
    for (int i = 0; i < 6 && player_team[i].id != 0; i++) 
    {
        // Save the pokemon's stats.
        int* stat_ptr = &player_team[i].id;
        for (int j = 0; j < 15; j++) {
            // Save the current stat.
            fprintf(f, "%d, ", *stat_ptr);

            // Move to the next stat.
            stat_ptr++;
        }

        // Save the pokemon's shininess.
        fprintf(f, "%d, ", player_team[i].shiny);

        // Save the pokemon's moves.
        for (int j = 0; j < 4; j++) {
            // Save the current move id.
            if (j == 3) {
                if (i + 1 >= 6 || player_team[i+1].id == 0) {
                    fprintf(f, "%d", player_team[i].moves[j].id);
                }
                else {
                    fprintf(f, "%d\n", player_team[i].moves[j].id);
                }
            }
            else {
                fprintf(f, "%d, ", player_team[i].moves[j].id);
            }
        }
    }
    */
}

void player_team_options()
{
    printf("1: Load team\n2: Modify team\n3: New team\n");
    int chosen_team_option;
    printf("%sYour choice: ", C_GRAY);
    scanf("%d", &chosen_team_option);
    getchar();
    printf("%s\n", C_DEFAULT);
    
    switch (chosen_team_option)
    {
        // Load the player's team as is.
        case 1:
            get_player_team(player_team);
            printf("\n\n");
            break;
        
        // Load the player's team and enable him to modify it.
        case 2:
            get_player_team(player_team);

            while (true)
            {
                for (int i = 0; i < 6; i++) {
                    if (player_team[i].id == 0) {
                        printf("%d: Add pokemon\n", i+1);
                    }
                    else {
                        printf("%d: Change %s to another pokemon\n", i+1, get_species_by_id(player_team[i].id).name);
                    }
                }
                printf("7: Exit\n");

                printf("%sYour choice: ", C_GRAY);
                int chosen_pk_option;
                scanf("%d", &chosen_pk_option);
                getchar();

                if (chosen_pk_option == 7) {
                    break;
                }
                else if (chosen_pk_option >= 1 && chosen_pk_option <= 6) {
                    // Let the player choose which pokemon to add.
                    printf("Enter the name or id of the new pokemon: ");
                    char chosen_pk[24];
                    fgets(chosen_pk, 24, stdin);
                    chosen_pk[strcspn(chosen_pk, "\n")] = '\0'; // Removes the newline at the end of the list.

                    // Let the player choose the new pokemon's level.
                    printf("Enter the level of the new pokemon: ");
                    int chosen_level;
                    scanf("%d", &chosen_level);
                    getchar();

                    // Get the new pokemon with its id.
                    if (atoi(chosen_pk) != 0) {
                        Species chosen_species = get_species_by_id(atoi(chosen_pk));
                        init_pk_instance(&player_team[chosen_pk_option-1], &chosen_species, chosen_level);
                    }
                    // Get the new pokemon with its name.
                    else {
                        Species chosen_species = get_species_by_name(strtoupper(chosen_pk));
                        init_pk_instance(&player_team[chosen_pk_option-1], &chosen_species, chosen_level);
                    }

                    printf("\n");
                }

                printf("%s", C_DEFAULT);
            }
            printf("%s\n\n", C_DEFAULT);
            break;
        
        // Let the player create a new team.
        case 3:
            for (int i = 0; i < 6; i++) {
                player_team[i].id = 0;
            }

            while (true)
            {
                for (int i = 0; i < 6; i++) {
                    if (player_team[i].id == 0) {
                        printf("%d: Add pokemon\n", i+1);
                    }
                    else {
                        printf("%d: Change %s to another pokemon\n", i+1, get_species_by_id(player_team[i].id).name);
                    }
                }
                printf("7: Exit\n");

                printf("%sYour choice: ", C_GRAY);
                int chosen_pk_option;
                scanf("%d", &chosen_pk_option);
                getchar();
                printf("%s\n", C_DEFAULT);

                if (chosen_pk_option == 7) {
                    break;
                }
                else if (chosen_pk_option >= 1 && chosen_pk_option <= 6) {
                    // Let the player choose which pokemon to add.
                    printf("Enter the name or id of the new pokemon: ");
                    char chosen_pk[24];
                    fgets(chosen_pk, 24, stdin);
                    chosen_pk[strcspn(chosen_pk, "\n")] = '\0'; // Removes the newline at the end of the list.

                    // Let the player choose the new pokemon's level.
                    printf("Enter the level of the new pokemon: ");
                    int chosen_level;
                    scanf("%d", &chosen_level);
                    getchar();

                    // Get the new pokemon with its id.
                    if (atoi(chosen_pk) != 0) {
                        Species chosen_species = get_species_by_id(atoi(chosen_pk));
                        init_pk_instance(&player_team[chosen_pk_option-1], &chosen_species, chosen_level);
                    }
                    else {
                        Species chosen_species = get_species_by_name(strtoupper(chosen_pk));
                        init_pk_instance(&player_team[chosen_pk_option-1], &chosen_species, chosen_level);
                    }

                    printf("\n");
                }
            }
            break;
        
        default:
            break;
    }
}

void get_player_inventory()
{
    // /*
    // Get the player's inventory from the binary file inventory.bin.
    FILE* f = fopen("../player_data/inventory.bin", "rb");
    fread(&inventory, sizeof(inventory[0]), 9, f);
    fclose(f);
    // */
    
    // Old way to do it with a csv file.
    /*
    FILE* f = fopen("../player_data/inventory.csv", "r");

    // Set the item names.
    for (int i = 0; i < 8; i++) {
        char current_item_name[16];
        fscanf(f, "%[^,\n]", current_item_name);
        fgetc(f);
        if (i < 7) {
            fgetc(f);
        }
        for (int j = 0; current_item_name[j] != '\0'; j++) {
            inventory[i].name[j] = current_item_name[j];
        }
    }

    // Set the item counts.
    for (int i = 0; i < 8; i++) {
        int current_item_count = 0;
        fscanf(f, "%d", &current_item_count);
        fgetc(f);
        inventory[i].count = current_item_count;
    }
    */
}

void save_player_inventory()
{
    // Save the player's inventory in a binary file.
    FILE* f = fopen("../player_data/inventory.bin", "wb");
    fwrite(&inventory, sizeof(inventory[0]), 9, f);
    fclose(f);

    // Old way to do it with a csv file.
    /*
    // Get the first line of the inventory.csv file (the one that contains all the headers).
    f = fopen("../player_data/inventory.csv", "r");
    char* first_inventory_csv_line = calloc(128, sizeof(char));
    fgets(first_inventory_csv_line, 256, f);

    // Wipe the inventory.csv and add the header line.
    f = fopen("../player_data/inventory.csv", "w");
    fprintf(f, "%s", first_inventory_csv_line);
    free(first_inventory_csv_line);

    // Save all the item counts.
    for (int i = 0; i < 8; i++) {
        if (i == 7) {
            fprintf(f, "%d", inventory[i].count);
        }
        else {
            fprintf(f, "%d, ", inventory[i].count);
        }
    }
    */
}


// --------------- BATTLE FUNCTIONS --------------- //

void before_battle_actions()
{

    // Check if the player can go to a pokecenter.
    FILE* f = fopen("../player_data/last_city.csv", "r");
    int last_city;
    fscanf(f, "%d", &last_city);
    fclose(f);

    while (true)
    {
        // Clear the console.
        system("clear");

        // Show the player's options.
        printf("1: BATTLE\n2: ITEMS\n");
        if (last_city >= 5) {
            printf("3: SHOP\n4: POKE CENTER\n");
        }

        // Let him choose.
        printf("%sYour choice: ", C_GRAY);
        int player_choice;
        scanf("%d", &player_choice);
        getchar();
        printf("%s\n\n", C_DEFAULT);

        switch (player_choice)
        {
            case 2: ;
                int item_choice = item_menu(false);
                // If the player used an item on his pokemon.
                if (item_choice >= 40 && item_choice <= 86) {
                    use_item(item_choice/10, &inventory[item_choice/10-1], &player_team[item_choice%10-1]);
                }
                break;
            
            case 3:
                // Go to a shop.
                if (last_city >= 5) {
                    shop_menu();

                    // Reset the last_city value.
                    f = fopen("../player_data/last_city.csv", "w");
                    fprintf(f, "%d", 0);
                    fclose(f);
                }
                break;

            case 4:
                // Go to a pokemon center.
                if (last_city >= 5) {
                    pokecenter();

                    // Reset the last_city value.
                    f = fopen("../player_data/last_city.csv", "w");
                    fprintf(f, "%d", 0);
                    fclose(f);
                }
        }

        if (player_choice == 1) {
            // Increment the last_city value.
            f = fopen("../player_data/last_city.csv", "r");
            fscanf(f, "%d", &last_city);
            fclose(f);
            f = fopen("../player_data/last_city.csv", "w");
            fprintf(f, "%d", last_city+1);
            fclose(f);
            break;
        }
    }
}

void pokecenter()
{
    printf("Welcome to our POKEMON CENTER!\nWe heal your POKéMON back to perfect health!\nShall we heal your POKEMON?\n");
    printf("%sYour choice (y/n): ", C_GRAY);

    // Let the player choose.
    char player_choice = getchar();
    getchar();
    printf("%s\n", C_DEFAULT);

    // If the player agreed.
    if (player_choice == 'y' || player_choice == 'Y') {
        printf("OK. We'll need your POKEMON.\n");

        printf("%sPress [enter] to continue.", C_GRAY);
        getchar();
        printf("%s\n", C_DEFAULT);

        // Heal the player's pokemons.
        for (int i = 0; i < 6; i++) {
            player_team[i].hp = player_team[i].max_hp;
            for (int j = 0; j < 4; j++) {
                player_team[i].moves[j].current_pp = move_list[player_team[i].moves[j].id].pp;
            }
        }

        printf("Thank you!\nYour POKEMON are fighting fit!\nWe hope to see you again!\n");

        printf("%sPress [enter] to continue.", C_GRAY);
        getchar();
        printf("%s\n", C_DEFAULT);
    }

    // If the player disagreed.
    else {
        printf("We hope to see you again!\n");

        printf("%sPress [enter] to continue.", C_GRAY);
        getchar();
        printf("%s\n", C_DEFAULT);
    }
}

void shop_menu()
{
    while (true)
    {
        // Clear the console.
        system("clear");

        // Show all of the player's options.
        printf("Items in shop:\n");
        for (int i = 0; i < 8; i++) {
            printf("%d: %s (Price: ₽%d, In bag: %d)\n", i+1, inventory[i].name, inventory[i].price, inventory[i].count);
        }
        printf("9: BACK\n");

        // Let the player choose which item to use.
        printf("Your budget: ₽%d\n%sBuy which item? ", inventory[9].count, C_GRAY);
        int item_choice;
        scanf("%d", &item_choice);
        getchar();
        printf("%s\n", C_DEFAULT);

        if (item_choice >= 1 && item_choice <= 8) {
            if (inventory[item_choice-1].price <= inventory[9].count) {
                inventory[item_choice-1].count++;
                inventory[9].count -= inventory[item_choice-1].price;
                printf("You bought a %s!\n", inventory[item_choice-1].name);

                // Wait for the player to continue.
                printf("%sPress [enter] to continue.", C_GRAY);
                getchar();
                printf("%s\n\n", C_DEFAULT);
            }
        }
        else if (item_choice == 9) {
            break;
        }
    }
}

int fight_menu(Pokemon* active_pokemon)
{
    for (int i = 0; i < 4; i++) 
    {
        if (active_pokemon->moves[i].id != 0) {
            MoveDBEntry current_move = get_move_by_id(active_pokemon->moves[i].id);
            printf("%d: %s (Type: %s, PP: %d/%d)\n", i + 1, strtoupper(current_move.identifier), get_type_by_id(current_move.type_id), active_pokemon->moves[i].current_pp, current_move.pp);
        }
        else {
            printf("%d: -\n", i + 1);
        }
    }
    printf("5: BACK\n");

    printf("%sYour choice: ", C_GRAY);
    int move_choice = -1;
    scanf("%d", &move_choice);
    getchar();
    printf("%s\n\n", C_DEFAULT);

    return move_choice;
}

int pkmn_menu(Pokemon* player_team, bool can_back)
{
    int pkmn_choice = -1;
    while (pkmn_choice < 2 || pkmn_choice > 7) {
        // Show the name, hp, max hp and level of each of the player's pokemon.
        for (int i = 0; i < 6; i++)
        {
            if (player_team[i].id > 0) {
                if (player_team[i].hp <= 0) {
                    printf("%d: %s (%sFNT%s, LV: %d)\n", i+1, get_species_by_id(player_team[i].id).name, C_RED, C_DEFAULT, player_team[i].lv);
                }
                else if (player_team[i].hp <= player_team[i].max_hp/4) {
                    printf("%d: %s (HP: %s%d/%d%s, LV: %d)\n", i+1, get_species_by_id(player_team[i].id).name, C_RED, player_team[i].hp, player_team[i].max_hp, C_DEFAULT, player_team[i].lv);
                }
                else if (player_team[i].hp <= player_team[i].max_hp/2) {
                    printf("%d: %s (HP: %s%d/%d%s, LV: %d)\n", i+1, get_species_by_id(player_team[i].id).name, C_YELLOW, player_team[i].hp, player_team[i].max_hp, C_DEFAULT, player_team[i].lv);
                }
                else {
                    printf("%d: %s (HP: %s%d/%d%s, LV: %d)\n", i+1, get_species_by_id(player_team[i].id).name, C_GREEN, player_team[i].hp, player_team[i].max_hp, C_DEFAULT, player_team[i].lv);
                }
            }
            else {
                printf("%d: -\n", i+1);
            }
        }
        if (can_back) {
            printf("7: BACK\n");
        }

        // Let the player choose which pokemon to switch in.
        printf("%sBring out which pokemon? ", C_GRAY);
        scanf("%d", &pkmn_choice);
        getchar();
        printf("%s\n\n", C_DEFAULT);

        // Let the player only choose slots where there are pokemons in his team.
        if (pkmn_choice != 7 && (player_team[pkmn_choice-1].id == 0 || player_team[pkmn_choice-1].hp <= 0)) {
            pkmn_choice = -1;
        }
        // Don't let the player go back if he shouldn't.
        if (!can_back && pkmn_choice == 7) {
            pkmn_choice = -1;
        }
    }
    return pkmn_choice;
}

int item_menu(bool can_use_balls)
{
    while (true) {
        // Clear the console.
        system("clear");

        // Show all of the player's options.
        printf("Items in bag:\n");
        for (int i = 0; i < 8; i++) {
            printf("%d: %s (%d remaining)\n", i+1, inventory[i].name, inventory[i].count);
        }
        printf("9: BACK\n");

        // Let the player choose which item to use.
        printf("%sUse which item? ", C_GRAY);
        int item_choice;
        scanf("%d", &item_choice);
        getchar();
        printf("%s\n", C_DEFAULT);

        // Catch a pokemon or go back.
        if ((((item_choice >= 1 && item_choice <= 3) && inventory[item_choice-1].count > 0 && can_use_balls) || item_choice == 9)) {
            return item_choice * 10;
        }
        // Use an item on a pokemon.
        else if ((item_choice >= 4 && item_choice <= 8) && inventory[item_choice-1].count > 0) {
            // Show all of the pokemon the item can be used on.
            for (int i = 0; i < 6; i++) {
                if (player_team[i].id != 0) {
                    if (player_team[i].hp <= 0) {
                        printf("%d: %s (%sFNT%s)\n", i+1, get_species_by_id(player_team[i].id).name, C_RED, C_DEFAULT);
                    }
                    else if (player_team[i].hp <= player_team[i].max_hp/4) {
                        printf("%d: %s (HP: %s%d/%d%s)\n", i+1, get_species_by_id(player_team[i].id).name, C_RED, player_team[i].hp, player_team[i].max_hp, C_DEFAULT);
                    }
                    else if (player_team[i].hp <= player_team[i].max_hp/2) {
                        printf("%d: %s (HP: %s%d/%d%s)\n", i+1, get_species_by_id(player_team[i].id).name, C_YELLOW, player_team[i].hp, player_team[i].max_hp, C_DEFAULT);
                    }
                    else {
                        printf("%d: %s (HP: %s%d/%d%s)\n", i+1, get_species_by_id(player_team[i].id).name, C_GREEN, player_team[i].hp, player_team[i].max_hp, C_DEFAULT);
                    }
                }
                else {
                    printf("%d: -\n", i+1);
                }
            }
            printf("7: CANCEL\n");

            // Let the player choose which pokemon to use the item on.
            printf("%sUse the item on which pokemon? ", C_GRAY);
            int pk_choice;
            scanf("%d", &pk_choice);
            getchar();
            printf("%s\n", C_DEFAULT);

            if ((pk_choice >= 1 && pk_choice <= 6)) {
                // Check if a potion can be used on the selected pokemon.
                if (item_choice >= 4 && item_choice <= 6) {
                    if (player_team[pk_choice-1].hp > 0 && player_team[pk_choice-1].hp < player_team[pk_choice-1].max_hp) {
                        return item_choice * 10 + pk_choice;
                    }
                }
                // Check if a revive can be used on the current pokemon.
                if (item_choice >= 7 && item_choice <= 8) {
                    if (player_team[pk_choice-1].hp <= 0) {
                        return item_choice * 10 + pk_choice;
                    }
                }
            }
        }
    }
}

bool run(Pokemon* active_pk, Pokemon* wild_pk, int escape_attempts)
{
    int player_speed = active_pk->speed;
    int wild_speed = wild_pk->speed;

    // If the player's active pokemon's speed is greater than the wild pokemon's speed, fleeing succeeds.
    if (player_speed > wild_speed) {
        return true;
    }
    // If (wild_speed / 4) % 256 is equal to 0, fleeing succeeds.
    else if ((wild_speed / 4) % 256 == 0) {
        return true;
    }
    else {
        int escape_odds = ((player_speed * 32) / ((wild_speed / 4) % 256)) + 30 * escape_attempts;

        // If the chance of escaping is greater than 255, fleeing succeeds.
        if (escape_odds > 255) {
            return true;
        }
        else {
            int escape_rand = rand() % 256;
            
            // If the random generated number is smaller than the chance of escaping, fleeing succeds.
            if (escape_rand < escape_odds) {
                return true;
            }
            // If everything fails, the player is unable to escape.
            else {
                return false;
            }
        }
    }
}

char* do_player_turn(Pokemon* player_team)
{
    char* total_action;
    int player_action;
    int fight_action;
    int pkmn_action;
    int item_action;

    while (true) 
    {
        // Show the player's options.
        printf("Possible actions:\n1: FIGHT\n2: PKMN\n3: ITEM\n4: RUN\n");

        // Get the player's choice.
        printf("%sYour choice: ", C_GRAY);
        scanf("%d", &player_action);
        getchar();
        printf("%s\n\n", C_DEFAULT);

        // Show the right menu for the player's choice.
        switch (player_action)
        {
            case 1: ;
                // Get which move the player chooses, or if he wants to go back.
                fight_action = fight_menu(&player_team[0]);
                if (fight_action >= 1 && fight_action <= 4) {
                    total_action = calloc(2, sizeof(char));
                    total_action[0] = '0' + player_action;
                    total_action[1] = '0' + fight_action;
                    return total_action;
                }
                break;

            case 2:
                // Get which pokemon the player wants to switch in, or if he wants to go back.
                pkmn_action = pkmn_menu(player_team, true);
                if (pkmn_action >= 2 && pkmn_action <= 6) {
                    total_action = calloc(2, sizeof(char));
                    total_action[0] = '0' + player_action;
                    total_action[1] = '0' + pkmn_action;
                    return total_action;
                }
                break;

            case 3:
                // Get which item the player wants to use or if he wants to go back.
                item_action = item_menu(true);
                char str_item_action[3];
                sprintf(str_item_action, "%d", item_action);
                if (item_action >= 10 && item_action <= 86) {
                    total_action = calloc(3, sizeof(char));
                    total_action[0] = '0' + player_action;
                    total_action[1] = str_item_action[0];
                    total_action[2] = str_item_action[1];
                    return total_action;
                }
                break;

            case 4: ;
                // Try to run.
                total_action = calloc(1, sizeof(char));
                *total_action = '4';
                return total_action;
        }
    }

}

int do_wild_pk_turn(Pokemon* wild_pk)
{
    int move_num = 0;
    for (int i = 0; i < 4; i++) {
        if (wild_pk->moves[i].id != 0) {
            move_num++;
        }
    }
    return rand() % move_num + 1;
}

int play_wild_battle_turn(Pokemon* player_team, char* player_choice, Pokemon* wild_pk, int wild_pk_choice, int* escape_attempts, bool* used_pk)
{
    // Variable that temporarily stores the damage of each one of the attacks.
    int temp_damage = 0;

    switch (player_choice[0])
    {
        // --- THE ACTIVE POKEMON ATTACKS --- //
        case '1': ;
            // Reset the player's number of consecutive fleeing attempts.
            (*escape_attempts) = 0;
            
            // If the player's pokemon is faster, make him play first.
            if (player_team[0].speed >= wild_pk->speed) 
            {
                // Make sure the player's pokemon has enough pp to attack.
                if (player_team[0].moves[player_choice[1]-'0'-1].current_pp > 0) 
                {
                    temp_damage = get_move_damage(&player_team[0], wild_pk, player_choice[1]-'0'-1);
                    wild_pk->hp -= temp_damage;

                    printf("%s used %s! \n%d damage dealt to %s.\n\n", 
                        get_species_by_id(player_team[0].id).name, 
                        get_move_by_id(player_team[0].moves[player_choice[1]-'0'-1].id).identifier, 
                        temp_damage, 
                        get_species_by_id(wild_pk->id).name);
                }

                // Tell the player his attack wasn't performed if the pokemon had no PP left.
                else {
                    printf("No PP left for to use %s!\n0 damage dealt to %s.\n\n",
                        get_move_by_id(player_team[0].moves[player_choice[1]-'0'-1].id).identifier, 
                        get_species_by_id(wild_pk->id).name);
                }

                // Make the wild pokemon attack.
                temp_damage = get_move_damage(wild_pk, &player_team[0], wild_pk_choice-1);
                player_team[0].hp -= temp_damage;

                printf("Enemy %s used %s!\n%d damage dealt to %s.\n", 
                    get_species_by_id(wild_pk->id).name, 
                    get_move_by_id(wild_pk->moves[wild_pk_choice-1].id).identifier, 
                    temp_damage, 
                    get_species_by_id(player_team[0].id).name);
            }

            // If the wild pokemon is faster, make him play first.
            else 
            {
                // Make the wild pokemon attack.
                temp_damage = get_move_damage(wild_pk, &player_team[0], wild_pk_choice-1);
                player_team[0].hp -= temp_damage;

                printf("Enemy %s used %s! \n%d damage dealt to %s.\n\n", 
                    get_species_by_id(wild_pk->id).name, 
                    get_move_by_id(wild_pk->moves[wild_pk_choice-1].id).identifier, 
                    temp_damage, 
                    get_species_by_id(player_team[0].id).name);

                // Make sure the player's pokemon has enough pp to attack.
                if (player_team[0].moves[player_choice[1]-'0'-1].current_pp > 0) 
                {
                    temp_damage = get_move_damage(&player_team[0], wild_pk, player_choice[1]-'0'-1);
                    wild_pk->hp -= temp_damage;
                    
                    printf("%s used %s! \n%d damage dealt to %s.\n", 
                        get_species_by_id(player_team[0].id).name, 
                        get_move_by_id(player_team[0].moves[player_choice[1]-'0'-1].id).identifier, 
                        temp_damage, 
                        get_species_by_id(wild_pk->id).name);
                }

                // Tell the player his attack wasn't performed if the pokemon had no PP left.
                else {
                    printf("No PP left for to use %s!\n0 damage dealt to %s.\n",
                        get_move_by_id(player_team[0].moves[player_choice[1]-'0'-1].id).identifier, 
                        get_species_by_id(wild_pk->id).name);
                }
            }

            // Decrease the player's pokemon's move's PPs.
            if (player_team[0].moves[player_choice[1]-'0'-1].current_pp > 0) {
                player_team[0].moves[player_choice[1]-'0'-1].current_pp--;
            }

            break;

        // --- THE PLAYER SWITCHES POKEMON --- //
        case '2':
            // Reset the player's number of consecutive fleeing attempts.
            (*escape_attempts) = 0;

            // Tell the player his active pokemon is getting switched out.
            printf("%s enough! Come back!\n", get_species_by_id(player_team[0].id).name);

            // Switch the chosen pokemon with the active pokemon.
            Pokemon temp_pk = player_team[0];
            player_team[0] = player_team[player_choice[1]-'0'-1];
            player_team[player_choice[1]-'0'-1] = temp_pk;

            // Switch the "used" bool of the pokemon to true.
            used_pk[player_choice[1]-'0'-1] = true;

            // Tell the player which pokemon was sent out.
            printf("Go! %s!\n\n", get_species_by_id(player_team[0].id).name);

            // Make the wild pokemon attack.
            temp_damage = get_move_damage(wild_pk, &player_team[0], wild_pk_choice-1);
            player_team[0].hp -= temp_damage;

            printf("Enemy %s used %s! \n%d damage dealt to %s.\n", 
                get_species_by_id(wild_pk->id).name, 
                get_move_by_id(wild_pk->moves[wild_pk_choice-1].id).identifier, 
                temp_damage, 
                get_species_by_id(player_team[0].id).name);

            break;
        
        // --- THE PLAYER USES AN ITEM --- //
        case '3':
            // Reset the player's number of consecutive fleeing attempts.
            (*escape_attempts) = 0;

            // If the player tried to capture the wild pokemon.
            if (player_choice[1]-'0' >= 1 && player_choice[1]-'0' <= 3) {
                if (capture_pk(wild_pk, player_choice[1]-'0', &inventory[player_choice[1]-'0'-1])) {
                    return 1;
                }
            }

            // If the player used an item on his pokemon.
            else if (player_choice[1]-'0' >= 4 && player_choice[1]-'0' <= 8) {
                use_item(player_choice[1]-'0', &inventory[player_choice[1]-'0'-1], &player_team[player_choice[2]-'0'-1]);
            }

            // Make the wild pokemon attack.
            temp_damage = get_move_damage(wild_pk, &player_team[0], wild_pk_choice-1);
            player_team[0].hp -= temp_damage;

            printf("Enemy %s used %s! \n%d damage dealt to %s.\n", 
                get_species_by_id(wild_pk->id).name, 
                get_move_by_id(wild_pk->moves[wild_pk_choice-1].id).identifier, 
                temp_damage, 
                get_species_by_id(player_team[0].id).name);

            break;

        // --- THE PLAYER TRIES TO RUN --- //
        case '4':
            // If the player wasn't able to escape, increment his number of escape attempts and make the enemy attack.
            if (!run(&player_team[0], wild_pk, *escape_attempts)) 
            {
                (*escape_attempts)++;
                printf("Can't escape!\n\n");

                // Make the wild pokemon attack.
                temp_damage = get_move_damage(wild_pk, &player_team[0], wild_pk_choice-1);
                player_team[0].hp -= temp_damage;

                printf("Enemy %s used %s! \n%d damage dealt to %s.\n", 
                    get_species_by_id(wild_pk->id).name, 
                    get_move_by_id(wild_pk->moves[wild_pk_choice-1].id).identifier, 
                    temp_damage, 
                    get_species_by_id(player_team[0].id).name);

            }
            // If the player was able to run return false to end the battle.
            else {
                return 2;
            }
            break;
    }

    return 0;
}

int start_wild_battle(Pokemon* player_team, Pokemon* wild_pk, bool* used_pk)
{
    // Clear the console.
    system("clear");

    // Battle start text.
    if (wild_pk->shiny) {
        printf("SHINY wild %s appeared!\n", get_species_by_id(wild_pk->id).name);
    }
    else {
        printf("Wild %s appeared!\n", get_species_by_id(wild_pk->id).name);
    }

    // Wait for player input to continue the battle.
    printf("%sPress [enter] to continue.", C_GRAY);
    getchar();
    printf("%s\n\n", C_DEFAULT);

    // Holds the number of consecutive fleeing attempts from the player.
    int escape_attempts = 0;

    while (true) 
    {
        // ----- SHOW THE GAME STATE ----- //

        // Clear the console.
        system("clear");
        
        // Show the wild pokemon's name, level and hp.
        if (wild_pk->shiny) {
            if (wild_pk->hp <= wild_pk->max_hp/4) {
                printf("SHINY wild %s:\nLV: %d\nHP: %s%d/%d%s\n\n", get_species_by_id(wild_pk->id).name, wild_pk->lv, C_RED, wild_pk->hp, wild_pk->max_hp, C_DEFAULT);
            }
            else if (wild_pk->hp <= wild_pk->max_hp/2) {
                printf("SHINY wild %s:\nLV: %d\nHP: %s%d/%d%s\n\n", get_species_by_id(wild_pk->id).name, wild_pk->lv, C_YELLOW, wild_pk->hp, wild_pk->max_hp, C_DEFAULT);
            }
            else {
                printf("SHINY wild %s:\nLV: %d\nHP: %s%d/%d%s\n\n", get_species_by_id(wild_pk->id).name, wild_pk->lv, C_GREEN, wild_pk->hp, wild_pk->max_hp, C_DEFAULT);
            }
        }
        else {
            if (wild_pk->hp <= wild_pk->max_hp/4) {
                printf("Wild %s:\nLV: %d\nHP: %s%d/%d%s\n\n", get_species_by_id(wild_pk->id).name, wild_pk->lv, C_RED, wild_pk->hp, wild_pk->max_hp, C_DEFAULT);
            }
            else if (wild_pk->hp <= wild_pk->max_hp/2) {
                printf("Wild %s:\nLV: %d\nHP: %s%d/%d%s\n\n", get_species_by_id(wild_pk->id).name, wild_pk->lv, C_YELLOW, wild_pk->hp, wild_pk->max_hp, C_DEFAULT);
            }
            else {
                printf("Wild %s:\nLV: %d\nHP: %s%d/%d%s\n\n", get_species_by_id(wild_pk->id).name, wild_pk->lv, C_GREEN, wild_pk->hp, wild_pk->max_hp, C_DEFAULT);
            }
        }

        // Show the player's pokemon's name, level, hp and max hp.
        if (player_team[0].shiny) {
            if (player_team[0].hp <= player_team[0].max_hp/4) {
                printf("Your SHINY %s:\nLV: %d\nHP: %s%d/%d%s\n", get_species_by_id(player_team[0].id).name, player_team[0].lv, C_RED, player_team[0].hp, player_team[0].max_hp, C_DEFAULT);
            }
            else if (player_team[0].hp <= player_team[0].max_hp/2) {
                printf("Your SHINY %s:\nLV: %d\nHP: %s%d/%d%s\n", get_species_by_id(player_team[0].id).name, player_team[0].lv, C_YELLOW, player_team[0].hp, player_team[0].max_hp, C_DEFAULT);
            }
            else {
                printf("Your SHINY %s:\nLV: %d\nHP: %s%d/%d%s\n", get_species_by_id(player_team[0].id).name, player_team[0].lv, C_GREEN, player_team[0].hp, player_team[0].max_hp, C_DEFAULT);
            }
        }
        else {
            if (player_team[0].hp <= player_team[0].max_hp/4) {
                printf("Your %s:\nLV: %d\nHP: %s%d/%d%s\n", get_species_by_id(player_team[0].id).name, player_team[0].lv, C_RED, player_team[0].hp, player_team[0].max_hp, C_DEFAULT);
            }
            else if (player_team[0].hp <= player_team[0].max_hp/2) {
                printf("Your %s:\nLV: %d\nHP: %s%d/%d%s\n", get_species_by_id(player_team[0].id).name, player_team[0].lv, C_YELLOW, player_team[0].hp, player_team[0].max_hp, C_DEFAULT);
            }
            else {
                printf("Your %s:\nLV: %d\nHP: %s%d/%d%s\n", get_species_by_id(player_team[0].id).name, player_team[0].lv, C_GREEN, player_team[0].hp, player_team[0].max_hp, C_DEFAULT);
            }
        }

        // Wait for player input to continue the battle.
        printf("%sPress [enter] to continue.", C_GRAY);
        getchar();
        printf("%s\n\n", C_DEFAULT);


        // ----- LET BOTH SIDES CHOOSE THEIR ACTION ----- //

        // Let the player choose what to do.
        char* player_choice = do_player_turn(player_team);

        // Let the wild pokemon choose what to do.
        int wild_pk_choice = do_wild_pk_turn(wild_pk);


        // ----- PLAY THE TURN ----- //

        // Clear the console.
        system("clear");

        // Play the turn according to what each chose to do.
        int turn_outcome = play_wild_battle_turn(player_team, player_choice, wild_pk, wild_pk_choice, &escape_attempts, used_pk);


        // ----- END THE TURN ----- //

        // Wait for player input to continue the battle if it is not finished.
        printf("%sPress [enter] to continue.", C_GRAY);
        getchar();
        printf("%s\n\n", C_DEFAULT);

        // Check if all the player's pokemon are ko.
        bool player_defeated = true;
        for (int i = 0; i < 6; i++) {
            if (player_team[i].id != 0 && player_team[i].hp > 0) {
                player_defeated = false;
            }
        }

        // Check if the player's active pokemon fainted.
        if (!player_defeated) {
            if (player_team[0].hp <= 0) {
                printf("%s fainted!\n", get_species_by_id(player_team[0].id).name);
                int chosen_pk = pkmn_menu(player_team, false);
                Pokemon temp_pk = player_team[0];
                player_team[0] = player_team[chosen_pk-1];
                player_team[chosen_pk-1] = temp_pk;
            }
        }

        free(player_choice);

        // Return the right value if one of the three return conditions occured.
        if (turn_outcome == 2) {
            // 0 for player escape.
            return 0;
        }
        else if (player_defeated) {
            // 1 for player defeat.
            return 1;
        }
        else if (wild_pk->hp <= 0) {
            // 2 for player victory.
            return 2;
        }
        else if (turn_outcome == 1) {
            // 3 for player capture.
            return 3;
        }
    }
}

void end_wild_battle(Pokemon* player_team, Pokemon* wild_pk, int outcome, bool* used_pk)
{
    switch (outcome)
    {
        // If the player escaped.
        case 0:
            printf("You got away to safety!\n");
            break;
        
        // If the player was defeated.
        case 1:
            printf("You are out of useable pokemon!\nYou dropped ₽%d and blacked out!\n\n", inventory[8].count / 2);
            inventory[8].count /= 2;
            while (player_team[0].hp <= 0) {
                pokecenter();
            }

            break;
        
        // If the player defeated the wild pokemon.
        case 2:
            // Clear the console.
            system("clear");

            // Tell the player the wild pokemon fainted.
            printf("Enemy %s fainted!\n", get_species_by_id(wild_pk->id).name);

            // Wait for player input to continue.
            printf("%sPress [enter] to continue.", C_GRAY);
            getchar();
            printf("%s\n", C_DEFAULT);

            // Get the exp yeilded by the wild pokemon.
            int earned_exp = get_earned_exp(&player_team[0], wild_pk, false);

            // Find the number of pokemon used.
            int used_pk_num = 1;
            for (int i = 1; i < 6; i++) {
                if (used_pk[i]) {
                    used_pk_num++;
                }
            }

            // Give the right amount of exp to each pokemon.
            for (int i = 0; i < 6; i++) {
                if (used_pk[i]) {
                    // Give the pokemon exp and notify the user.
                    player_team[i].exp += earned_exp / used_pk_num;
                    printf("%s gained %d EXP points.\n", get_species_by_id(player_team[i].id).name, earned_exp / used_pk_num);

                    // Wait for player input to continue.
                    printf("%sPress [enter] to continue.", C_GRAY);
                    getchar();
                    printf("%s\n", C_DEFAULT);

                    // Update the player's pokemon's levels and prompt him if one leveled up, learned a move or evolved.
                    update_pk_lv(&player_team[i]);
                }
            }
            
            // Make the player earn pokedollars.
            int money_multiplier = wild_pk->lv - player_team[0].lv;
            if (money_multiplier < 0) {
                inventory[8].count += 200 / (-1 * money_multiplier);
                printf("You earned ₽%d!\n", 200 / (-1 * money_multiplier));
            }
            else {
                inventory[8].count += 200 * money_multiplier;
                printf("You earned ₽%d!\n", 200 * money_multiplier);
            }

            break;
    }
}