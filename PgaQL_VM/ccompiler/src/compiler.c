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

    const char *src = token->content;
    size_t len = token->token_length;

    if (len >= 2 && 
        ((src[0] == '"' && src[len - 1] == '"') ||
         (src[0] == '\'' && src[len - 1] == '\''))) {
        src++;
        len -= 2;
    }

    char *copy = malloc(len + 1);
    if (!copy) {
        perror("malloc failed");
        exit(1);
    }
    memcpy(copy, src, len);
    copy[len] = '\0'; 
    return copy;
}

void append_token_instruction(InstructionBuilder *ib, OpCode op, TOKEN *token1, TOKEN *token2) {
    char *content1 = token_to_cstring(token1);
    char *content2 = token_to_cstring(token2);
    append_instruction(ib, (Instruction){op, {content1, content2}});
}

// void append_token_instruction(InstructionBuilder *ib, OpCode op, TOKEN *token) {
//     char *content = token_to_cstring(token);
//     append_instruction(ib, (Instruction){op, {content, NULL}});
// }


Instruction* compile(AST* ast, int* out_len) {
    InstructionBuilder ib = {NULL, 0, 8};


    CHART_IDENTIFIER_NODE chart_identifier_node = ast->chart_identifier;
    WHERE_IDENTIFIER_NODE *where_identifier_node = ast->where_identifier_list;

    append_token_instruction(&ib, OP_SCAN, chart_identifier_node.charted_token, NULL);

    // PLACE EACH WHERE CLAUSE ON STACK AND CHECK IF EQ, ADDING ANY AND or OR


    while (where_identifier_node != NULL) {

        // if (first_where) {
        //     append_identifier_to_query(&sql_identifier_token_node, &where_token);
        //     first_where = false;
        // } else {
        //     append_identifier_to_query(&sql_identifier_token_node, &and_token);
        // }

        // append_identifier_to_query(&sql_identifier_token_node, where_identifier_node->where_identifier.where_field_token);
        // convert_and_append_identifier_to_query(&sql_identifier_token_node, &equals_token);
        // append_identifier_to_query(&sql_identifier_token_node, where_identifier_node->where_identifier.where_condition_token);
        
        append_token_instruction(&ib, OP_LOAD_FIELD, where_identifier_node->where_identifier.where_field_token, NULL);
        append_token_instruction(&ib, OP_LOAD_CONST, where_identifier_node->where_identifier.where_condition_token, NULL);
        append_token_instruction(&ib, OP_EQ, NULL, NULL);

        where_identifier_node = where_identifier_node->next_where_identifier;
    }

    append_token_instruction(&ib, OP_FILTER, NULL, NULL);

    append_token_instruction(&ib, OP_PROJECT, chart_identifier_node.x_axis_token, chart_identifier_node.y_axis_token);

    append_instruction(&ib, (Instruction){OP_OUTPUT, {NULL, NULL}});


    *out_len = ib.len;
    return ib.instructions;
}
