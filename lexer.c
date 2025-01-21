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
        case CHART:
            printf("Token Type: SEARCH_LIMIT_TOKEN, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case CHARTED_TOKEN_TYPE:
            printf("Token Type: CHARTED_TOKEN_TYPE, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case IN:
            printf("Token Type: IN, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case CHART_TYPE:
            printf("Token Type: CHART_TYPE, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case FOR:
            printf("Token Type: FOR, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case AXIS_TOKEN_TYPE:
            printf("Token Type: AXIS_TOKEN_TYPE, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case VS:
            printf("Token Type: VS, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case SEARCH_LIMIT_TOKEN:
            printf("Token Type: SEARCH_LIMIT_TOKEN, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case WHERE:
            printf("Token Type: WHERE, Content: %c, Length: %d\n", *(token.content), token.token_length);
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
    } else if (strncmp(input, "CHART", tokenLength) == 0) {
        token_ptr->type = CHART;
    } else if (strncmp(input, "IN", tokenLength) == 0) {
        token_ptr->type = IN;
    } else if (strncmp(input, "FOR", tokenLength) == 0) {
        token_ptr->type = FOR;
    } else if (strncmp(input, "AXIS_TOKEN_TYPE", tokenLength) == 0) {
        token_ptr->type = AXIS_TOKEN_TYPE;
    } else if (strncmp(input, "VS", tokenLength) == 0) {
        token_ptr->type = VS;
    } else if (strncmp(input, "golfers", tokenLength) == 0) {
        token_ptr->type = CHARTED_TOKEN_TYPE;
    } else if (strncmp(input, "scatter_plot", tokenLength) == 0) {
        token_ptr->type = CHART_TYPE;
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
            token->type = INVALID_TOKEN;

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
        case CHART: return "CHART";
        case CHARTED_TOKEN_TYPE: return "CHARTED_TOKEN_TYPE";
        case IN: return "IN";
        case CHART_TYPE: return "CHART_TYPE";
        case FOR: return "FOR";
        case AXIS_TOKEN_TYPE: return "AXIS_TOKEN_TYPE";
        case VS: return "VS";
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
        free(tmp);
    }
}