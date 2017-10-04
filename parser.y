%{
#include "token.h"
#include <iostream>
#include <iomanip>
#define DEBUG
#ifdef DEBUG
# define DEBUG_PRINT(x) std::cout << std::endl << x << std::endl;
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif
extern "C" {
    void yyerror (char const *s) {
      fprintf (stderr, "%s\n", s);
    }
    extern int yylex(void);
}
void yyerror (const std::string &s)
{
    yyerror(s.c_str());
}
%}

%error-verbose

%token ID
%token VAR
%token NUM 
%token PRINT 
%token PRINT_ALL 
%token ADD 
%token SUB 
%token MUL 
%token LPAREN 
%token RPAREN 
%token DERIVE 
%token EXP 
%token ASSIGN 
%token EQUAL

%right ASSIGN
%left ADD SUB
%left MUL 
%left DERIVE
%right NEG
%right EXP


%%
S   :   E           { std::cout << "00000 "     << "ANS"  << " " << $1 << std::endl;}
    |   ID ASSIGN E { std::cout << "00000 "     << $1     << " " << $3 << std::endl;}
    |   ID EQUAL ID { std::cout << "00001 "     << $1     << " " << $3 << std::endl;}
    |   PRINT ID    { std::cout << "00010 "     << $2     << " " << std::endl;}
    |   PRINT_ALL   { std::cout << "00010 "     << "ALL"  << std::endl;}
    ;
E   :   T                             { DEBUG_PRINT("GRAMMER1") $$ = $1;}
    |   ID                            { DEBUG_PRINT("GRAMMER2") $$ = $1;}
    |   E ADD E                       { DEBUG_PRINT("GRAMMER3") $$ = $1 + " " + $3 + " " + std::string("+");}
    |   E SUB E                       { DEBUG_PRINT("GRAMMER4") $$ = $1 + " " + $3 + " " + std::string("-");}
    |   E MUL E                       { DEBUG_PRINT("GRAMMER5") $$ = $1 + " " + $3 + " " + std::string("*");}
    |   E DERIVE                      { DEBUG_PRINT("GRAMMER6") $$ = $1 + " " + std::string("'");}
    |   SUB LPAREN E RPAREN %prec EXP { DEBUG_PRINT("GRAMMER7") $$ = $3 + " " + std::string("~");}
    |       LPAREN E RPAREN           { DEBUG_PRINT("GRAMMER8") $$ = $2;}
    ;
T   :   N                             { DEBUG_PRINT("GRAMMER9") $$ = $1 + " " + "0";}
    |       VAR                       { DEBUG_PRINT("GRAMMERA") $$ = std::string("1") + " " + "1";}
    |   N   VAR                       { DEBUG_PRINT("GRAMMERB") $$ = $1               + " " + "0";}
    |       VAR EXP N       %prec EXP { DEBUG_PRINT("GRAMMERa") $$ = std::string("1") + " " + $3;}
    |   N   VAR EXP N       %prec EXP { DEBUG_PRINT("GRAMMERC") $$ = $1               + " " + $4;}
    |   N       EXP N       %prec EXP { DEBUG_PRINT("GRAMMERD") $$ = $1 + " " + $3 + " " + std::string("^");}
    ;
N   :       NUM                       {DEBUG_PRINT("GRAMEMRE") $$ = $1;}
    |   SUB NUM             %prec NEG {DEBUG_PRINT("GRAMEMRF") $$ = "-" + $2;}
    ;
%%

