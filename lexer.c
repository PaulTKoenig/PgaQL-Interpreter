#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "lexer.h"


const char *SEARCHABLE_FIELDS[] = {
    "team_abbr", "team_city", "player_name", "start_position",
    "min", "fgm", "fga", "fg_pct", "three_pm", "three_pa", "three_pct",
    "ftm", "fta", "ft_pct", "o_reb", "d_reb", "reb", "ast", "stl",
    "blk", "turnover", "pf", "pts", "plus_minus"
};
const int SEARCHABLE_FIELDS_SIZE = sizeof(SEARCHABLE_FIELDS) / sizeof(SEARCHABLE_FIELDS[0]); 


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
            printf("Token Type: CHART, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case WHERE_FIELD:
            printf("Token Type: WHERE_FIELD, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case EQUALS:
            printf("Token Type: EQUALS, Content: %c, Length: %d\n", *(token.content), token.token_length);
            break;
        case WHERE_VALUE:
            printf("Token Type: WHERE_VALUE, Content: %c, Length: %d\n", *(token.content), token.token_length);
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
        case INVALID_TOKEN:
            printf("Unknown Token Type, Content: %c, Length: %d\n", *(token.content), token.token_length);
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

bool compare_strings(const char* tokenIdentifier, const char* input, const size_t inputLength) {
    size_t tokenIdentifierLength = strlen(tokenIdentifier);
    if (tokenIdentifierLength != inputLength) {
        return false;
    }
    return strncmp(input, tokenIdentifier, inputLength) == 0;
}

bool exists_in_set(const char *set[], int set_size, const char *input, const size_t inputLength) {
    for (int i = 0; i < set_size; i++) {
        if (compare_strings(set[i], input, inputLength)) {
            return true;
        }
    }
    return false;
}
int set_token_type(TOKEN **token, char *input) {

    size_t tokenLength = 0;
    while (*(input+tokenLength) != '\0' && *(input+tokenLength) != ' ') {
        tokenLength++;
    }

    TOKEN *token_ptr = *token;

    if (compare_strings("FIND", input, tokenLength)) {
        token_ptr->type = FIND;
    } else if (compare_strings("tournament", input, tokenLength)) {
        token_ptr->type = WHERE_FIELD;
    } else if (compare_strings("=", input, tokenLength)) {
        token_ptr->type = EQUALS;
    } else if (compare_strings("Masters", input, tokenLength)) {
        token_ptr->type = WHERE_VALUE;
    } else if (compare_strings("CHART", input, tokenLength)) {
        token_ptr->type = CHART;
    } else if (compare_strings("IN", input, tokenLength)) {
        token_ptr->type = IN;
    } else if (compare_strings("FOR", input, tokenLength)) {
        token_ptr->type = FOR;
    } else if (exists_in_set(SEARCHABLE_FIELDS, SEARCHABLE_FIELDS_SIZE, input, tokenLength)) {
        token_ptr->type = AXIS_TOKEN_TYPE;
    } else if (compare_strings("VS", input, tokenLength)) {
        token_ptr->type = VS;
    } else if (compare_strings("players", input, tokenLength)) {
        token_ptr->type = CHARTED_TOKEN_TYPE;
    } else if (compare_strings("scatter_plot", input, tokenLength)) {
        token_ptr->type = CHART_TYPE;
    } else if (compare_strings("player", input, tokenLength)) {
        token_ptr->type = player;
    } else if (compare_strings("WHERE", input, tokenLength)) {
        token_ptr->type = WHERE;
    } else {
        token_ptr->type = INVALID_TOKEN;
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
        case WHERE_FIELD: return "WHERE_FIELD";
        case EQUALS: return "EQUALS";
        case WHERE_VALUE: return "WHERE_VALUE";
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