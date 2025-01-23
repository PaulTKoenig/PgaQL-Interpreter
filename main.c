#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

#define CHUNK_SIZE 256

int main(void) {

    // char input[] = "CHART golfers IN scatter_plot FOR driving_distance VS score WHERE tournament = Masters";

    char *input = malloc(CHUNK_SIZE * sizeof(char));  // Start with an initial buffer
    if (input == NULL) {
        perror("malloc failed");
        return 1;
    }

    int buffer_size = CHUNK_SIZE;
    int position = 0;
    int ch;

    // Read characters one at a time until EOF or newline is encountered
    while ((ch = getchar()) != EOF && ch != '\n') {
        // If we have reached the end of the current buffer, reallocate more space
        if (position >= buffer_size - 1) {
            buffer_size += CHUNK_SIZE;  // Increase buffer size by CHUNK_SIZE
            input = realloc(input, buffer_size * sizeof(char));
            if (input == NULL) {
                perror("realloc failed");
                return 1;
            }
        }

        // Store the character in the buffer and move the position forward
        input[position++] = (char)ch;
    }

    // Null-terminate the string
    input[position] = '\0';
    // printf("You entered: %s\n", input);
    // return 0;




    TOKEN_NODE *token_list_head = lex(input);

    // print_token_list(token_list_head);

    AST *ast = parse(token_list_head);
    if (ast == NULL) {
        printf("Error: Invalid Syntax");
        return 0;
    }
    // print_ast(ast);

    char *query_string = interpret(ast);

    printf("%s\n", query_string);

    // CLEAN UP MEMORY
    free_token_list(token_list_head);
    free(ast);
    free(query_string);

    return 0;
}