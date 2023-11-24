#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./lexer.h"

struct File_Contents get_file_contents(const char* file_path)
{
    struct File_Contents fc;

    FILE * f = fopen(file_path, "r");

    void* buf = malloc(FILE_CAP);

    fc.length = fread(buf, sizeof(char), FILE_CAP, f);
    fc.content = (char*)memcpy(malloc(fc.length), buf, fc.length);

    free(buf);

    fclose(f);

    return fc;
}

int notspace(int argument) { return !isspace(argument); }

int find(const char* text, size_t length, int start, int f (int) )
{
    for (size_t i = start; i < length; i++)
    {
        char c = text[i];
        if (f((int)c)) return i;
    }
    return -1;
}

struct Tokens get_tokens_from_file(const char* file_path)
{
    struct File_Contents fc = get_file_contents(file_path);

    struct Token* tokens = ( struct Token * )malloc( sizeof(struct Token) );
    size_t tokens_size = 0;
    int index = find(fc.content, fc.length, 0, notspace);
    while (index >= 0)
    {
        int token_end = find(fc.content, fc.length, index+1, isspace);
        size_t token_size = token_end - index;
        tokens[tokens_size] = (struct Token){
            .text = (char*)memcpy(malloc(token_size), fc.content + index, token_size),
            .size = token_size
        };
        tokens_size++;
        tokens = (struct Token*)realloc(tokens, (tokens_size + 1) * sizeof(struct Token));
        
        index = find(fc.content, fc.length, token_end + 1, notspace);
    }

    struct Tokens t = {
        .tokens = tokens,
        .count = tokens_size
    };

    return t;
}