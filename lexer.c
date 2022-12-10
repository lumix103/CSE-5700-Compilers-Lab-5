#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Token END = {.ttype = End, .lexeme = "End", .s_lexeme = 4}; // EOF or End Token

Lexer new_lexer(char *buffer, long length) {
  Lexer lexer = {.start = 0, .foward = 0, .buffer = buffer, .length = length};
  return lexer;
}

//=================================================
// Makes a token from a single character token.
//=================================================
Token simple_token(TokenType ttype, char c) {
  Token tok;

  tok.ttype = ttype;

  tok.lexeme = (char *)malloc(sizeof(char) * 2);
  tok.lexeme[0] = c;
  tok.lexeme[1] = '\0';
  tok.s_lexeme = 2;

  return tok;
}

char advance(Lexer *l) {
  if (l == NULL)
    return '\0';
  if (l->foward >= l->length)
    return '\0';
  l->foward = l->foward + 1;
  return l->buffer[l->foward - 1];
}

char peek(Lexer *l) {
  if (l->foward >= l->length)
    return '\0';
  return l->buffer[l->foward];
}

void skip_whitespace(Lexer *l) {
  if (l == NULL)
    return;
  while (peek(l) == ' ' || peek(l) == '\r' || peek(l) == '\t' ||
         peek(l) == '\n')
    advance(l);
}

Token identifier(Lexer *l);
Token number(Lexer *l);

Token next(Lexer *l) {
  if (l->foward >= l->length)
    return END;
  skip_whitespace(l);
  l->start = l->foward;
  char c = advance(l);
  Token tok;
  switch (c) {
  case '(':
    tok = simple_token(LParen, c);
    break;
  case ')':
    tok = simple_token(RParen, c);
    break;
  case '+':
    tok = simple_token(Plus, c);
    break;
  case '-':
    tok = simple_token(Minus, c);
    break;
  case '*':
    tok = simple_token(Mult, c);
    break;
  case '/':
    tok = simple_token(Div, c);
    break;
  case '=':
    tok = simple_token(Assign, c);
    break;
  case '\0':
    tok = END;
  default:
    if (isalpha(c) || c == '_')
      tok = identifier(l);
    else if (isdigit(c))
      tok = number(l);
    else
      tok = END;
    break;
  }
  return tok;
}

//=================================================
// Scans the following text for a valid indentifier
// An identifier is considered as 
// ([a-zA-Z_]+[a-zA-Z_0-9]*)
//=================================================
Token identifier(Lexer *l) {

  while (isalnum(peek(l)) || peek(l) == '_') {
    advance(l);
  }
  Token tok;
  tok.s_lexeme = l->foward - l->start + 1;
  tok.lexeme = (char *)malloc(sizeof(char) * tok.s_lexeme);
  tok.ttype = Identifier;
  memcpy(tok.lexeme, &l->buffer[l->start], tok.s_lexeme - 1);
  tok.lexeme[tok.s_lexeme - 1] = '\0';
  return tok;
}


//=================================================
// Scans the following text until a non digit
// character is found.
// Note: number will only scan for integers.
//=================================================
Token number(Lexer *l) {

  while (isdigit(peek(l))) {
    advance(l);
  }
  Token tok;
  tok.s_lexeme = l->foward - l->start + 1;
  tok.lexeme = (char *)malloc(sizeof(char) * tok.s_lexeme);
  tok.ttype = Identifier;
  memcpy(tok.lexeme, &l->buffer[l->start], tok.s_lexeme - 1);
  tok.lexeme[tok.s_lexeme - 1] = '\0';
  return tok;
}
