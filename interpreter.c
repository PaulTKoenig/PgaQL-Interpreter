#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "interpreter.h"
#include "lexer.h"

size_t get_token_list_length(SQL_IDENTIFIER_TOKEN_NODE *head) {
    size_t length = 0;
    size_t num_tokens = 0;

    while (head != NULL) {
        length += head->current_token.token_length;
        num_tokens += 1;
        head = head->next_token;
    }

    size_t num_spaces = num_tokens - 1;

    return length + num_spaces;
}

void append_to_list(SQL_IDENTIFIER_TOKEN_NODE** token_node, SQL_IDENTIFIER_TOKEN_NODE *new_token_node) {
    if (*token_node == NULL) {
        *token_node = new_token_node;
        return;
    }
    
    SQL_IDENTIFIER_TOKEN_NODE *temp = *token_node;
    while (temp->next_token != NULL) {
        temp = temp->next_token;
    }
    temp->next_token = new_token_node;
}

void append_identifier_to_query(SQL_IDENTIFIER_TOKEN_NODE** token_node, TOKEN* token) {
    // size_t token_length = token->token_length;

    // strncpy((*query)+*current_position, token->content, token_length);
    // *current_position += token_length;
    // (*query)[*current_position] = ' ';
    // (*current_position)++;

    SQL_IDENTIFIER_TOKEN_NODE *new_token_node = malloc(sizeof(SQL_IDENTIFIER_TOKEN_NODE));

    char *token_content = malloc(token->token_length * sizeof(char));
    strncpy(token_content, token->content, token->token_length);
    SQL_IDENTIFIER_TOKEN new_token = { token_content, token->token_length };
    
    new_token_node->current_token = new_token;
    new_token_node->next_token = NULL;

    
    append_to_list(token_node, new_token_node);
}

void append_column_identifier_to_query(SQL_IDENTIFIER_TOKEN_NODE** token_node, TOKEN* token) {

    SQL_IDENTIFIER_TOKEN_NODE *new_token_node = malloc(sizeof(SQL_IDENTIFIER_TOKEN_NODE));

    char *token_content = malloc((token->token_length+1) * sizeof(char));
    strncpy(token_content, token->content, token->token_length);
    token_content[token->token_length] = ',';
    SQL_IDENTIFIER_TOKEN new_token = { token_content, token->token_length+1 };
    
    new_token_node->current_token = new_token;
    new_token_node->next_token = NULL;

    
    append_to_list(token_node, new_token_node);
}



void append_column_identifier_to_query_fixed(SQL_IDENTIFIER_TOKEN_NODE** token_node, TOKEN* token, TOKEN* aggregate_token, bool lastColumnIdentifier) {

    SQL_IDENTIFIER_TOKEN_NODE *new_token_node = malloc(sizeof(SQL_IDENTIFIER_TOKEN_NODE));

    int query_identifier_length = token->token_length;
    if (!lastColumnIdentifier) {
        query_identifier_length++;
    }

    if (aggregate_token != NULL) {
        query_identifier_length += aggregate_token->token_length + 2;
    }

    char *token_content = malloc(query_identifier_length * sizeof(char));

    if (aggregate_token != NULL) {
        strncpy(token_content, aggregate_token->content, aggregate_token->token_length);
        strcat(token_content, "(");
    }

    strncat(token_content, token->content, token->token_length);

    if (aggregate_token != NULL) {
        strcat(token_content, ")");
    }

    if (!lastColumnIdentifier) {
        token_content[query_identifier_length-1] = ',';
    }

    SQL_IDENTIFIER_TOKEN new_token = { token_content, query_identifier_length };

    
    new_token_node->current_token = new_token;
    new_token_node->next_token = NULL;

    
    append_to_list(token_node, new_token_node);
}

char* convert_token_to_sql_identifier(TOKEN *token) {

    if (token->type == FIND) {
        return "SELECT";
    } else if (token->type == SEARCH_LIMIT_TOKEN) {
        if (strncmp(token->content, "FIRST", token->token_length) == 0) {
            return "ORDER BY playerId ASC LIMIT 1";
        } else if (strncmp(token->content, "LAST", token->token_length) == 0) {
            return "ORDER BY playerId DESC LIMIT 1";
        } else {
            return "";
        }
    } else if (token->type == EQUALS) {
        return "=";
    } else if (token->type == CHARTED_TOKEN_TYPE) {
        if (strncmp(token->content, "season_player_box_score", token->token_length) == 0
            || strncmp(token->content, "player_stats", token->token_length) == 0) {
            return "playerId";
        } else if (strncmp(token->content, "season_team_box_score", token->token_length) == 0
                    || strncmp(token->content, "game_team_box_score", token->token_length) == 0) {
            return "teamId";
        } else {
            return "";
        }
    } else {
        printf("Element is unknown: %s\n", type_to_string(token->type));
    }
}

void convert_and_append_identifier_to_query(SQL_IDENTIFIER_TOKEN_NODE** token_node, TOKEN* token) {

    SQL_IDENTIFIER_TOKEN_NODE *new_token_node = malloc(sizeof(SQL_IDENTIFIER_TOKEN_NODE));
    
    char* sql_identifier = convert_token_to_sql_identifier(token);
    SQL_IDENTIFIER_TOKEN new_token = { sql_identifier, strlen(sql_identifier) };
    new_token_node->current_token = new_token;
    new_token_node->next_token = NULL;

    
    append_to_list(token_node, new_token_node);
}

char* build_query_string(SQL_IDENTIFIER_TOKEN_NODE *token_node) {
    size_t query_length = get_token_list_length(token_node);

    char* query_string = malloc((query_length+1) * sizeof(char));
    size_t current_position = 0;

    while (token_node != NULL) {
        char* token_content = token_node->current_token.content;
        size_t token_length = token_node->current_token.token_length;

        strncpy(query_string+current_position, token_content, token_length);
        current_position += token_length;
        query_string[current_position] = ' ';
        current_position++;

        token_node = token_node->next_token;
    }
    query_string[query_length] = '\0'; // null-terminate
    return query_string;
}

void print_sql_token_list(SQL_IDENTIFIER_TOKEN_NODE *token_node) {
    while (token_node != NULL) {
        printf("%s\n", token_node->current_token.content);
        // printf("%d\n", token_node->current_token.token_length);
        token_node = token_node->next_token;
    }
    printf("\n");
}

char* interpret(AST* ast) {
    
    SQL_IDENTIFIER_TOKEN_NODE *sql_identifier_token_node = NULL;

    CHART_IDENTIFIER_NODE chart_identifier_node = ast->chart_identifier;
    WHERE_IDENTIFIER_NODE *where_identifier_node = ast->where_identifier_list;

    TOKEN select_token = {SELECT, "SELECT", 6};
    TOKEN player_stats_token = {PLAYER_STATS, "player_stats", 12};
    TOKEN from_token = {FROM, "FROM", 4};
    TOKEN where_token = {WHERE, "WHERE", 5};
    TOKEN and_token = {AND, "AND", 3};
    TOKEN equals_token = {EQUALS, "=", 1};
    TOKEN group_by_token = {GROUP_BY, "GROUP BY", 8};
    TOKEN comma_token = {COMMA, ",", 1};

    append_identifier_to_query(&sql_identifier_token_node, &select_token);
    
    convert_and_append_identifier_to_query(&sql_identifier_token_node, chart_identifier_node.charted_token);
    append_identifier_to_query(&sql_identifier_token_node, &comma_token);

    append_column_identifier_to_query_fixed(&sql_identifier_token_node, chart_identifier_node.x_axis_token, chart_identifier_node.x_axis_aggregate_token, false);
    append_column_identifier_to_query_fixed(&sql_identifier_token_node, chart_identifier_node.y_axis_token, chart_identifier_node.y_axis_aggregate_token, true);

    append_identifier_to_query(&sql_identifier_token_node, &from_token);
    append_identifier_to_query(&sql_identifier_token_node, &player_stats_token);

    bool first_where = true;

    while (where_identifier_node != NULL) {

        if (first_where) {
            append_identifier_to_query(&sql_identifier_token_node, &where_token);
            first_where = false;
        } else {
            append_identifier_to_query(&sql_identifier_token_node, &and_token);
        }

        append_identifier_to_query(&sql_identifier_token_node, where_identifier_node->where_identifier.where_field_token);
        convert_and_append_identifier_to_query(&sql_identifier_token_node, &equals_token);
        append_identifier_to_query(&sql_identifier_token_node, where_identifier_node->where_identifier.where_condition_token);
        
        where_identifier_node = where_identifier_node->next_where_identifier;
    }

    if (chart_identifier_node.x_axis_aggregate_token != NULL && chart_identifier_node.y_axis_aggregate_token != NULL) {
        append_identifier_to_query(&sql_identifier_token_node, &group_by_token);
        convert_and_append_identifier_to_query(&sql_identifier_token_node, chart_identifier_node.charted_token);
    }

    // print_sql_token_list(sql_identifier_token_node);

    char* query_string = build_query_string(sql_identifier_token_node);

    return query_string;
}

char* get_x_column_name(AST* ast) {

    SQL_IDENTIFIER_TOKEN_NODE *sql_identifier_token_node = NULL;

    CHART_IDENTIFIER_NODE chart_identifier_node = ast->chart_identifier;

    append_column_identifier_to_query_fixed(&sql_identifier_token_node, chart_identifier_node.x_axis_token, NULL, true);
    char* x_column_name = build_query_string(sql_identifier_token_node);

    return x_column_name;
}

char* get_y_column_name(AST* ast) {

    SQL_IDENTIFIER_TOKEN_NODE *sql_identifier_token_node = NULL;

    CHART_IDENTIFIER_NODE chart_identifier_node = ast->chart_identifier;

    append_column_identifier_to_query_fixed(&sql_identifier_token_node, chart_identifier_node.y_axis_token, NULL, true);
    char* y_column_name = build_query_string(sql_identifier_token_node);

    return y_column_name;
}