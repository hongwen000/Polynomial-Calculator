#include "calc_interface.h"
#include "token.h"

QMap<QString, Polynomial> calc_interface::variable_table;
scanner_error_t calc_interface::scanner_error;
parser_error_t calc_interface::parser_error;
statement_type_t calc_interface::statement_type;
assign_stm_t calc_interface::assign_stm_info;
test_equal_stm_t calc_interface::test_equal_stm_info;
print_stm_t calc_interface::print_stm_info;

calc_interface::calc_interface()
{
}

process_result_t calc_interface::process_input_main(const QString& main_input, const QString& input_x)
{
    doParse(main_input);
    if (scanner_error.illegal_token) {
        return process_result_t(QString("illegal token at column") + QString::number(scanner_error.illegal_token_pos));
    }
    if (parser_error.synax_error) {
        return process_result_t(QString("synax error"));
    }
    if (parser_error.undefined_reference) {
        return process_result_t(QString::fromStdString("undefined reference to variable " + parser_error.var_name));
    }

    if (statement_type == ASSIGN_STM) {
        QString eval_result;
        if (input_x != "")
            eval_result = QString::number(assign_stm_info.parsed_input.eval(input_x.toDouble()));
        assign_stm_info.parsed_input.setID(assign_stm_info.variable_name);
        variable_table[QString::fromStdString(assign_stm_info.variable_name)] = assign_stm_info.parsed_input;
        return process_result_t(assign_stm_info.parsed_input.printPretty(), QString::fromStdString(assign_stm_info.variable_name), eval_result);
    } else if (statement_type == TEST_EQUAL_STM) {
        std::cout << test_equal_stm_info.var1.getID();
        std::cout << test_equal_stm_info.var2.getID();
        if (variable_table[QString::fromStdString(test_equal_stm_info.var1.getID())] == variable_table[QString::fromStdString(test_equal_stm_info.var2.getID())])
            return process_result_t("true", "", "");
        else
            return process_result_t("false", "", "");
    } else //print
    {
        QString ret;
        if (print_stm_info.var.getID() == "ALL") {
            QMap<QString, Polynomial>::iterator it = variable_table.begin();
            while (it != variable_table.end()) {
                ret += it.key();
                ret += " = ";
                ret += it.value().printPretty();
                ret += '\n';
                ++it;
            }
        } else {
            ret += variable_table[QString::fromStdString(print_stm_info.var.getID())].printPretty();
        }
        return process_result_t(ret, "", "");
    }
}

void calc_interface::doParse(const QString& main_input)
{
    reset();
    scan(main_input.toStdString());
    yyparse();
}

Polynomial calc_interface::getVariable(const std::string& name)
{
    auto ptr = variable_table.find(QString::fromStdString(name));
    if (ptr == variable_table.end()) {
        parser_error.undefined_reference = true;
        parser_error.var_name = name;
        return Polynomial();
    } else {
        return *ptr;
    }
}

void calc_interface::reset()
{
    assign_stm_info = assign_stm_t();
    test_equal_stm_info = test_equal_stm_t();
    print_stm_info = print_stm_t();
    scanner_error.illegal_token = false;
    scanner_error.illegal_token_pos = 0;
    parser_error.synax_error = false;
    parser_error.undefined_reference = false;
    parser_error.var_name = "";
}

process_result_t::process_result_t(const QString& _m, const QString& _v, const QString& _e)
    : main_result(_m)
    , variable_result(_v)
    , eval_result(_e)
{
}
