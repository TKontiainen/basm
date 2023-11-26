#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main(void)
{
    struct Tokens tokens = get_tokens_from_file("foo.asm");

    printf("TOKENS:\n");
    for (size_t i = 0; i < tokens.count; i++)
    {
        struct Token token = tokens.tokens[i];
        printf("    %s:%d:%d: %s\n", token.loc.file, token.loc.row, token.loc.col, token.text);
    }

    struct Ops ops = get_operations_from_tokens(tokens);

    printf("OPERATIONS:\n");
    for (size_t i = 0; i < ops.count; i++)
    {
        struct Op op = ops.ops[i];
        printf("    {op.instruction = %d, op.operands = %p}\n", op.instruction, (void*)op.operands);
    }

    return 0;
}
