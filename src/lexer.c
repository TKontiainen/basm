#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./lexer.h"

struct FileContents
get_file_contents(char* file_path)
{
    struct FileContents fc;

    FILE * f = fopen(file_path, "r");

    if (f == NULL) {
        printf("fatal: file %s not found\n", file_path);
        exit(1);
    }

    void* buf = malloc(FILE_CAP);

    fc.length = fread(buf, sizeof(char), FILE_CAP, f);
    fc.content = (char*)memcpy(malloc(fc.length), buf, fc.length);

    free(buf);

    fclose(f);

    return fc;
}

// Opposite of isspace
int
notspace(int argument)
{
    return !isspace(argument);
}

// Find the next character in a text according to a predicate
// Return the index if found, -1 if not found
int
find(const char* text, size_t length, int start, int predicate(int) )
{
    for (size_t i = start; i < length; i++)
    {
        char c = text[i];
        if (predicate((int)c)) return i;
    }
    return -1;
}

struct Location
get_location_from_index(char* file_path, const char* text, int index)
{
    int row = 1;
    int col = 1;
    for (int i = 0; i <= index; i++)
    {
        col++;
        if (text[i] == '\n')
        {
            row++;
            col = 1;
        }
    }
    
    struct Location loc = {
        .file = file_path,
        .row = row,
        .col = col
    };

    return loc;
}

struct Tokens
get_tokens_from_file(char* file_path)
{
    struct FileContents fc = get_file_contents(file_path);

    // Initialize the structure
    struct Tokens tokens = {
        .tokens = ( struct Token * )malloc( sizeof(struct Token) ),
        .count = 0
    };

    // Loop over the tokens in the file
    int index = find(fc.content, fc.length, 0, notspace);
    while (index >= 0)
    {
        tokens.count++;
        
        // Find the next space
        int token_end = find(fc.content, fc.length, index + 1, isspace);
        
        // Create the token structure
        struct Token token = {
            .loc = get_location_from_index(file_path, fc.content, index - 1),
            .text = strndup(fc.content + index, token_end - index), // The text (null terminated)
            .length = token_end - index                             // The size (without null byte)
        };

        // Append to list of tokens
        tokens.tokens[tokens.count - 1] = token;  

        // Resize tokens appropriately 
        tokens.tokens = ( struct Token * )realloc(tokens.tokens, ( tokens.count + 1) * sizeof(struct Token) );

        // Find the next token
        index = find(fc.content, fc.length, token_end + 1, notspace);
    }

    return tokens;
}

