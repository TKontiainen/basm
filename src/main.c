#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

int main(void)
{
    struct Tokens tokens = get_tokens_from_file("foo.asm");
    for (size_t i = 0; i < tokens.count; i++)
    {
        printf("%.*s\n", tokens.tokens[i].size, tokens.tokens[i].text);
    }

    return 0;
}
