#include <stdio.h>
#include "lexer.h"
#include "parser.h"

void usage(char* program_name)
{
    printf("USAGE: %s <file>\n", program_name);
    printf("    no assembler flags for now\n");
}

int main(int argc, char* argv[])
{
    char* program_name = argv[0];
    if (argc < 2) {
        printf("fatal: no inut file provided\n");
        usage(program_name);
        return 0;
    }

    char* file_path = argv[1];

    struct Tokens tokens = get_tokens_from_file(file_path);

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
