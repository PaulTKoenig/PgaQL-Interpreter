#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"



int main(void) {

    char input[] = "FIND LAST player WHERE";
    // char input[] = "FIND    player   WITH THE (MOST, LEAST.. TOTAL), (HIGHEST, LOWEST .. AVG) BIRDIES   ACROSS ROUNDS 1,2,4   IN THE MASTERS   WHERE   player IS european";
    
    TOKEN_NODE *token_list_head = lex(input);
    print_token_list(token_list_head);

    AST *ast = parse(token_list_head);
    print_ast(ast);

    char *query_string = interpret(ast);

    printf("%s\n", query_string);

    // CLEAN UP MEMORY
    free_token_list(token_list_head);
    free_ast(ast);
    free(query_string);

    return 0;
}