#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexer.h"
#include "parser.h"

void lexer_next_token(TOKEN_NODE **token_node, TOKEN **token) {
    *token = &(*token_node)->currentToken;
    *token_node = (*token_node)->nextToken;
}

bool expected_token_type(TOKEN_TYPE actualType, TOKEN_TYPE expectedType) {
    if (actualType != expectedType) {
        printf("Error: Expected %s, got %s\n", expectedType, actualType);
        return false;
    }
    return true;
}

AST* parse(TOKEN_NODE *token_node) {

    AST *ast = malloc(sizeof(AST));
    TOKEN *token = NULL;

    lexer_next_token(&token_node, &token);
    if (!expected_token_type(token->type, FIND)) {
        return NULL;
    }
    printf("Token type here: %s\n", type_to_string(token->type));
    FIND_IDENTIFIER_NODE find_identifier_node;
    find_identifier_node.search_type_token = token;
    
    lexer_next_token(&token_node, &token);
    if (!expected_token_type(token->type, SEARCH_LIMIT_TOKEN)) {
        return NULL;
    }
    find_identifier_node.limit_type_token = token;

    lexer_next_token(&token_node, &token);
    if (!expected_token_type(token->type, player)) {
        return NULL;
    }
    find_identifier_node.search_category_token = token;
    ast->find_identifier = find_identifier_node;

    lexer_next_token(&token_node, &token);
    if (!expected_token_type(token->type, WHERE)) {
        return NULL;
    }
    WHERE_IDENTIFIER where_identifier;
    where_identifier.where_condition_token = token;

    WHERE_IDENTIFIER_NODE *where_identifier_node = malloc(sizeof(WHERE_IDENTIFIER_NODE));;
    where_identifier_node->where_identifier = where_identifier;
    where_identifier_node->next_where_identifier = NULL;
    
    ast->where_identifier_list = where_identifier_node;

    return ast;
}

void print_ast(AST *ast) {
    printf("Token type in AST: %s\n", type_to_string(ast->find_identifier.search_type_token->type));
    printf("Token type in AST: %s\n", type_to_string(ast->find_identifier.limit_type_token->type));
    printf("Token type in AST: %s\n", type_to_string(ast->find_identifier.search_category_token->type));
    printf("Token type in AST: %s\n", type_to_string(ast->where_identifier_list->where_identifier.where_condition_token->type));

    printf("\n");
}

void free_ast(AST *ast) {

    FIND_IDENTIFIER_NODE find_node = ast->find_identifier;

    free(find_node.search_type_token->content);
    free(find_node.limit_type_token->content);
    free(find_node.search_category_token->content);

    free(find_node.search_type_token);
    free(find_node.limit_type_token);
    free(find_node.search_category_token);


    WHERE_IDENTIFIER_NODE *where_node_head = ast->where_identifier_list;
    WHERE_IDENTIFIER_NODE *tmp;

   while (where_node_head != NULL)
    {
       tmp = where_node_head;
       where_node_head = where_node_head->next_where_identifier;
       free(tmp->where_identifier.where_condition_token);
       free(tmp);
    }
    
    free(ast);
}