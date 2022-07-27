#include <time.h>

#include "game_functions.h"

int main()
{
    srand(time(NULL));

    // ---------- GET ALL THE GAME DATA ---------- //

    printf("%s", C_DEFAULT);

    // Load all the database data (generation pokemon species and pokemon moves).
    get_gen1Species();
    get_move_list();
    get_player_inventory();
    player_team_options(); // Let the player choose if he wants to keep his team or change it.


    // ---------- MAIN GAME LOOP ---------- //
    while (true)
    {
        // ----- SETUP THE POKEMON BATTLE ----- //

        before_battle_actions();

        // Let the player choose to fight a random pokemon or a chosen pokemon.
        printf("%s1: Fight a random pokemon\n2: Chose a pokemon to fight\n%sYour choice: ", C_DEFAULT, C_GRAY);
        int chosen_battle_option = 0;
        scanf("%d", &chosen_battle_option);
        getchar();

        Species chosen_pk_species;
        int chosen_level = 0;
        if (chosen_battle_option == 2)
        {
            char chosen_pk[24];
            chosen_pk[0] = '\0';
            while (chosen_pk[0] == '\0') {
                // Let the player choose which pokemon to fight.
                printf("Enter the name or id of the pokemon you would like to fight: ");
                fgets(chosen_pk, 24, stdin);
                chosen_pk[strcspn(chosen_pk, "\n")] = '\0'; // Removes the newline at the end of the list.
            }

            // Get the opponent pokemon with its id.
            if (atoi(chosen_pk) != 0) {
                chosen_pk_species = get_species_by_id(atoi(chosen_pk));
            }
            else {
                chosen_pk_species = get_species_by_name(strtoupper(chosen_pk));
            }

            // Let the player choose the opponent pokemon's level.
            printf("Enter the level of the opponent pokemon: ");
            scanf("%d", &chosen_level);
            getchar();
            printf("%s\n\n\n", C_DEFAULT);
        }
        else
        {
            chosen_pk_species = get_species_by_id(rand() % 152);
            int average_lv = 0;
            for (int i = 0; i < 6; i++) {
                average_lv += player_team[i].lv;
            }
            average_lv /= 6;
            chosen_level = average_lv + rand() % 10;
        }

        // Initialize the opponent pokemon.
        Pokemon wild_pk;
        init_pk_instance(&wild_pk, &chosen_pk_species, chosen_level);


        // ----- START THE TURN BASED BATTLE ----- //

        // Holds a boolean for each of the player's pokemons.
        // The bool of a pk is set to true if it is used in battle.
        bool used_pk[6] = { true, false, false, false, false, false };

        // Play the whole battle and get the outcome.
        int battle_outcome = start_wild_battle(player_team, &wild_pk, used_pk);

        // End the battle according to the outcome.
        end_wild_battle(player_team, &wild_pk, battle_outcome, used_pk);


        // ----- SAVE THE PLAYER'S TEAM ----- //

        save_player_team();

        save_player_inventory();

        // Wait for player input to end the battle.
        printf("0: Quit\n1: Continue\n%sYour choice: ", C_GRAY);
        int start_next_battle;
        scanf("%d", &start_next_battle);
        getchar();
        printf("%s", C_DEFAULT);

        if (!start_next_battle) {
            return 0;
        }

        printf("\n\n");
    }
}