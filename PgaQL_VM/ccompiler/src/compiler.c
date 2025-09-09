#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "compiler.h"
#include "lexer.h"
#include "opcodes.h"

void append_instruction(InstructionBuilder* ib, Instruction instr) {
    if (!ib->instructions) {
        ib->capacity = 8;
        ib->instructions = malloc(sizeof(Instruction) * ib->capacity);
        if (!ib->instructions) exit(1);
    } else if (ib->len >= ib->capacity) {
        ib->capacity *= 2;
        Instruction* tmp = realloc(ib->instructions, sizeof(Instruction) * ib->capacity);
        if (!tmp) exit(1);
        ib->instructions = tmp;
    }
    ib->instructions[ib->len++] = instr;
}

Instruction* compile(AST* ast, int* out_len) {
    InstructionBuilder ib = {NULL, 0, 8};

    Instruction instr1 = {OP_SCAN, {"games", NULL}};
    append_instruction(&ib, instr1);

    Instruction instr2 = {OP_LOAD_CONST, {"season", NULL}};
    append_instruction(&ib, instr2);

    *out_len = ib.len;
    return ib.instructions;
}