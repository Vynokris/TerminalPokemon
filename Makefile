# List of c files to build.
OBJS = parsing/parser_helper.o parsing/pokemons/pokemon_parser.o parsing/moves/moves_parser.o parsing/pokemon_moves/pokemon_moves_parser.o src/species.o src/game_functions.o src/game.o

# List of parameters for build (Preprocessor, C compilation, and Linking).
CPPFLAGS = 
CFLAGS = -O0 -g -Wall -lm
LDFLAGS = 

# .PHONY: Rules that won't generate files
.PHONY: run clean

# First rule, default one to be executed when launching make without argument.
game.out: $(OBJS)
	-gcc -o $@ $^ $(CFLAGS)
	
# Pattern matching (compiles all the OBJS files).
%.o: %.c
	gcc -c -o $@ $< $(CFLAGS)

# Other rules, executed when launching make with arguments.
tests/pokemon_parser_test.out: parsing/pokemons/pokemon_parser.o
	gcc -o $@ $^ $(CFLAGS)

tests/moves_parser_test.out: parsing/moves/moves_parser.of
	gcc -o $@ $^ $(CFLAGS)

tests/pokemon_moves_parser_test.out: parsing/pokemon_moves/pokemon_moves_parser.o
	gcc -o $@ $^ $(CFLAGS)

tests/species_test.out: src/species.o parsing/pokemons/pokemon_parser.o parsing/pokemon_moves/pokemon_moves_parser.of
	gcc -o $@ $^ $(CFLAGS)

run: game.out
	game.out

clean:
	rm -f $(OBJS) game.out