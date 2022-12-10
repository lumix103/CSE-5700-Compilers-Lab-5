#include "tac.h"

#include <stdbool.h>
#include <string.h>
#include <errno.h>

bool is_atomic(int type);
char *get_atomic_lexeme(Expr *expr);

int generate_expr(Expr *expr, int *reg, FILE *file);
int generate_ident(Expr *expr, int *reg, FILE *file);
int generate_number(Expr *expr, int *reg, FILE *file);
int generate_unary(Expr *expr, int *reg, FILE *file);
int generate_binary(Expr *expr, int *reg, FILE *file);

void generate(Stmt *stmt, const char *dir)
{

    FILE *file = fopen(dir, "w");
    if (file == NULL)
    {
        printf("Error: %s\n", strerror(errno));
        return;
    }

    if (stmt == NULL)
        return;
    int reg = 1;
    int temp = 0;
    switch (stmt->stmt_type)
    {
    case Assignment:
        temp = generate_expr(stmt->AssignmentStmt.value, &reg, file);
        fprintf(file, "%s T%d = ?\n", stmt->AssignmentStmt.ident.lexeme, temp);
        break;
    case Expression:
        reg = 1;
        generate_expr(stmt->ExpressionStmt.expr, &reg, file);
        break;
    }

    fclose(file);
}
int generate_expr(Expr *expr, int *reg, FILE *file)
{
    int curr = *reg;
    switch (expr->expr_type)
    {
    case Ident:
        curr = generate_ident(expr, reg, file);
        break;
    case Num:
        curr = generate_number(expr, reg, file);
        break;
    case Unary:
        curr = generate_unary(expr, reg, file);
        *reg = *reg + 1;
        break;
    case Binary:
        curr = generate_binary(expr, reg, file);
        *reg = *reg + 1;
        break;
    }
    return curr;
}

int generate_ident(Expr *expr, int *reg, FILE *file)
{

    fprintf(file, "T%d %s _ _\n", *reg, expr->expr_struct.IdentifierExpr.lexeme);
    return *reg;
}

int generate_number(Expr *expr, int *reg, FILE *file)
{

    fprintf(file, "T%d %s _ _\n", *reg, expr->expr_struct.NumberExpr.lexeme);
    return *reg;
}

int generate_unary(Expr *expr, int *reg, FILE *file)
{

    if (expr->expr_struct.UnaryExpr.right->expr_type == Ident)
        fprintf(file, "T%d 0 - %s\n", *reg, expr->expr_struct.UnaryExpr.right->expr_struct.IdentifierExpr.lexeme);
    else if (expr->expr_struct.UnaryExpr.right->expr_type == Num)
        fprintf(file, "T%d 0 - %s\n", *reg, expr->expr_struct.UnaryExpr.right->expr_struct.NumberExpr.lexeme);
    else
    {
        int right = *reg;
        generate_expr(expr->expr_struct.UnaryExpr.right, reg, file);
        fprintf(file, "T%d 0 - T%d\n", *reg, right);
    }
    return *reg;
}

int generate_binary(Expr *expr, int *reg, FILE *file)
{

    if (is_atomic(expr->expr_struct.BinaryExpr.left->expr_type) && is_atomic(expr->expr_struct.BinaryExpr.right->expr_type))
    {
        fprintf(file, "T%d %s %s %s\n", *reg, get_atomic_lexeme(expr->expr_struct.BinaryExpr.left), expr->expr_struct.BinaryExpr.op.lexeme, get_atomic_lexeme(expr->expr_struct.BinaryExpr.right));
    }
    else if (!is_atomic(expr->expr_struct.BinaryExpr.left->expr_type) && !is_atomic(expr->expr_struct.BinaryExpr.right->expr_type))
    {
        int left = 
        generate_expr(expr->expr_struct.BinaryExpr.left, reg, file);
        int right = 
        generate_expr(expr->expr_struct.BinaryExpr.right, reg, file);
        fprintf(file, "T%d T%d %s T%d\n", *reg, left, expr->expr_struct.BinaryExpr.op.lexeme, right);
    }
    else if ((is_atomic(expr->expr_struct.BinaryExpr.left->expr_type) && !is_atomic(expr->expr_struct.BinaryExpr.right->expr_type)) ||
             (!is_atomic(expr->expr_struct.BinaryExpr.left->expr_type) && is_atomic(expr->expr_struct.BinaryExpr.right->expr_type)))
    {
        if (is_atomic(expr->expr_struct.BinaryExpr.left->expr_type))
        {
            int right =  generate_expr(expr->expr_struct.BinaryExpr.right, reg, file);
            fprintf(file, "T%d %s %s T%d\n", *reg, get_atomic_lexeme(expr->expr_struct.BinaryExpr.left), expr->expr_struct.BinaryExpr.op.lexeme, right);
        }
        else
        {
            int left = generate_expr(expr->expr_struct.BinaryExpr.left, reg, file);
            fprintf(file, "T%d T%d %s %s\n", *reg, left, expr->expr_struct.BinaryExpr.op.lexeme, get_atomic_lexeme(expr->expr_struct.BinaryExpr.right));
        }
    }
    return *reg;
}

bool is_atomic(int val)
{
    return val == Ident || val == Num;
}

char *get_atomic_lexeme(Expr *expr)
{
    switch (expr->expr_type)
    {
    case Ident:
        return expr->expr_struct.IdentifierExpr.lexeme;
        break;
    case Num:
        return expr->expr_struct.NumberExpr.lexeme;
        break;
    default:
        return NULL; // if reached good luck
        break;
    }
}
