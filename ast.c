#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//=================================================
// These function prototypes are helper functions 
// that print out the AST in prefix notation.
// Their main purpose was to make sure that the
// AST parsed from the string is accurate.
//=================================================
void print_assign(Stmt *stmt);
void print_expression(Stmt *stmt);
void print_binary(Expr *expr);
void print_unary(Expr *expr);
void print_number(Expr *expr);
void print_ident(Expr *expr);

Stmt *make_assignment(Token ident, Expr *value)
{
  Stmt *stmt = malloc(sizeof(Stmt));
  stmt->stmt_type = Assignment;
  stmt->AssignmentStmt.ident = ident;
  stmt->AssignmentStmt.value = value;

  return stmt;
}

Stmt *make_expression(Expr *expr)
{
  Stmt *stmt = malloc(sizeof(Stmt));
  stmt->stmt_type = Expression;
  stmt->ExpressionStmt.expr = expr;

  return stmt;
}

Expr *make_binary(Expr *left, Expr *right, Token op)
{
  Expr *expr = malloc(sizeof(Expr));
  expr->expr_type = Binary;
  expr->expr_struct.BinaryExpr.op = op;
  expr->expr_struct.BinaryExpr.left = left;
  expr->expr_struct.BinaryExpr.right = right;

  return expr;
}

Expr *make_unary(Token left, Expr *right)
{
  Expr *expr = malloc(sizeof(Expr));
  expr->expr_type = Unary;
  expr->expr_struct.UnaryExpr.left = left;
  expr->expr_struct.UnaryExpr.right = right;

  return expr;
}

Expr *make_ident(Token ident)
{
  Expr *expr = malloc(sizeof(Expr));
  expr->expr_type = Ident;
  expr->expr_struct.IdentifierExpr = ident;

  return expr;
}

Expr *make_number(Token num)
{
  Expr *expr = malloc(sizeof(Expr));
  expr->expr_type = Num;
  expr->expr_struct.NumberExpr = num;

  return expr;
}

void pretty_print(Expr *expr)
{
  if (expr == NULL)
    return;

  switch (expr->expr_type)
  {
  case Binary:
    print_binary(expr);
    break;
  case Unary:
    print_unary(expr);
    break;
  case Num:
    print_number(expr);
    break;
  case Ident:
    print_ident(expr);
    break;
  }
}

void print_binary(Expr *expr)
{
  printf("(%s ", expr->expr_struct.BinaryExpr.op.lexeme);
  pretty_print(expr->expr_struct.BinaryExpr.left);
  printf(" ");
  pretty_print(expr->expr_struct.BinaryExpr.right);
  printf(")");
}

void print_unary(Expr *expr)
{
  printf("(%s ", expr->expr_struct.UnaryExpr.left.lexeme);
  printf(" ");
  pretty_print(expr->expr_struct.UnaryExpr.right);
  printf(")");
}

void print_number(Expr *expr)
{
  printf("%s", expr->expr_struct.NumberExpr.lexeme);
}

void print_ident(Expr *expr)
{
  printf("%s", expr->expr_struct.IdentifierExpr.lexeme);
}

void pretty_print_stmt(Stmt *stmt)
{
  if (stmt == NULL)
    return;
  switch (stmt->stmt_type)
  {
  case Assignment:
    print_assign(stmt);
    break;
  case Expression:
    pretty_print(stmt->ExpressionStmt.expr);
    break;
  }
}

void print_assign(Stmt *stmt)
{
  printf("(= %s ", stmt->AssignmentStmt.ident.lexeme);
  pretty_print(stmt->AssignmentStmt.value);
  printf(")");
}
