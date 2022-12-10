#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include <stdbool.h>
#include <string.h>

Parser new_parser(char *input);
//=================================================
// Helper functions that helped determine the next
// type of token. 
//=================================================
bool match_peek(Parser *p, TokenType ttype);
bool match_current(Parser *p, TokenType ttype);
bool expect_peek(Parser *p, TokenType ttype);

Stmt *parse_assignment(Parser *p);
Stmt *parse_expression_stmt(Parser *p);

Expr *parse_expression(Parser *p, int precedence);
Expr *parse_identifier(Parser *p);
Expr *parse_number(Parser *p);
Expr *parse_unary(Parser *p);
Expr *parse_group(Parser *p);
Expr *parse_binary(Parser *p, Expr *left);

Expr *prefix(Parser *p);
Expr *infix(Parser *p, Expr *expr);

int get_precedence(TokenType ttype);
int peek_precedence(Parser *p);

bool has_prefix(TokenType ttype);
bool has_infix(TokenType ttype);

Stmt *parse(char *input) {
  Parser p = new_parser(input);
  if (match_current(&p, Identifier) && match_peek(&p, Assign)) {
    return parse_assignment(&p);
  } else {
    return parse_expression_stmt(&p);
  }
}

void next_token(Parser *p) {
  if (p == NULL)
    return;
  p->current = p->peek;
  p->peek = next(&p->lexer);
}

Parser new_parser(char *input) {
  Parser p;

  p.lexer = new_lexer(input, strlen(input));
  next_token(&p);
  next_token(&p);
  return p;
}

bool match_current(Parser *p, TokenType ttype) {
  return p->current.ttype == ttype;
}

bool match_peek(Parser *p, TokenType ttype) { return p->peek.ttype == ttype; }

bool expect_peek(Parser *p, TokenType ttype) {
  if (p->peek.ttype == ttype) {
    next_token(p);
    return true;
  } else {
    return false;
  }
}

Stmt *parse_assignment(Parser *p) {
  Token ident = p->current;
  next_token(p); // Consume the identifier
  next_token(p); // Consume the '='
  Expr *value = parse_expression(p, 0);
  return make_assignment(ident, value);
}

Stmt *parse_expression_stmt(Parser *p) {
  return make_expression(parse_expression(p, 0));
}

Expr *parse_expression(Parser *p, int precedence) {
  if (!has_prefix(p->current.ttype)) {
    // TODO: Error Handling
    return NULL;
  }
  Expr *left = prefix(p);

  while (precedence < peek_precedence(p)) {
    if (!has_infix(p->peek.ttype)) {
      return left;
    }
    next_token(p);
    left = infix(p, left);
  }
  return left;
}

Expr *prefix(Parser *p) {
  TokenType ttype = p->current.ttype;
  switch (ttype) {
  case Identifier:
    return parse_identifier(p);
    break;
  case Number:
    return parse_number(p);
    break;
  case Minus:
    return parse_unary(p);
    break;
  case LParen:
    return parse_group(p);
    break;
  default:
    return NULL;
    break;
  }
}

Expr *infix(Parser *p, Expr *left) {
  TokenType ttype = p->current.ttype;
  switch (ttype) {
  case Plus:
  case Minus:
  case Mult:
  case Div:
    return parse_binary(p, left);
    break;
  default:
    return NULL;
    break;
  }
}

Expr *parse_identifier(Parser *p) { return make_ident(p->current); }

Expr *parse_number(Parser *p) { return make_number(p->current); }

Expr *parse_unary(Parser *p) {
  Expr *expr = make_unary(p->current, NULL);
  next_token(p);
  expr->expr_struct.UnaryExpr.right = parse_expression(p, 5);
  return expr;
}

Expr *parse_group(Parser *p) {
  next_token(p);
  Expr *expr = parse_expression(p, 1);
  if (!expect_peek(p, RParen)) {
    printf("error: expected \')\', got %s", p->peek.lexeme);
    return NULL;
  }
  return expr;
}

Expr *parse_binary(Parser *p, Expr *left) {
  Expr *expr = make_binary(left, NULL, p->current);

  int prec = get_precedence(p->current.ttype);
  next_token(p);
  expr->expr_struct.BinaryExpr.right = parse_expression(p, prec);

  return expr;
}

int get_precedence(TokenType ttype) {
  switch (ttype) {
  case Div:
  case Mult:
    return 4;
    break;
  case Minus:
  case Plus:
    return 3;
    break;
  case Assign:
    return 2;
    break;
  default:
    return 1;
    break;
  }
}

int peek_precedence(Parser *p) { return get_precedence(p->peek.ttype); }

bool has_prefix(TokenType ttype) {
  switch (ttype) {
  case Identifier:
  case Number:
  case Minus:
  case LParen:
    return true;
    break;
  default:
    return false;
    break;
  }
}

bool has_infix(TokenType ttype) {
  switch (ttype) {
  case Plus:
  case Minus:
  case Mult:
  case Div:
    return true;
    break;
  default:
    return false;
    break;
  }
}
