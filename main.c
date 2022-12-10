//=================================================
// Name: Fabs
// Class: CSE-5700 01
// Project: Lab 5
// Project Desc: scans a string that is a
// mathematical expression as input and generate
// machine code for it
//=================================================
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "tac.h"

#define MAX_BUF 256

char op[2], arg1[5], arg2[5], result[5];

int main(int argc, char **argv)
{
  char *input = malloc(sizeof(char) * MAX_BUF);
  printf("Enter your math equation: \n");
  printf("> ");
  fgets(input, MAX_BUF, stdin);
  if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n'))
    input[strlen(input) - 1] = '\0';
  Stmt *stmt = parse(input);
  // pretty_print_stmt(stmt);
  // printf("\n");
  generate(stmt, "input.txt");

  FILE *fp1, *fp2;
  fp1 = fopen("input.txt", "r");
  fp2 = fopen("output.txt", "w");
  while (!feof(fp1))
  {
    fscanf(fp1, "%s%s%s%s", result, arg1, op, arg2);
    if (strcmp(op, "+") == 0)
    {
      fprintf(fp2, "\nLD RO, %s", arg1);
      fprintf(fp2, "\nADD RO, RO, %s", arg2);
      fprintf(fp2, "\nLD %s,RO", result);
    }
    if (strcmp(op, "*") == 0)
    {
      fprintf(fp2, "\nLD RO, %s", arg1);
      fprintf(fp2, "\nMULT RO,RO, %s", arg2);
      fprintf(fp2, "\nLD %s,RO", result);
    }
    if (strcmp(op, "-") == 0)
    {
      fprintf(fp2, "\nLD RO, %s", arg1);
      fprintf(fp2, "\nSUB RO, RO, %s", arg2);
      fprintf(fp2, "\nLD %s, RO", result);
    }
    if (strcmp(op, "/") == 0)
    {
      fprintf(fp2, "\nLD RO, %s", arg1);
      fprintf(fp2, "\nMULT RO,RO, %s", arg2);
      fprintf(fp2, "\nLD %s,RO", result);
    }
    if (strcmp(op, "=") == 0)
    {
      fprintf(fp2, "\nLD RO,%s", arg1);
      fprintf(fp2, "\nST %s,RO", result);
    }
  }
  fclose(fp1);
  fclose(fp2);
  return 0;
}
