#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "text_formatter.h"
#include "../parsing/parser_helper.h"
#include "species.h"
#include "../parsing/moves/moves_parser.h"

// All the pokemon species of generation 1.
extern Species gen1Species[151];


// Structure for a move instance (id, pp, pp max).
typedef struct Move {
    int id;
    int current_pp;
} Move;

// Structure for a pokemon instance that can be fought.
typedef struct Pokemon {
    int id;
    int lv;
    int exp;
    int hp;
    int max_hp;
    int attack;
    int defense;
    int spe_attack;
    int spe_defense;
    int speed;
    int hp_iv;
    int attack_iv;
    int defense_iv;
    int speed_iv;
    int special_iv;
    bool shiny;
    Move moves[4];
} Pokemon;

// The player's team.
extern Pokemon player_team[6];

// Structure for an item entry in the player's inventory.
typedef struct Item {
    char name[16];
    int count;
    int price;
} Item;

// The player's inventory.
extern Item inventory[9];


// --------------- UTILITY FUNCTIONS --------------- //

// Fills the global array gen1Species with data from the gen1Species.bin file.
void get_gen1Species();

// FIlls the global array move_list with data from the move_list.bin file.
void get_move_list();

// Returns the Species corresponding to the given id.
Species get_species_by_id(int id);

// Returns the Species corresponding to the given name.
Species get_species_by_name(char* name);

// Returns the damage of a pokemon's move.
int get_move_damage(Pokemon* attack_pk, Pokemon* defense_pk, int chosen_move);

// Returns the exp a pokemon should earn after defeating another pokemon.
int get_earned_exp(Pokemon* winner_pk, Pokemon* fainted_pk, bool trainer_battle);

// Updates a pokemon's lv using its exp.
// Returns true if the pokemon leveled up.
bool update_pk_lv(Pokemon* pk);

// Attempts to make the given pokemon learn the given move.
void learn_move(Pokemon* pk, int move_id);

// Attempts to capture the wild pokemon.
bool capture_pk(Pokemon* wild_pk, int ball_type, Item* ball_item);

// Uses an item on one of the player's pokemons.
void use_item(int item_type, Item* item, Pokemon* pk);


// --------------- SETUP FUNCTIONS --------------- //

// Initializes a pokemon instance based on a pokemon species.
void init_pk_instance(Pokemon* pk_instance, Species* species, int level);

// Gets the player's pokemon team.
void get_player_team();

// Saves the player's pokemon team.
void save_player_team();

// Let the player choose wether to load his team, change it, or make a new one.
void player_team_options();

// Get the player's inventory.
void get_player_inventory();

// Saves the player's inventory.
void save_player_inventory();


// --------------- BATTLE FUNCTIONS --------------- //

// Makes the player able to do a few things before starting a battle.
void before_battle_actions();

// Heals all of the player's team.
void pokecenter();

// Makes the player able to purchase items.
void shop_menu();

// Shows the fight menu, gets the player's choice and returns it.
int fight_menu(Pokemon* active_pokemon);

// Shows the PKMN menu, gets the player's choice and returns it.
int pkmn_menu(Pokemon* player_team, bool can_back);

// Shows the item menu, gets the player's choice and returns it.
int item_menu(bool can_use_balls);

// Shows what happened when the player tried to run and return the outcome (true: ran away, false: couldn't run).
bool run(Pokemon* active_pk, Pokemon* wild_pk, int escape_attempts);

// Makes the player choose what to do during his turn.
// Returns the player's choice ("12":FIGHT->MOVE_2, "24":PKMN->POKEMON_4, "38":ITEM->ITEM_8, "41":RUN->TRUE).
char* do_player_turn(Pokemon* player_team);

// Makes the wild pokemon choose what to do during his turn.
// Returns the wild pokemon's choice (2:MOVE_2).
int do_wild_pk_turn(Pokemon* wild_pk);

// Play the turn according to what each chose to do.
// If this function returns 0, the player didn't end the battle.
// If this function returns 1, the player captured the wild pokemon.
// If this function returns 2, the player escaped.
int play_wild_battle_turn(Pokemon* player_team, char* player_choice, Pokemon* wild_pk, int wild_pk_choice, int* escape_attempts, bool* used_pk);

// Starts a turn based battle between the player's team and a wild pokemon.
// Returns 0 if the player escaped, 1 if the player was defeated and 2 if the player defeated the wild pokemon.
int start_wild_battle(Pokemon* player_team, Pokemon* wild_pk, bool* used_pk);

// Ends a turn based battle between the player's team and a wild pokemon.
void end_wild_battle(Pokemon* player_team, Pokemon* wild_pk, int outcome, bool* used_pk);
