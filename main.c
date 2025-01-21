#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"



int main(void) {

    char input[] = "CHART golfers IN scatter_plot FOR driving_distance VS score"; // WHERE tournament = Masters";
    // "FIND LAST player WHERE"

    TOKEN_NODE *token_list_head = lex(input);
    print_token_list(token_list_head);

    AST *ast = parse(token_list_head);
    print_ast(ast);

    char *query_string = interpret(ast);

    printf("%s\n", query_string);

    // CLEAN UP MEMORY
    free_token_list(token_list_head);
    free(ast);
    free(query_string);

    return 0;
}