#pragma once
#include <stdio.h>
#include <ctype.h>

// Skips all characters until the next comma of the file.
void goto_next_comma_or_line(FILE* f);

// Skips all characters until the next new line of the file.
void goto_next_line(FILE* f);

// Returns the given string with only upper case letters.
char* strtoupper(char* str);