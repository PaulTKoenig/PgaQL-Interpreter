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

char *token_to_cstring(const TOKEN *token) {
    if (!token || !token->content) {
        return NULL;
    }
    char *copy = malloc(token->token_length + 1);
    if (!copy) {
        perror("malloc failed");
        exit(1);
    }
    memcpy(copy, token->content, token->token_length);
    copy[token->token_length] = '\0'; 
    return copy;
}

void append_token_instruction(InstructionBuilder *ib, OpCode op, TOKEN *token1, TOKEN *token2) {
    char *content1 = token_to_cstring(token1);
    char *content2 = token2 ? token_to_cstring(token2) : NULL;
    append_instruction(ib, (Instruction){op, {content1, content2}});
}

// void append_token_instruction(InstructionBuilder *ib, OpCode op, TOKEN *token) {
//     char *content = token_to_cstring(token);
//     append_instruction(ib, (Instruction){op, {content, NULL}});
// }


Instruction* compile(AST* ast, int* out_len) {
    InstructionBuilder ib = {NULL, 0, 8};


    CHART_IDENTIFIER_NODE chart_identifier_node = ast->chart_identifier;
    // WHERE_IDENTIFIER_NODE *where_identifier_node = ast->where_identifier_list;

    append_token_instruction(&ib, OP_SCAN, chart_identifier_node.charted_token, NULL);

    // PLACE EACH WHERE CLAUSE ON STACK AND CHECK IF EQ, ADDING ANY AND or OR

    // FILTER

    append_token_instruction(&ib, OP_PROJECT, chart_identifier_node.x_axis_token, chart_identifier_node.y_axis_token);

    append_instruction(&ib, (Instruction){OP_OUTPUT, {NULL, NULL}});


    *out_len = ib.len;
    return ib.instructions;
}
