#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lexer.h"
#include "parser.h"

// Return the register on success -1 on failure
enum Register
get_register_from_token(struct Token token)
{
    if (strcmp(token.text, "%rax") == 0) return RAX;
    else if (strcmp(token.text, "%rbx") == 0) return RBX;
    else if (strcmp(token.text, "%rcx") == 0) return RCX;
    else if (strcmp(token.text, "%rdx") == 0) return RDX;
    else if (strcmp(token.text, "%rsp") == 0) return RSP;
    else if (strcmp(token.text, "%rbp") == 0) return RBP;
    else if (strcmp(token.text, "%rsi") == 0) return RSI;
    else if (strcmp(token.text, "%rdi") == 0) return RDI;
    else if (strcmp(token.text, "%rip") == 0) return RIP;
    else return -1;
}

struct Operand
get_operand_from_token(struct Token token)
{
    // Get the type of operand
    enum OperandType type;
    switch (token.text[0])
    {
        case '%': {
            type = REGISTER;
        } break;

        case '#': {
            type = IMMEDIATE;
        } break;

        default: {
            printf("%s:%d:%d: fatal: invalid operand type: %s\n", token.loc.file, token.loc.row, token.loc.col, token.text);
            exit(1);
        } break;
    }
    
    // Get the value of operand
    union OperandValue value;
    switch (type)
    {
        case REGISTER: {
            value.reg = get_register_from_token(token);
        } break;

        case IMMEDIATE: {
            value.imm = atoi(token.text);
        } break;

        default: {
            printf("unreachable\n");
            exit(1);
        } break;
    }

    struct Operand operand = {
        .type = type,
        .value = value
    };

    return operand;
}

struct Operand *
get_operands_from_tokens(enum Instruction instruction, struct Tokens tokens, size_t* index)
{
    struct Operand * operands;

    switch (instruction)
    {
        case MOV: {
            assert(*index + 2 < tokens.count); // Too few arguments provided
            struct Token t1 = tokens.tokens[*index + 1];
            struct Token t2 = tokens.tokens[*index + 2];
            *index += 2;

            operands = (struct Operand *)malloc( 2 * sizeof(struct Operand) );
            operands[0] = get_operand_from_token(t1);
            operands[1] = get_operand_from_token(t2);
        } break;

        case SYSCALL: {
            operands = NULL; // syscall doesn't take any operands
        } break;

        default: {
            printf("unreachable\n");
            exit(1);
        } break;
    }

    return operands;
}

enum Instruction
get_instruction_from_token(struct Token token)
{
    if (strcmp(token.text, "mov") == 0) return MOV;

    else if (strcmp(token.text, "syscall") == 0) return SYSCALL;

    else {
        fprintf(stderr, "%s:%d:%d: fatal: unknown instruction %s\n", token.loc.file, token.loc.row, token.loc.col, token.text);
        exit(1);
    }
}

struct Ops
get_operations_from_tokens(struct Tokens tokens)
{
    // Initialize the structure
    struct Ops ops = {
        .ops = ( struct Op *)malloc( sizeof(struct Op) ),
        .count = 0
    };

    // Loop over the tokens
    for (size_t index = 0; index < tokens.count; index++)
    {
        ops.count++;

        struct Token token = tokens.tokens[index];

        // Create the operation structure
        struct Op op = {
            // Every token that this loop processes should be an instruction (labels and such not yet implemented)
            .instruction = get_instruction_from_token(token),
            .operands = get_operands_from_tokens(op.instruction, tokens, &index)
        };
        
        // Append to list of operations
        ops.ops[ops.count - 1] = op;

        // Resize ops appropriately
        ops.ops = ( struct Op * )realloc(ops.ops, ( ops.count + 1) * sizeof(struct Op) );
    }

    return ops;
}

