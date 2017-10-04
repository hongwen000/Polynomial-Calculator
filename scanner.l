%{
    #include "token.h"
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
[[:digit:]]+ {next(); yylval = yytext; return NUM;}
[a-zA-Z\_][a-zA-Z\_0-9]* {next(); yylval = yytext; return ID;}
"+" {next(); return ADD;}
"-" {next(); return SUB;}
"*" {next(); return MUL;}
"(" {next(); return LPAREN;}
")" {next(); return RPAREN;}
"'" {next(); return DERIVE;}
"^" {next(); return EXP;}
"=" {next(); return ASSIGN;}
"==" {next(); return EQUAL;}
. {pos = 1; return ERROR;}
%%

