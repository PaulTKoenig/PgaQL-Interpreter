#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

#define CHUNK_SIZE 256

int main(void) {

    // char input[] = "CHART season_player_box_score IN scatter_plot FOR AVG pts VS SUM fgm WHERE team_abbr = 'CLE' AND player_id = '1627745'";
    char input[] = "CHART player_stats IN scatter_plot FOR fga VS fgm WHERE blk = '10'";
    // char *input = malloc(CHUNK_SIZE * sizeof(char));  // Start with an initial buffer
    // if (input == NULL) {
    //     perror("malloc failed");
    //     return 1;
    // }

    // int buffer_size = CHUNK_SIZE;
    // int position = 0;
    // int ch;

    // // Read characters one at a time until EOF or newline is encountered
    // while ((ch = getchar()) != EOF && ch != '\n') {
    //     // If we have reached the end of the current buffer, reallocate more space
    //     if (position >= buffer_size - 1) {
    //         buffer_size += CHUNK_SIZE;  // Increase buffer size by CHUNK_SIZE
    //         input = realloc(input, buffer_size * sizeof(char));
    //         if (input == NULL) {
    //             perror("realloc failed");
    //             return 1;
    //         }
    //     }

    //     // Store the character in the buffer and move the position forward
    //     input[position++] = (char)ch;
    // }

    // input[position] = '\0';


    TOKEN_NODE *token_list_head = lex(input);

    print_token_list(token_list_head);

    AST *ast = parse(token_list_head);
    if (ast == NULL) {
        printf("{\"status\": \"failure\", \"error_code\": %d, \"message\": \"%s\"}\n", 400, "ERROR MESSAGE");
        return 0;
    }
    print_ast(ast);

    char *query_string = interpret(ast);
    printf("%s\n", query_string);

    char *x_column_name = get_x_column_name(ast);
    printf("%s\n", x_column_name);
    char *y_column_name = get_y_column_name(ast);
    printf("%s\n", y_column_name);

    printf("{\"status\": \"success\", \"error_code\": %d, \"message\": { \"query\": \"%s\", \"x_column_name\": \"%s\", \"y_column_name\": \"%s\" }}\n", 200, query_string, x_column_name, y_column_name);

    // CLEAN UP MEMORY
    free_token_list(token_list_head);
    free(ast);
    free(query_string);
    free(input);

    return 0;
}