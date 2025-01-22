#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
    TOKEN *charted_token;
    TOKEN *chart_type_token;
    TOKEN *x_axis_token;
    TOKEN *y_axis_token;
} CHART_IDENTIFIER_NODE;

typedef struct {
    TOKEN *search_type_token;
    TOKEN *limit_type_token;
    TOKEN *search_category_token;
} FIND_IDENTIFIER_NODE;

typedef struct {
    TOKEN *where_field_token;
    TOKEN *where_condition_token;
} WHERE_IDENTIFIER;

typedef struct where_identifier_node {
    WHERE_IDENTIFIER where_identifier;
    struct where_identifier_node *next_where_identifier;
} WHERE_IDENTIFIER_NODE;

typedef struct {
    CHART_IDENTIFIER_NODE chart_identifier;
    FIND_IDENTIFIER_NODE find_identifier;
    WHERE_IDENTIFIER_NODE *where_identifier_list;
} AST;

AST* parse(TOKEN_NODE* token);
void print_ast(AST *ast);
void free_ast(AST *ast);

#endif