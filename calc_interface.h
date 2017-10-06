#ifndef CALC_INTERFACE_H
#define CALC_INTERFACE_H
#include <QPair>
#include <QString>
#include "polynomial.h"
struct scanner_error_t {
    bool illegal_token;
    int illegal_token_pos;
};

struct parser_error_t {
    bool undefined_reference;
    std::string var_name;
    bool synax_error;
};
struct process_result_t {
    process_result_t(const QString& _m, const QString& _v = QString(), const QString& _e = QString());
    QString main_result;
    QString variable_result;
    QString eval_result;
};
enum statement_type_t {
    ASSIGN_STM,
    TEST_EQUAL_STM,
    PRINT_STM
};
struct assign_stm_t {
    Polynomial parsed_input;
    std::string variable_name;
};
struct test_equal_stm_t{
    Polynomial var1;
    Polynomial var2;
};
struct print_stm_t {
    Polynomial var;
};

class calc_interface
{
public:
    calc_interface();
    static process_result_t process_input_main(const QString& main_input, const QString &input_x = QString());
    static void doParse(const QString& main_input);
    static QMap<QString, Polynomial> variable_table;
    static Polynomial getVariable(const std::string& name);

    static assign_stm_t assign_stm_info;
    static test_equal_stm_t test_equal_stm_info;
    static print_stm_t print_stm_info;

    static scanner_error_t scanner_error;
    static parser_error_t parser_error;
    static statement_type_t statement_type;
    static void reset();
};

#endif // CALC_INTERFACE_H
