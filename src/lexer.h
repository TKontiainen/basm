#ifndef LEXER_H_
#define LEXER_H_

#include <inttypes.h>

#define FILE_CAP 1024*1024 // The maximum size of a source file: 1MB
struct FileContents
{
    char* content;
    size_t length;
};

struct Location
{
    char* file;
    int row;
    int col;
};

struct Tokens
{
    struct Token * tokens;
    size_t count;
};

struct Token
{
    struct Location loc;
    char* text;
    size_t length;
};

struct FileContents
get_file_contents(char* file_path);

struct Tokens
get_tokens_from_file(char* file_path);

#endif /* LEXER_H */