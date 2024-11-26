#ifndef LEXER_H
#define LEXER_H

typedef enum {
    FIND,
    SEARCH_LIMIT_TOKEN,
    player,
    WHERE,
    INVALID_TOKEN
} TOKEN_TYPE;

typedef enum {
    FIRST,
    LAST,
    ALL,
    number,
    times,
    plus,
    invalid
} TOKEN_CONTENT;

typedef struct {
    TOKEN_TYPE type;
    char *content;
    size_t token_length;
} TOKEN;

typedef struct token_node {
    TOKEN currentToken;
    struct token_node *nextToken;
} TOKEN_NODE;

void append_token(TOKEN_NODE **list, TOKEN token);
void print_token(TOKEN token);
void print_token_list(TOKEN_NODE *head);
TOKEN_NODE* lex(char *input);
char* type_to_string(TOKEN_TYPE t);
void free_token_list(TOKEN_NODE *head);

#endif