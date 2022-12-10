#ifndef AST_H
#define AST_H

#include "lexer.h"


//=================================================
// Structure for AST
// AST is seperated into two structures:
//  Expressions
//  Statements
//=================================================
typedef struct EXPR {
  enum { Binary, Ident, Num, Unary } expr_type;
  union {
    Token NumberExpr;
    Token IdentifierExpr;
    struct {
      struct EXPR *left;
      struct EXPR *right;
      Token op;
    } BinaryExpr;
    struct {
      struct EXPR *right;
      Token left;
    } UnaryExpr;
    struct {
      Token ident;
      struct EXPR *value;
    } AssignExpr;
  } expr_struct;
} Expr;

typedef struct STMT {
  enum { Assignment, Expression } stmt_type;
  union {
    struct {
      Token ident;
      Expr *value;
    } AssignmentStmt;
    struct {
      Expr *expr;
    } ExpressionStmt;
  };
} Stmt;


//=================================================
// More helper functiuns that make it easier to
// make an AST from a given token or expression.
//=================================================

Stmt *make_assignment(Token ident, Expr *expr);
Stmt *make_expression(Expr *expr);

Expr *make_binary(Expr *left, Expr *right, Token op);
Expr *make_unary(Token left, Expr *right);
Expr *make_number(Token num);
Expr *make_ident(Token ident);

void pretty_print_stmt(Stmt *stmt);
void pretty_print(Expr *expr);

#endif // AST_H
