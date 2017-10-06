%{
#include "token.h"
#include <iostream>
#include <iomanip>
#include "polynomial.h"
#include "calc_interface.h"
#include <string>
extern "C" {
    void yyerror (char const *s) {
        calc_interface::parser_error.synax_error = true;
    }
    extern int yylex(void);
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
%token DIV
%token MOD

%right ASSIGN
%left ADD SUB
%left MUL DIV MOD
%left DERIVE
%right NEG
%right EXP


%%
S   :   E           {calc_interface::statement_type = ASSIGN_STM; calc_interface::assign_stm_info.variable_name = "ANS"; calc_interface::assign_stm_info.parsed_input = $$;}
    |   ID ASSIGN E {calc_interface::statement_type = ASSIGN_STM; calc_interface::assign_stm_info.variable_name = $1.getID();  calc_interface::assign_stm_info.parsed_input = $3;}
    |   ID EQUAL ID {calc_interface::statement_type = TEST_EQUAL_STM; calc_interface::test_equal_stm_info.var1 = $1; calc_interface::test_equal_stm_info.var2 = $3;}
    |   PRINT ID    {calc_interface::statement_type = PRINT_STM; calc_interface::print_stm_info.var = $2;}
    |   PRINT_ALL   {calc_interface::statement_type = PRINT_STM; calc_interface::print_stm_info.var.getID() = "ALL";}
    ;
E   :   T								{$$ = $1;}
    |   ID								{$$ = calc_interface::getVariable($1.getID());}
    |   E ADD E							{$$ = $1 + $3;}
    |   E SUB E							{$$ = $1 - $3;}
    |   E MUL E							{$$ = $1 * $3;}
    |	E DIV E							{$$ = $1 / $3;}
    |   E DERIVE						{$$ = $1.derivative();}
    |   LPAREN E RPAREN					{$$ = $2;}
    |	LPAREN E RPAREN EXP N			{$$ = $2.poly_power($5.get(0));}
    |	SUB E				%prec NEG	{$$ = -$2;}
    |	ADD E				%prec NEG	{$$ = $2;}
    ;
T   :   N							  	{$$ = $1;}
    |       VAR							{$$ = Polynomial(1, 1);}
    |   N   VAR							{$$ = Polynomial($1.get(0), 1);}
    |       VAR EXP N       %prec EXP	{$$ = Polynomial(1, $3.get(0));}
    |   N   VAR EXP N       %prec EXP	{$$ = Polynomial($1.get(0), $4.get(0));}
    |   N       EXP N       %prec EXP	{$$ = Polynomial(std::pow($1.get(0), $3.get(0)), 0);}
    ;
N   :       NUM						  	{Polynomial::coef_type coef = std::stod($1.getNum()); $$ = Polynomial(coef, 0);}
    |	NUM MOD	NUM 					{Polynomial::coef_type coef = std::stol($1.getNum()) % std::stol($3.getNum()); $$ = Polynomial(coef, 0);}
    ;
%%

