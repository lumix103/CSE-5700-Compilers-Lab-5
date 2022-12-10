#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

typedef struct LEXER {
  int start, foward;
  long length;
  char *buffer;
} Lexer;

typedef enum TOKENTYPE {
  End,
  Plus,
  Minus,
  Mult,
  Div,
  Assign,
  LParen,
  RParen,
  Identifier,
  Number,
} TokenType;

typedef struct TOKEN {
  TokenType ttype; // Type of Token
  char *lexeme;
  long s_lexeme; // Size of Lexeme
} Token;

Lexer new_lexer(char *buffer, long length);

//=================================================
// Gets the next token from the given input.
// If no input remaining or an error has occurred
// then an EOF is produced.
//=================================================
Token next(Lexer *l);

#endif // LEXER_H
