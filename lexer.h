#ifndef LEXER_H
#define LEXER_H


typedef enum {
    WHERE_FIELD,
    EQUALS,
    WHERE_VALUE,
    CHART,
    CHARTED_TOKEN_TYPE,
    IN,
    CHART_TYPE,
    FOR,
    AXIS_TOKEN_TYPE,
    VS,
    FIND,
    SEARCH_LIMIT_TOKEN,
    player,
    WHERE,
    AND,
    INVALID_TOKEN
} TOKEN_TYPE;

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