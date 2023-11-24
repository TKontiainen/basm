#ifndef LEXER_H_
#define LEXER_H_

#include <stdlib.h>

#define FILE_CAP 1024*1024 // The maximum size of a source file: 1MB
struct File_Contents {
    char* content;
    size_t length;
};

struct Tokens {
    struct Token* tokens;
    size_t count;
};

struct Token {
    char* text;
    size_t size;
};

struct File_Contents get_file_contents(const char* file_path);
int notspace(int argument);
int find(const char* text, size_t length, int start, int f (int) );
struct Tokens get_tokens_from_file(const char* file_path);

#endif