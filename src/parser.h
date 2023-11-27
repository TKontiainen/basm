#ifndef PARSER_H_
#define PARSER_H_

#include <stdlib.h>
#include "lexer.h"

enum Instruction
{
    MOV,
    SYSCALL
};

enum Register
{
    // 8-bit
    AL, BL, CL, DL, SPL, BPL, SIL, DIL, AH, BH, CH, DH,

    // 16-bit
    AX, BX, CX, DX, SP, BP, SI, DI, IP,

    // 32-bit
    EAX, EBX, ECX, EDX, ESP, EBP, ESI, EDI, EIP,

    // 64-bit
    RAX, RBX, RCX, RDX, RSP, RBP, RSI, RDI, RIP
};

enum OperandType
{
    REGISTER,
    IMMEDIATE
};

union OperandValue
{
    enum Register reg;
    int64_t imm;
};

struct Operand
{
    enum OperandType type;
    union OperandValue value;
};

struct Op
{
    enum Instruction instruction;
    struct Operand * operands;
};

struct Ops
{
    struct Op * ops;
    size_t count;
};

struct Ops
get_operations_from_tokens(struct Tokens tokens);

#endif /* PARSER_H_ */

