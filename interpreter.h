#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

typedef struct {
    char *content;
    size_t token_length;
} SQL_IDENTIFIER_TOKEN;

typedef struct sql_identifier_token_node {
    SQL_IDENTIFIER_TOKEN current_token;
    struct sql_identifier_token_node *next_token;
} SQL_IDENTIFIER_TOKEN_NODE;

char* interpret(AST* ast);

#endif