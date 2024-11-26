#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

void append_token(TOKEN_NODE **list, TOKEN token) {

    TOKEN_NODE *newToken = malloc(sizeof(TOKEN_NODE));
    newToken->currentToken = token;
    newToken->nextToken = NULL;

    if (*list == NULL) {
        *list = newToken;
        return;
    }


    TOKEN_NODE *temp = *list;
    while (temp->nextToken != NULL) {
        temp = temp->nextToken;
    }
    temp->nextToken = newToken;
}

void print_token(TOKEN token) {
    switch (token.type) {
        case FIND:
            printf("Token Type: FIND, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case SEARCH_LIMIT_TOKEN:
            printf("Token Type: SEARCH_LIMIT_TOKEN, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case ALL:
            printf("Token Type: ALL, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case WHERE:
            printf("Token Type: WHERE, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case times:
            printf("Token Type: times, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case plus:
            printf("Token Type: plus, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        default:
            printf("Unknown Token Type, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
    }
}

void print_token_list(TOKEN_NODE *head) {
    TOKEN_NODE *current = head;
    
    while (current != NULL) {
        print_token(current->currentToken);
        current = current->nextToken;
    }
    printf("\n");
}

int set_token_type(TOKEN **token, char *input) {

    size_t tokenLength = 0;
    while (*(input+tokenLength) != '\0' && *(input+tokenLength) != ' ') {
        tokenLength++;
    }

    TOKEN *token_ptr = *token;

    if (strncmp(input, "FIND", tokenLength) == 0) {
        token_ptr->type = FIND;
    } else if (strncmp(input, "FIRST", tokenLength) == 0 || strncmp(input, "LAST", tokenLength) == 0 || strncmp(input, "ALL", tokenLength) == 0) {
        token_ptr->type = SEARCH_LIMIT_TOKEN;
    } else if (strncmp(input, "player", tokenLength) == 0) {
        token_ptr->type = player;
    } else if (strncmp(input, "WHERE", tokenLength) == 0) {
        token_ptr->type = WHERE;
    } else {
        printf("Element is unknown: %c\n", *input);
    }
    return tokenLength;
}

TOKEN_NODE* lex(char *input) {

    TOKEN_NODE *token_list_head = NULL;

    while (*input != '\0') {
        size_t token_length = 1;

        if (*input == ' ') {
            input++;
        } else {
            TOKEN *token = malloc(sizeof(TOKEN));
            token->type = invalid;

            token_length = set_token_type(&token, input);
            token->content = input;
            token->token_length = token_length;
            
            append_token(&token_list_head, *token);

            input += token_length;
        }
    }

    return token_list_head;
}

char* type_to_string(TOKEN_TYPE t) {
    switch(t) {
        case FIND: return "FIND";
        case SEARCH_LIMIT_TOKEN: return "SEARCH_LIMIT_TOKEN";
        case player: return "player";
        case WHERE: return "WHERE";
        case INVALID_TOKEN: return "INVALID_TOKEN";
        default: return "Unknown";
    }
}

void free_token_list(TOKEN_NODE *head) {
    TOKEN_NODE *tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->nextToken;
       free(tmp->currentToken.content);
       free(tmp);
    }
}