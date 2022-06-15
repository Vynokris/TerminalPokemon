#include "parser_helper.h"


void goto_next_comma_or_line(FILE* f)
{
    char c = fgetc(f);
    while(c != ',' && c != '\n' && !feof(f)) {
        c = fgetc(f);
    }
}

void goto_next_line(FILE* f)
{
    char c = fgetc(f);
    while(c != '\n' && !feof(f)) {
        c = fgetc(f);
    }
}

char* strtoupper(char* str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}