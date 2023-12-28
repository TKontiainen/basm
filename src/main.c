#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include "lexer.h"
#include "parser.h"

typedef int8_t byte;

struct Bytes
{
    byte * content;
    size_t count;
};

struct Bytes *
concatenate_bytes(struct Bytes* dst, struct Bytes* src, size_t n)
{
    dst->content = (byte *)realloc(dst->content, dst->count + n);
    memcpy(dst->content + dst->count, src->content, n);
    dst->count += n;
    return dst;
}

size_t
write_bytes_to_file(struct Bytes bytes, char* file_path)
{
    if (file_path == NULL) {
        fprintf(stderr, "file_path is NULL. Using a.out\n");
        file_path = "a.out";
    }

    FILE * f = fopen(file_path, "w");
    if (f == NULL)
    {
        fprintf(stderr, "Couldn't open file %s\n", file_path);
        exit(1);
    }

    size_t ret = fwrite(bytes.content, 1, bytes.count, f);

    fclose(f);

    return ret;
}

// Get Register Width
#define GRW(r) (r >= RAX ? 64 : (r >= EAX ? 32 : (r >= AX ? 16 : 8)))

struct Bytes
mov_r_imm(enum Register reg)
{
    int width = GRW(reg);
    if (width == 64)
    {
        struct Bytes ret = {
            .content = (byte *)malloc(2),
            .count = 2
        };

        ret.content[0] = 0x48;
        ret.content[1] = 0xb8 + reg - RAX;

        return ret;
    }

    else 
    {
        fprintf(stderr, "Unsupported register width for mov instruction\n");
        exit(1);
    }
}

struct Bytes
lsb_immediate_bytes(int64_t imm, int width)
{
    struct Bytes bytes = {
        .content = (byte *)malloc(width / 8),
        .count = width / 8
    };

    for (int i = 0; i < width; i += 8)
    {
        bytes.content[i >> 3] = (imm >> i) & 0xff;
    }

    return bytes;
}    

struct Bytes
machine_code_from_op(struct Op op)
{
    if (op.instruction == MOV)
    {
        if (op.operands[0].type == REGISTER)
        {
            // first operand is a register
            if (op.operands[1].type == IMMEDIATE)
            {
                // second operand is an immediate
                struct Bytes bytes = mov_r_imm(op.operands[0].value.reg);
                struct Bytes immediate = lsb_immediate_bytes(op.operands[1].value.imm, GRW(op.operands[0].value.reg));
                concatenate_bytes(&bytes, &immediate, immediate.count);
                free(immediate.content);
                return bytes;
            }
            
            else
            {
                fprintf(stderr, "Unsupported operand types for MOV\n");
                exit(1);
            }
        }

        else
        {
            fprintf(stderr, "Unsupported operand types for MOV\n");
            exit(1);
        }
    }

    else if (op.instruction == SYSCALL)
    {
        struct Bytes bytes = {
            .content = (byte *)malloc(2),
            .count = 2
        };
        bytes.content[0] = 0x0f;
        bytes.content[1] = 0x05;
        return bytes;
    }

    else
    {
        fprintf(stderr, "Unsupported Instruction: %d (keep in mind that this is an enumeration not a string literal)\n", op.instruction);
        exit(1);
    }
}

void
usage(char* program_name)
{
    printf("USAGE: %s <file>\n", program_name);
    printf("    no assembler flags for now\n");
}

int 
main(int argc, char* argv[])
{
    char* program_name = argv[0];
    if (argc < 2) {
        usage(program_name);
        printf("fatal: no inut file provided\n");
        return 0;
    }

    char* file_path = argv[1];

    struct Tokens tokens = get_tokens_from_file(file_path);

//     Debug information
//     printf("TOKENS:\n");
//     for (size_t i = 0; i < tokens.count; i++)
//     {
//         struct Token token = tokens.tokens[i];
//         printf("    %s:%d:%d: %s\n", token.loc.file, token.loc.row, token.loc.col, token.text);
//     }

    struct Ops ops = get_operations_from_tokens(tokens);

    struct Bytes bytes = {
        .content = (byte *)malloc(0),
        .count = 0
    };

//    printf("OPERATIONS:\n");
    for (size_t i = 0; i < ops.count; i++)
    {
        struct Op op = ops.ops[i];
//        printf("    {op.instruction = %d, op.operands = %p}\n", op.instruction, (void*)op.operands);
        struct Bytes tmp = machine_code_from_op(op);
        concatenate_bytes(&bytes, &tmp, tmp.count);
        free(tmp.content);
    }

    // TODO: add flag for output file
    write_bytes_to_file(bytes, "a.out");

    return 0;
}
