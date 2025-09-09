#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"

typedef struct {
    int Op;
    const char* Args[2];
} Instruction;

typedef struct {
    Instruction* instructions;
    int len;
    int capacity;
} InstructionBuilder;

Instruction* compile(AST* ast, int* out_len);

#endif