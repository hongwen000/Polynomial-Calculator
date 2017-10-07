#ifndef H_TOKEN_H
#define H_TOKEN_H value
#include "polynomial.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define YYSTYPE Polynomial
#include "parser.h"

#ifndef VAR
typedef enum {
    ERROR = 256,
    ID = 258,
    VAR,
    NUM,
    PRINT,
    PRINT_ALL,
    ADD,
    SUB,
    MUL,
    LPAREN,
    RPAREN,
    DERIVE,
    EXP,
    ASSIGN,
    EQUAL
} TOKEN_TYPE;
#else
#define ERROR 256
#endif

static const char* token_strs[] = {
    "ID",
    "VAR",
    "NUM",
    "PRINT",
    "PRINT_ALL",
    "ADD",
    "SUB",
    "MUL",
    "LPAREN",
    "RPAREN",
    "DERIVE",
    "EXP",
    "ASSIGN",
    "EQUAL"
};

#define YYSTYPE Polynomial

typedef struct
{
    int type;
    std::string lval;
} TOKEN;

extern YYSTYPE yylval;

extern "C" {
extern int yylex(void);
extern int yyparse(void);
}

void scan(std::string str);

#endif /* ifndef H_TOKEN_H */
