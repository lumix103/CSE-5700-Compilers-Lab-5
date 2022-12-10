#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

typedef struct Parser {
  Lexer lexer;
  Token current, peek;
} Parser;

Parser new_parser(char *input);

Stmt *parse(char *input);

#endif // PARSER_H
