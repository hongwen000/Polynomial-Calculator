%{
    #include "token.h"
    #include "calc_interface.h"
    int pos = 1;

    extern "C" {
    int yywrap(void);
    int yylex(void);
    }

    int yywrap(void) {
        pos = 1;
        return 1;
    }

    void next(void)
    {
        pos += yyleng;
    }
%}

%option header-file="scanner.h"
%option outfile="scanner.cpp"

%%
"x" {next(); return VAR;}
[" "\t]+ {next(); continue;}
"\n" {pos = 1; continue;}
print {next(); return PRINT;}
"print all" {next(); return PRINT_ALL;}
-?(([0-9]+)|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?) {next(); yylval.getNum() = yytext; return NUM;}
[a-zA-Z\_][a-zA-Z\_0-9]* {next(); yylval.getID() = yytext; return ID;}
"+" {next(); return ADD;}
"-" {next(); return SUB;}
"*" {next(); return MUL;}
"(" {next(); return LPAREN;}
")" {next(); return RPAREN;}
"'" {next(); return DERIVE;}
"^" {next(); return EXP;}
"=" {next(); return ASSIGN;}
"==" {next(); return EQUAL;}
"/" {next(); return DIV;}
"%" {next(); return MOD;}
. {calc_interface::scanner_error.illegal_token = true; calc_interface::scanner_error.illegal_token_pos = pos; pos = 1; return ERROR;}
%%

